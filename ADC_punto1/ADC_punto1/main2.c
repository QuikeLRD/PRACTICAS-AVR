/*
======================================================================================
PROJECT:       PRACTICA 10B - MEDIDOR DE VOLTAJE CALIBRADO (SINCRONIZADO)
COMPANY:       UPIITA-IPN
FILE:          ADC_PRACTICA10_CALIBRADO.C
AUTHOR:        [LUIS ENRIQUE LERDO CRISOSTOMO]
DATE:          [FECHA]

DESCRIPTION:
-------------
ESTE PROYECTO UTILIZA UN FACTOR DE ATENUACION CALIBRADO PARA MOSTRAR EN
LA LCD EL VOLTAJE REAL DE UNA FUENTE DE 0 A 25?V CONECTADA AL ATENUADOR Y
AL MICROCONTROLADOR ATMEGA16.

DURANTE LA SIMULACION EN PROTEUS, SE MIDE LA RELACION:
    k = V_ADC / V_FUENTE

EL FACTOR DE ATENUACION (ATTEN_FACTOR = 1/k) ES AJUSTADO EN EL CÓDIGO
PARA COINCIDIR CON LOS DATOS DE LA FUENTE Y DEL ADC.

LA LCD PRESENTA:

- LINEA 1: "Modo: 10 bits" O "Modo:  8 bits"
- LINEA 2: "Vf=xx.xx V" (VOLTAJE REAL DE LA FUENTE).

HARDWARE:
---------
- ATmega16 a 1 MHz (cristal externo)
- LCD conectada a PORTC (PC0–PC7).
- Atenuador tipo ? conectando la fuente (0..25?V) al pin ADC2 (PA2).
- Factores ajustados según medición en Proteus.

REVISION HISTORY:
-----------------
- [FECHA] [LELC]: Versión inicial calibrada.

======================================================================================
*/

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"

// ========================= CONSTANTES ANALOGICAS ==================================

// Referencia de ADC (AVCC)
#define ADC_VREF      5.0f

// Factor de calibración ajustado en simulación:
// k = V_ADC / V_FUENTE ? ATTEN_FACTOR = 1/k
#define ATTEN_FACTOR  4.9f // Ajusta este valor con base en tus mediciones

// ======================= PROTOTIPOS DE FUNCIONES ADC ==============================
void     ADC_Init_10bit(void);
void     ADC_Init_8bit(void);
uint16_t ADC_Read_10bit(void);
uint8_t  ADC_Read_8bit(void);
uint8_t  LeeSelector(void);

// ============================== CONFIGURACION ADC =================================

/* ----------------------------------------------------------------------------
   ADC_Init_10bit
   CONFIGURA EL ADC PARA MEDICION A 10 BITS EN EL CANAL ADC2.
-----------------------------------------------------------------------------*/
void ADC_Init_10bit(void)
{
    ADMUX = (1 << REFS0) | (1 << MUX1); // AVCC referencia, ADC2, ajuste a la derecha

    ADCSRA = (1 << ADEN)  |   // Enable ADC
             (1 << ADATE) |   // Auto-trigger (free running)
             (1 << ADSC)  |   // Start conversion
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler /128
}

/* ----------------------------------------------------------------------------
   ADC_Init_8bit
   CONFIGURA EL ADC PARA MEDICION A 8 BITS EN EL CANAL ADC2.
-----------------------------------------------------------------------------*/
void ADC_Init_8bit(void)
{
    ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1); // AVCC referencia, ADC2, ajuste a la izquierda

    ADCSRA = (1 << ADEN)  | 
             (1 << ADATE) |
             (1 << ADSC)  | 
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/* ----------------------------------------------------------------------------
   ADC_Read_10bit
   DEVUELVE EL VALOR ADC A 10 BITS (0..1023).
-----------------------------------------------------------------------------*/
uint16_t ADC_Read_10bit(void)
{
    while (!(ADCSRA & (1 << ADIF)));   // Wait for conversion
    uint8_t low  = ADCL;               // Read low bits first
    uint8_t high = ADCH;               // Read high bits
    ADCSRA |= (1 << ADIF);             // Clear the interrupt flag
    return ((uint16_t)high << 8) | low;
}

/* ----------------------------------------------------------------------------
   ADC_Read_8bit
   DEVUELVE EL VALOR ADC A 8 BITS (0..255).
-----------------------------------------------------------------------------*/
uint8_t ADC_Read_8bit(void)
{
    while (!(ADCSRA & (1 << ADIF)));   // Wait for conversion
    uint8_t low  = ADCL;               // Dummy read of lower 8 bits
    uint8_t high = ADCH;               // Read upper 8 bits
    ADCSRA |= (1 << ADIF);             // Clear the interrupt flag
    (void)low;                         // Avoid warning
    return high;
}

/* ----------------------------------------------------------------------------
   LeeSelector
   LEE EL ESTADO DE LOS SWITCHES DE SELECCION (A1:A0).
-----------------------------------------------------------------------------*/
uint8_t LeeSelector(void)
{
    return (PINA & 0x03); // Return the lower 2 bits
}

// =============================== FUNCION PRINCIPAL ================================

int main(void)
{
    char linea1[20];
    char linea2[20];
    char mensaje1[] = "Modo: 10 bits";
    char mensaje2[] = "Modo:  8 bits";

    // --------------------------- INICIALIZACION HARDWARE -------------------------
    DDRC = 0xFFU; // LCD en PORTC
    LCD_INICIALIZA();
    LIMPIA_LCD();

    DDRA &= ~(1 << PA2);   // ADC2 entrada
    PORTA &= ~(1 << PA2);  // No pull-up

    // PA0 y PA1 como entradas digitales para selector (pull-up activado)
    DDRA &= ~((1 << PA0) | (1 << PA1));
    PORTA |=  (1 << PA0) | (1 << PA1);

    // --------------------------- BUCLE PRINCIPAL ---------------------------------
    while (1) {
        uint8_t sel = LeeSelector();   // Lee A1:A0

        // Decodificacion:
        // 00 o 10 ? Modo 10 bits
        // 01 o 11 ? Modo 8 bits
        if ((sel == 0x00) || (sel == 0x02)) {
            ADC_Init_10bit();
            _delay_ms(5);

            uint16_t adc10 = ADC_Read_10bit();

            // Calcula voltaje real:
            float Vadc = (adc10 * ADC_VREF) / 1024.0f; // Voltaje en nodo ADC
            float Vfuente = Vadc * ATTEN_FACTOR;       // Voltaje real de la fuente

            LIMPIA_LCD();
            POS_LINEA1(0);
            ENVIA_CADENA(mensaje1);

            POS_LINEA2(0);
            sprintf(linea2, "Vf=%.2f V", Vfuente);
            ENVIA_CADENA(linea2);
        }
        else {
            ADC_Init_8bit();
            _delay_ms(5);

            uint8_t adc8 = ADC_Read_8bit();

            // Calcula voltaje real:
            float Vadc = (adc8 * ADC_VREF) / 256.0f; // Voltaje en nodo ADC
            float Vfuente = Vadc * ATTEN_FACTOR;    // Voltaje real de la fuente

            LIMPIA_LCD();
            POS_LINEA1(0);
            ENVIA_CADENA(mensaje2);

            POS_LINEA2(0);
            sprintf(linea2, "Vf=%.2f V", Vfuente);
            ENVIA_CADENA(linea2);
        }

        _delay_ms(300); // Retardo
    }

    return 0;
}

