/*
======================================================================================
PROJECT:      COMPARATOR 
COMPANY:       UPIITA-IPN.
FILE:          MOTORCONTROL_PWM.C
AUTHOR:        [LERDO CRISOSTOMO LUIS ENRIQUE]
DATE:          [5/11/2025]

DESCRIPTION:
-------------
THIS PROJECT IMPLEMENTS A USER-INTERACTIVE DC MOTOR SPEED CONTROL SYSTEM USING PWM

- THE SYSTEM IS BASED ON AN ATMEGA16 MCU RUNNING AT 1MHZ.
- USER INPUTS ARE PROVIDED VIA A MATRIX KEYPAD.
- REAL-TIME FEEDBACK IS SHOWN ON AN LCD DISPLAY.
- PWM DUTY CYCLE CAN BE SET BY ENTERING A PERCENTAGE VALUE ON THE KEYPAD.
- RESET AND VALUE ENTRY OPERATIONS ARE MANAGED BY INT2 (PB2) HARDWARE INTERRUPT.

REVISION HISTORY:
-----------------
- [DATE] [AUTHOR/DEPT]: INITIAL VERSION.

======================================================================================
*/

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom16.h>
#include <util/delay.h>
#include "LCD.h"

// ========================== LCD MESSAGE BUFFERS ===================================
char mensaje1[]  = "CONTROL DE UN";
char mensaje2[]  = "  MOTOR DC";
char mensaje3[]  = "CICLO DE TRABAJO:";
char mensaje4[]  = "NUEVO PWM:";

// ================================ GLOBAL VARIABLES ================================
uint16_t P_motor;                                                                  // MOTOR DUTY CYCLE (AS INTEGER %)
char     tecla;                                                                    // KEY RECEIVED FROM KEYPAD

// ================== UTILITY: CLEARS SPECIFIED CHAR ARRAY ==========================


// ======================== EXTERNAL INTERRUPT: INT2 (PB2) ==========================
ISR(INT2_vect) {
	flag = 1;                                                                      // SET FLAG TO RESTART DATA ENTRY AFTER USER INTERRUPTION
	GICR &= (uint8_t)~(1 << INT2);                                                 // DISABLE INT2 UNTIL PROCESSED IN MAIN LOOP
}

int main(void) {
	// ============================ HARDWARE INITIALIZATION =========================
	DDRC = 0xFFU;                                                                  // LCD: ALL OUTPUTS
	DDRD = 0xF0U;                                                                  // KEYPAD: PD0->PD3 INPUT, PD4->PD7 OUTPUT
	PORTD = COL1;

	DDRB  |=  (1 << PB3);                                                          // PB3 AS PWM OUTPUT
	DDRD  |=  (1 << PD5);                                                          // PD5 AS OUTPUT (FUTURE USE)
	PORTB &= ~(1 << PB3);                                                          // START PWM LOW
	PORTD &= ~(1 << PD5);

	DDRB  &= ~(1 << PB2);                                                          // PB2 AS INPUT (INT2)
	PORTB |=  (1 << PB2);                                                          // ENABLE PULL-UP RESISTOR ON PB2

	// TIMER0: PHASE CORRECT PWM, NON-INVERTING, PRESCALER=1
	TCCR0 &= ~(1 << WGM01);                                                        // PHASE CORRECT PWM MODE
	TCCR0 |=  (1 << WGM00);
	TCCR0 &= ~(1 << CS02);
	TCCR0 &= ~(1 << CS01);
	TCCR0 |=  (1 << CS00);
	TCCR0 |=  (1 << COM01);                                                        // NON-INVERTING OUTPUT
	TCCR0 &= ~(1 << COM00);

	// INT2: RISING EDGE TRIGGER
	MCUCSR |=  (1 << ISC2);                                                        // CONFIGURE INT2 FOR RISING EDGE
	GIFR   |=  (1 << INTF2);
	GICR   |=  (1 << INT2);

	LCD_INICIALIZA();
	LIMPIA_LCD();
	POS_LINEA1(0);
	ENVIA_CADENA(mensaje1);
	POS_LINEA2(0);
	ENVIA_CADENA(mensaje2);
	_delay_ms(800);
	LIMPIA_LCD();

	sei();                                                                         // ENABLE GLOBAL INTERRUPTS

	// ======================== MAIN APPLICATION CYCLE ==============================
	while(1) {
		tecla = LEE_TECLADO();
		if((tecla != ' ') && (tecla != 'A') && (tecla != 'B') && (tecla != 'C') &&
		(tecla != 'D') && (tecla != '#') && (tecla != '*') && (sign != 2)) {

			sprintf(BUF, "%c", tecla);                                             // STORE NEW CHARACTER IN BUFFER
			strcat(PRO, BUF);                                                      // CONCATENATE TO MAIN ENTRY BUFFER
			LIMPIA_LCD();
			ENVIA_CADENA(PRO);                                                     // SHOW ENTERED DIGITS
			_delay_ms(200);

			} else if(tecla == '#') {

			strcat(PORCENTAJE, PRO);                                               // TRANSFER CONCATENATED INPUT
			clearArray(PRO, 16);                                                   // CLEAN TEMP BUFFERS
			clearArray(BUF, 16);
			LIMPIA_LCD();
			_delay_ms(100);

			P_motor = atoi(PORCENTAJE);                                            // CONVERT USER ENTRY TO INTEGER PERCENT
			POS_LINEA1(0);
			ENVIA_CADENA(mensaje3);
			sign = 1;                                                              // MOVE TO NEXT STAGE FOR PWM UPDATE
		}
		BARRE_TECLADO();
		_delay_ms(3);

		// ----------- PWM UPDATE: DUTY CYCLE -----------
		if (sign == 1) {
			if (P_motor > 100)
			P_motor = 100;                                                     // LIMIT TO 100%
			sprintf(CYCLE_DUTY, "%d", P_motor);
			strcat(CYCLE_DUTY, S_porcentaje);
			POS_LINEA2(10);
			ENVIA_CADENA(CYCLE_DUTY);

			OCR0 = (255 * P_motor) / 100;                                          // SET NEW DUTY CYCLE
		}

		// ----------- INTERRUPT: NEW ENTRY REQUEST -----------
		if (flag == 1) {
			OCR0 = 0;                                                              // STOP PWM (IF DESIRED)
			LIMPIA_LCD();
			POS_LINEA1(0);
			ENVIA_CADENA(mensaje4);

			clearArray(PRO, 16);
			clearArray(BUF, 16);
			clearArray(PORCENTAJE, 16);
			clearArray(CYCLE_DUTY, 16);
			sign = 0;                                                              // ALLOW FOR NEW INPUT
			flag = 0;                                                              // RESET INTERRUPT FLAG

			GICR |= (1 << INT2);                                                   // RE-ENABLE INT2 FOR FURTHER USE
			_delay_ms(300);                                                        // SIMPLE DEBOUNCE
		}

		tecla = ' ';
	} // END MAIN LOOP
}

/*
======================================================================================
END OF FILE
======================================================================================
*/