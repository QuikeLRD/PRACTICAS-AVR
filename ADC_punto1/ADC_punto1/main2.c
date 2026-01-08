/*
======================================================================================
PROJECT:       PRACTICA 10B - MEDIDOR DE VOLTAJE 0–25 V CON ATENUADOR PI (CALIBRADO)
COMPANY:       UPIITA-IPN
FILE:          ADC_PRACTICA10_ATENUADOR_PI_25V_CAL_LINEAL.C
AUTHOR:        [LUIS ENRIQUE LERDO CRISOSTOMO]
DATE:          [FECHA]
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

// Referencia del ADC (AVCC real)
#define ADC_VREF      5.00f      // ajusta si tu AVCC real no es 5.00 V

// Relación ideal del atenuador: Vfuente_ideal = Vadc * 5
#define ATTEN_IDEAL   5.0f

// ---- CALIBRACIÓN LINEAL ----
// Primero calculas Vfuente_ideal = Vadc * ATTEN_IDEAL.
// Luego aplicas una corrección lineal Vfuente = Vfuente_ideal * GAIN + OFFSET.
//
// GAIN y OFFSET los sacas de DOS puntos de medición:
//
// 1) Elimina momentáneamente cualquier corrección extra (pon GAIN=1, OFFSET=0).
// 2) Elige dos voltajes de entrada (por ejemplo 12.3 V y 17.3 V).
// 3) Anota lo que muestre la LCD en esos puntos: V1_med y V2_med.
// 4) Calcula:
//
//     m = (V2_real - V1_real) / (V2_med - V1_med)
//     b = V1_real - m * V1_med
//
// 5) Pon GAIN = m, OFFSET = b aquí.

m = 1.6025;
b = 232.99;

#define GAIN    1.00f   // <-- AJUSTA CON TUS MEDICIONES
#define OFFSET  0.00f   // <-- AJUSTA CON TUS MEDICIONES

// ======================= PROTOTIPOS DE FUNCIONES ADC ==============================
void     ADC_Init_10bit(void);
void     ADC_Init_8bit(void);
uint16_t ADC_Read_10bit(void);
uint8_t  ADC_Read_8bit(void);
uint8_t  LeeSelector(void);

// ============================== CONFIGURACION ADC =================================

void ADC_Init_10bit(void)
{
	// REFERENCIA = AVCC, AJUSTE A LA DERECHA, CANAL ADC2
	ADMUX = (1 << REFS0) | (1 << MUX1);

	ADCSRA = (1 << ADEN)  |
	(1 << ADATE) |
	(1 << ADSC)  |
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);   // prescaler /128
}

void ADC_Init_8bit(void)
{
	// REFERENCIA = AVCC, AJUSTE A LA IZQUIERDA, CANAL ADC2
	ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1);

	ADCSRA = (1 << ADEN)  |
	(1 << ADATE) |
	(1 << ADSC)  |
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_Read_10bit(void)
{
	while (!(ADCSRA & (1 << ADIF)));
	uint8_t low  = ADCL;
	uint8_t high = ADCH;
	ADCSRA |= (1 << ADIF);
	return ((uint16_t)high << 8) | low;
}

uint8_t ADC_Read_8bit(void)
{
	while (!(ADCSRA & (1 << ADIF)));
	uint8_t low  = ADCL;
	uint8_t high = ADCH;
	ADCSRA |= (1 << ADIF);
	(void)low;
	return high;
}

uint8_t LeeSelector(void)
{
	return (PINA & 0x03);  // PA1:PA0
}

// =============================== FUNCION PRINCIPAL ================================

int main(void)
{
	char linea2[20];

	// LCD EN PORTC
	DDRC = 0xFFU;
	LCD_INICIALIZA();
	LIMPIA_LCD();

	// PA2 COMO ENTRADA ANALOGICA (ADC2)
	DDRA  &= ~(1 << PA2);
	PORTA &= ~(1 << PA2);

	// PA0 Y PA1 COMO ENTRADAS DIGITALES PARA SELECTOR (CON PULL-UPS)
	DDRA  &= ~((1 << PA0) | (1 << PA1));
	PORTA |=  (1 << PA0) | (1 << PA1);

	while (1) {
		uint8_t sel = LeeSelector();

		if ((sel == 0x00) || (sel == 0x02)) {
			// ===================== MODO 10 BITS ================================
			ADC_Init_10bit();
			_delay_ms(5);

			uint16_t adc10 = ADC_Read_10bit();          // 0..1023

			// Voltaje en nodo ADC
			float Vadc = (adc10 * ADC_VREF) / 1024.0f;

			// Voltaje calculado en la fuente usando el atenuador ideal
			float Vfuente_ideal = Vadc * ATTEN_IDEAL;

			// Corrección lineal
			float Vfuente = Vfuente_ideal * GAIN + OFFSET;

			// LIMITES
			if (Vfuente < 0.0f)  Vfuente = 0.0f;
			if (Vfuente > 25.0f) Vfuente = 25.0f;

			LIMPIA_LCD();
			POS_LINEA1(0);
			ENVIA_CADENA(mensaje1);

			POS_LINEA2(0);
			sprintf(linea2, "Vf=%.2f V", Vfuente);
			ENVIA_CADENA(linea2);
			} else {
			// ===================== MODO 8 BITS ================================
			ADC_Init_8bit();
			_delay_ms(5);

			uint8_t adc8 = ADC_Read_8bit();             // 0..255

			float Vadc = (adc8 * ADC_VREF) / 256.0f;
			float Vfuente_ideal = Vadc * ATTEN_IDEAL;
			float Vfuente = Vfuente_ideal * GAIN + OFFSET;

			if (Vfuente < 0.0f)  Vfuente = 0.0f;
			if (Vfuente > 25.0f) Vfuente = 25.0f;

			LIMPIA_LCD();
			POS_LINEA1(0);
			ENVIA_CADENA(mensaje2);

			POS_LINEA2(0);
			sprintf(linea2, "Vf=%.2f V", Vfuente);
			ENVIA_CADENA(linea2);
		}

		_delay_ms(300);
	}

	return 0;
}