/*
======================================================================================
PROJECT:           PRACTICA 10 - ADC CONTINUO (8 BITS / 10 BITS) CON SELECTOR
COMPANY:           UPIITA-IPN
FILE:              ADC_PRACTICA10_ATMEGA16_SWITCH.C
AUTHOR:            [LUIS ENRIQUE LERDO CRISOSTOMO]
DATE:              [FECHA]

DESCRIPTION:
-------------
SE UTILIZA EL ADC DEL ATMEGA16 EN MODO CONTINUO (FREE RUNNING) PARA MUESTREAR
UN VOLTAJE DE 0 A 5V APLICADO EN EL PIN ADC2 (PA2).

EL RESULTADO SE PRESENTA EN LA LCD COMO VOLTAJE, Y EN LA PRIMERA LINEA
SE INDICA SI LA MEDICION SE HACE A "10 bits" O " 8 bits", SEGUN LA
SELECCION REALIZADA MEDIANTE DOS SWITCHES CONECTADOS A LAS LINEAS A1 Y A0.

CODIFICACION DE SELECCION (SW EN A1:A0):
    A1 A0 = 00  -> MODO 10 BITS  (V=... a 10 bits)
    A1 A0 = 01  -> MODO  8 BITS  (V=... a 8 bits)
    A1 A0 = 10  -> SE TRATA COMO 10 BITS
    A1 A0 = 11  -> SE TRATA COMO  8 BITS

HARDWARE:
---------
- ATmega16 a 1 MHz.
- LCD en PORTC (PC0..PC7).
- Potenciometro RV3 en PA2/ADC2 (rango 0..5V).
- Switches de seleccion en PA0 y PA1.
- AVCC=5V, AREF con capacitor de 100 nF a GND.

======================================================================================
*/

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"

char mensaje1[] = "Modo: 10 bits";
char mensaje2[] = "Modo:  8 bits";


// ------------------------- CONFIGURACION ADC --------------------------------------
#define ADC_VREF   5.0f   // Volts (AVCC)

/* Inicializa ADC para 10 bits, ajuste a la derecha, canal ADC2, Vref = AVCC */
void ADC_Init_10bit(void)
{
    // ADMUX: REFS1:0=01, ADLAR=0, MUX1=1 -> ADC2
    ADMUX = (1 << REFS0) | (1 << MUX1);

    // ADCSRA: ADEN=1, ADATE=1, ADSC=1, ADPS2..0=111 (/128)
    ADCSRA = (1 << ADEN)  |
             (1 << ADATE) |
             (1 << ADSC)  |
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/* Inicializa ADC para 8 bits, ajuste a la izquierda, canal ADC2, Vref = AVCC */
void ADC_Init_8bit(void)
{
    // ADMUX: REFS1:0=01, ADLAR=1, MUX1=1 -> ADC2
    ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1);

    ADCSRA = (1 << ADEN)  |
             (1 << ADATE) |
             (1 << ADSC)  |
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/* Lectura 10 bits (ADLAR=0). Devuelve 0..1023 */
uint16_t ADC_Read_10bit(void)
{
    while (!(ADCSRA & (1 << ADIF)));   // espera fin de conversión
    uint8_t low  = ADCL;               // leer primero ADCL
    uint8_t high = ADCH;
    ADCSRA |= (1 << ADIF);             // limpiar bandera
    return ((uint16_t)high << 8) | low;
}

/* Lectura 8 bits (ADLAR=1, valor en ADCH). Devuelve 0..255 */
uint8_t ADC_Read_8bit(void)
{
    while (!(ADCSRA & (1 << ADIF)));
    uint8_t low  = ADCL;               // lectura dummy obligatoria
    uint8_t high = ADCH;               // 8 MSB
    ADCSRA |= (1 << ADIF);
    (void)low;
    return high;
}

/* Lee el estado de los switches en A1 y A0.
   Devuelve 0..3:  b1 = A1, b0 = A0. */
uint8_t LeeSelector(void)
{
    uint8_t a = PINA & 0x03; // solo PA1..PA0
    return a;
}

// ------------------------------ MAIN ---------------------------------------------
int main(void)
{
    char linea1[20];
    char linea2[20];

    // LCD en PORTC
    DDRC = 0xFF;
    LCD_INICIALIZA();
    LIMPIA_LCD();

    // PA2 (ADC2) como entrada analógica
    DDRA &= ~(1 << PA2);
    PORTA &= ~(1 << PA2);  // sin pull-up

    // PA0 y PA1 como ENTRADAS para switches (pull-ups internos)
    DDRA &= ~((1 << PA0) | (1 << PA1));
    PORTA |=  (1 << PA0) | (1 << PA1);

    while (1) {
        uint8_t sel = LeeSelector();   // lee A1:A0

        // 00 o 10 -> modo 10 bits
        // 01 o 11 -> modo 8 bits
        if ( (sel == 0x00) || (sel == 0x02) ) {
            // ===================== MODO 10 BITS ======================
            ADC_Init_10bit();
            _delay_ms(5);

            uint16_t adc10 = ADC_Read_10bit();
            float volt10 = (adc10 * ADC_VREF) / 1024.0f;

            LIMPIA_LCD();
            POS_LINEA1(0);
            ENVIA_CADENA(mensaje1);

            POS_LINEA2(0);
            sprintf(linea2, "V=%.2f V", volt10);
            ENVIA_CADENA(linea2);
        }
        else {
            // ===================== MODO 8 BITS =======================
            ADC_Init_8bit();
            _delay_ms(5);

            uint8_t adc8 = ADC_Read_8bit();
            float volt8 = (adc8 * ADC_VREF) / 256.0f;

            LIMPIA_LCD();
            POS_LINEA1(0);
            ENVIA_CADENA(mensaje2);

            POS_LINEA2(0);
            sprintf(linea2, "V=%.2f V", volt8);
            ENVIA_CADENA(linea2);
        }

        _delay_ms(300);  // pequeña pausa
    }

    return 0;
}

/*
======================================================================================
END OF FILE
======================================================================================
*/