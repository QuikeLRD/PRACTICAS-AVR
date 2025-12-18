/*
======================================================================================
PROJECT:       PRACTICA 10B - MEDIDOR DE VOLTAJE 0–25 V CON ATENUADOR PI
COMPANY:       UPIITA-IPN
FILE:          ADC_PRACTICA10_ATENUADOR_PI_25V.C
AUTHOR:        [LUIS ENRIQUE LERDO CRISOSTOMO]
DATE:          [FECHA]

DESCRIPTION:
-------------
ESTE PROYECTO UTILIZA EL ADC DEL MICROCONTROLADOR ATMEGA16 PARA MEDIR EL VOLTAJE
DE UNA FUENTE DE 0 A 25 VOLTS. COMO EL ADC SOLO ADMITE 0–5V, SE EMPLEA UN
ATENUADOR TIPO PI PARA REDUCIR EL VOLTAJE DE ENTRADA ANTES DE APLICARLO AL ADC.

EL ATENUADOR PROPUESTO ES:

          Vin ----[ 10k ]----+----[ 10k ]---- (sin conectar / alta Z)
                              |
                             [ 5k ]
                              |
                             GND

EL NODO CENTRAL (UNION DE LAS DOS RESISTENCIAS DE 10k Y LA DE 5k) ES CONECTADO
AL PIN PA2/ADC2 DEL ATMEGA16.

RELACION DEL ATENUADOR:
-----------------------
ANALITICAMENTE:

    k = V_ADC / V_FUENTE = Rshunt / (2*Rserie + Rshunt)

PARA Rserie = 10k Y Rshunt = 5k:

    k = 5k / (2*10k + 5k) = 5 / 25 = 0.2

POR LO TANTO:

    V_ADC   = 0.2 * V_FUENTE    (0..5V EN EL ADC)
    V_FUENTE = 5 * V_ADC        (0..25V EN LA FUENTE)

EL PROGRAMA REALIZA CONVERSIONES ADC CONTINUAS (FREE RUNNING) EN EL CANAL ADC2
Y CALCULA EL VOLTAJE REAL DE LA FUENTE A PARTIR DEL VALOR MEDIDO EN EL ADC.

SE SOPORTAN DOS MODOS DE RESOLUCION, SELECCIONADOS MEDIANTE SWITCHES EN A1 Y A0:

    A1 A0 = 00  -> MODO 10 BITS  (V_FUENTE CALCULADO A 10 BITS)
    A1 A0 = 01  -> MODO  8 BITS  (V_FUENTE CALCULADO A 8 BITS)
    A1 A0 = 10  -> SE TRATA COMO 10 BITS
    A1 A0 = 11  -> SE TRATA COMO  8 BITS

LA LCD MUESTRA:

    - LINEA 1: "Modo: 10 bits" O "Modo:  8 bits"
    - LINEA 2: "Vf=xx.xx V" (VOLTAJE REAL DE LA FUENTE 0..25V)

HARDWARE:
---------
- ATmega16 A 1 MHz (CRISTAL EXTERNO).
- LCD CONECTADA AL PUERTO C (PC0..PC7) MEDIANTE "LCD.h".
- ATENUADOR PI 10k-5k-10k CONECTADO A LA FUENTE (0..25V).
- NODO CENTRAL DEL ATENUADOR CONECTADO A PA2/ADC2.
- SWITCHES DE SELECCION EN PA0 Y PA1 (ENTRADAS DIGITALES).
- AVCC A 5V, AREF CON CAPACITOR DE 100 nF A GND (REFERENCIA ANALOGICA = AVCC).

REVISION HISTORY:
-----------------
- [FECHA] [LELC]: VERSION INICIAL DE LA PRACTICA 10B (0–25V CON ATENUADOR PI).

======================================================================================
*/

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"

char mensaje1[] = "Modo: 10 bits";
char mensaje2[] = "Modo:  8 bits";


// ========================= CONSTANTES ANALOGICAS ==================================

// TENSION DE REFERENCIA DEL ADC (AVCC)
#define ADC_VREF      5.0f     // Volts

// FACTOR DE ATENUACION DEL PI PROPUESTO:
//  V_ADC   = 0.2 * V_FUENTE
//  V_FUENTE = 5 * V_ADC
#define ATTEN_FACTOR  5.0f     // V_FUENTE = V_ADC * 5.0  (0..25V)

// ======================= PROTOTIPOS DE FUNCIONES ADC ==============================
void     ADC_Init_10bit(void);
void     ADC_Init_8bit(void);
uint16_t ADC_Read_10bit(void);
uint8_t  ADC_Read_8bit(void);
uint8_t  LeeSelector(void);

// ============================== CONFIGURACION ADC =================================

/* ----------------------------------------------------------------------------
   ADC_Init_10bit
   CONFIGURA EL ADC PARA:

   - REFERENCIA: AVCC (REFS1:0 = 01)
   - AJUSTE A LA DERECHA: ADLAR = 0 (RESOLUCION COMPLETA DE 10 BITS)
   - CANAL: ADC2 (MUX1=1, MUX4..0 = 00010)
   - MODO: FREE RUNNING (ADATE=1, ADTS=000)
   - PRESCALER: /128 (ADPS2:0 = 111) -> F_ADC ~ 7.8 kHz @ 1 MHz

-----------------------------------------------------------------------------*/
void ADC_Init_10bit(void)
{
    // ADMUX: [REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0]
    //        [  0     1     0    0    0    0    1    0 ] -> 0x42
    ADMUX = (1 << REFS0) | (1 << MUX1);

    // ADCSRA: [ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0]
    //          1    1     1    0    0    1     1     1
    ADCSRA = (1 << ADEN)  |   // HABILITA ADC
             (1 << ADATE) |   // AUTO DISPARO (FREE RUNNING)
             (1 << ADSC)  |   // INICIA PRIMERA CONVERSION
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // PRESCALER 128
}

/* ----------------------------------------------------------------------------
   ADC_Init_8bit
   CONFIGURA EL ADC PARA:

   - REFERENCIA: AVCC (REFS1:0 = 01)
   - AJUSTE A LA IZQUIERDA: ADLAR = 1 (SE UTILIZA SOLO ADCH -> 8 BITS)
   - CANAL: ADC2 (MUX1=1)
   - MODO: FREE RUNNING (ADATE=1)
   - PRESCALER: /128

-----------------------------------------------------------------------------*/
void ADC_Init_8bit(void)
{
    // ADMUX: REFS1:0=01, ADLAR=1, MUX1=1
    ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1);

    ADCSRA = (1 << ADEN)  |
             (1 << ADATE) |
             (1 << ADSC)  |
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/* ----------------------------------------------------------------------------
   ADC_Read_10bit
   LEE EL RESULTADO DE 10 BITS (ADLAR=0) DESDE LOS REGISTROS ADCL Y ADCH.

   - ESPERA A QUE LA BANDERA ADIF SE PONGA EN 1.
   - LEE PRIMERO ADCL Y LUEGO ADCH.
   - CONCATENA AMBOS BYTE PARA OBTENER UN VALOR EN EL RANGO 0..1023.

-----------------------------------------------------------------------------*/
uint16_t ADC_Read_10bit(void)
{
    while (!(ADCSRA & (1 << ADIF)));   // ESPERA FIN DE CONVERSION
    uint8_t low  = ADCL;               // LEER PRIMERO ADCL
    uint8_t high = ADCH;               // LUEGO ADCH
    ADCSRA |= (1 << ADIF);             // LIMPIA BANDERA ADIF ESCRIBIENDO 1
    return ((uint16_t)high << 8) | low;
}

/* ----------------------------------------------------------------------------
   ADC_Read_8bit
   LEE UN RESULTADO DE 8 BITS (ADLAR=1).

   - ADCL CONTIENE LOS BITS MENOS SIGNIFICATIVOS (SE DESCARTAN).
   - ADCH CONTIENE LOS 8 BITS MAS SIGNIFICATIVOS QUE SE UTILIZAN COMO
     RESULTADO (0..255).

-----------------------------------------------------------------------------*/
uint8_t ADC_Read_8bit(void)
{
    while (!(ADCSRA & (1 << ADIF)));
    uint8_t low  = ADCL;               // LECTURA DUMMY REQUERIDA
    uint8_t high = ADCH;               // 8 BITS UTILES
    ADCSRA |= (1 << ADIF);
    (void)low;                         // EVITA WARNING POR VARIABLE NO USADA
    return high;
}

/* ----------------------------------------------------------------------------
   LeeSelector
   LEE EL ESTADO DE LOS SWITCHES DE SELECCION CONECTADOS EN PA1 Y PA0.

   RETORNA UN VALOR DE 0 A 3 CORRESPONDIENTE A:

       bit1 = PA1
       bit0 = PA0

-----------------------------------------------------------------------------*/
uint8_t LeeSelector(void)
{
    return (PINA & 0x03);              // MASCARA 00000011
}

// =============================== FUNCION PRINCIPAL ================================

int main(void)
{
    char linea1[20];
    char linea2[20];

    // --------------------------- INICIALIZACION HARDWARE -------------------------
    // LCD CONECTADA A PORTC
    DDRC = 0xFFU;
    LCD_INICIALIZA();
    LIMPIA_LCD();

    // PA2 COMO ENTRADA ANALOGICA (ADC2)
    DDRA  &= ~(1 << PA2);              // ENTRADA
    PORTA &= ~(1 << PA2);              // SIN PULL-UP

    // PA0 Y PA1 COMO ENTRADAS DIGITALES PARA SELECTOR (CON PULL-UPS)
    DDRA  &= ~((1 << PA0) | (1 << PA1));
    PORTA |=  (1 << PA0) | (1 << PA1); // SWITCH A GND -> NIVEL BAJO AL ACTIVAR

    // --------------------------- BUCLE PRINCIPAL ---------------------------------
    while (1) {
        uint8_t sel = LeeSelector();   // LEE A1:A0

        // CODIFICACION:
        // 00 o 10 -> MODO 10 BITS
        // 01 o 11 -> MODO  8 BITS
        if ((sel == 0x00) || (sel == 0x02)) {

            // ===================== MODO 10 BITS ================================
            ADC_Init_10bit();          // CONFIGURA ADC
            _delay_ms(5);              // PEQUEÑO TIEMPO DE ESTABILIZACION

            // LECTURA RAW DEL ADC (0..1023)
            uint16_t adc10 = ADC_Read_10bit();

            // CALCULO DEL VOLTAJE EN EL PIN ADC:
            // V_ADC = (ADC10 * VREF) / 1024
            float Vadc = (adc10 * ADC_VREF) / 1024.0f;

            // CALCULO DEL VOLTAJE REAL DE LA FUENTE (ANTES DEL ATENUADOR):
            // V_FUENTE = V_ADC * ATTEN_FACTOR  (ATTEN_FACTOR = 5.0)
            float Vfuente = Vadc * ATTEN_FACTOR;

            // DESPLIEGUE EN LCD
            LIMPIA_LCD();
            POS_LINEA1(0);
            ENVIA_CADENA(mensaje1);

            POS_LINEA2(0);
            sprintf(linea2, "Vf=%.2f V", Vfuente);
            ENVIA_CADENA(linea2);
        }
        else {

            // ===================== MODO 8 BITS =================================
            ADC_Init_8bit();
            _delay_ms(5);

            // LECTURA RAW DEL ADC (0..255)
            uint8_t adc8 = ADC_Read_8bit();

            // V_ADC = (ADC8 * VREF) / 256
            float Vadc = (adc8 * ADC_VREF) / 256.0f;

            // V_FUENTE = V_ADC * ATTEN_FACTOR
            float Vfuente = Vadc * ATTEN_FACTOR;

            // DESPLIEGUE EN LCD
            LIMPIA_LCD();
            POS_LINEA1(0);
            ENVIA_CADENA(mensaje2);

            POS_LINEA2(0);
            sprintf(linea2, "Vf=%.2f V", Vfuente);
            ENVIA_CADENA(linea2);
        }

        // PEQUEÑO RETARDO ENTRE ACTUALIZACIONES
        _delay_ms(300);
    }

    // NUNCA DEBE LLEGAR AQUI
    return 0;
}

/*
======================================================================================
END OF FILE
======================================================================================
*/