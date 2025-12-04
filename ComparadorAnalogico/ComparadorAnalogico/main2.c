/*
======================================================================================
PROJECT:           ANALOG COMPARATOR INTERRUPT PRACTICE POINT 2
COMPANY:           UPIITA-IPN
FILE:              COMPARADORANALOGICO2.C
AUTHOR:            LUIS ENRIQUE LERDO CRISOSTOMO
DATE:              2025-11-20

DESCRIPTION:
-------------
THIS PROJECT DEMONSTRATES INTERRUPT-DRIVEN CONTROL USING THE ATMEGA16 MCU ANALOG COMPARATOR
(PRACTICE POINT 2). WHEN THE VOLTAGE AT THE ANALOG INPUT (AIN0 = PB2) EXCEEDS THE THRESHOLD
(REF = AIN1 = PB3), AN INTERRUPT IS GENERATED. THIS EVENT IS USED TO CONTROL A PWM OUTPUT
(PD5/OC1A), BY CHANGING THE DUTY CYCLE IN REAL TIME THROUGH SOFTWARE.

- ANALOG COMPARATOR CONFIGURED TO GENERATE AN INTERRUPT ON RISING VOLTAGE CROSSING.
- TIMER1 IS USED IN PHASE CORRECT PWM MODE, WITH OUTPUT OC1A (PD5).
- LCD DISPLAY INITIALIZED FOR LAB INTERACTION AND BASIC FEEDBACK.
- PWM DUTY CYCLE IS MODIFIED WHEN COMPARATOR INTERRUPT IS TRIGGERED.

KEY FEATURES:
-------------
1. USE OF VOLTAGE COMPARATOR TO DETECT CROSSING EVENTS.
2. USE OF INTERRUPT SERVICE ROUTINE TO MODIFY PWM DUTY CYCLE.
3. CLEAR SEPARATION BETWEEN ISR FAST HANDLING AND MAIN LOOP CONTROL LOGIC.
4. PRACTICAL INTEGRATION FOR LAB DEMO: PWM CAN DRIVE LED/ACTUATOR ON PD5.

REVISION HISTORY:
-----------------
- 2025-11-20 LELC: INITIAL LAB VERSION, ANALOG COMPARATOR WITH TIMER1 PWM INTEGRATION.

======================================================================================

*/

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom16.h>
#include <util/delay.h>
#include "LCD.h"
#include "Keyboard.h"

// ========================== GLOBAL FLAGS/MESSAGES ===================================
volatile uint8_t comp_flag = 0;          // SET BY ISR ON COMPARATOR EVENT

char mensaje2[] = "PRACTICA";            // LCD STARTUP MSG
char mensaje3[] = "COMPARADOR 2";        // LCD STARTUP MSG #2

// ======================== ANALOG COMPARATOR INTERRUPT =============================
ISR(ANA_COMP_vect)
{
	comp_flag = 1;                       // SET FLAG FOR MAIN LOOP ACTION
}

// ============================ MAIN FUNCTION ========================================
int main(void)
{
	// LCD AND I/O INITIALIZATION
	DDRC   = 0xFFU;                      // LCD: ALL OUTPUTS
	DDRB  &= ~((1 << PB2)|(1 << PB3));   // AIN0 (PB2) & AIN1 (PB3): INPUT
	DDRD  |=  (1 << PD5);                // OC1A (PWM OUTPUT) PIN AS OUTPUT

	PORTB &= ~((1 << PB2)|(1 << PB3));   // DISABLE PULL-UPS FOR ANALOG INPUTS
	PORTD &= ~(1 << PD5);                // INITIAL OC1A LOW

	// ANALOG COMPARATOR SETUP
	ADCSRA &= ~(1 << ADEN);              // DISABLE ADC MODULE
	SFIOR  &= ~(1 << ACME);              // ANALOG COMP MULTIPLEXER DISABLED
	ACSR   |= (1 << ACI);                // CLEAR COMPARATOR INTERRUPT FLAG

	// ENABLE COMPARATOR INTERRUPT (RISING EDGE)
	// ACD = 0 (enabled), ACIE = 1, ACIS1 = 1, ACIS0 = 1
	ACSR =  (0 << ACD)
	| (0 << ACBG)
	| (0 << ACO)
	| (1 << ACIE)
	| (1 << ACIS1)
	| (1 << ACIS0);

	// TIMER1: PHASE CORRECT PWM MODE, OC1A OUTPUT
	// WGM13 = 1, WGM11 = 1 (TCCR1B/TCCR1A), others = 0: Phase Correct PWM, TOP = ICR1
	TCCR1A &= ~(1 << WGM10);
	TCCR1A |=  (1 << WGM11);
	TCCR1B &= ~(1 << WGM12);
	TCCR1B |=  (1 << WGM13);

	// NON-INVERTED PWM OC1A (COM1A1=1, COM1A0=0)
	TCCR1A |=  (1 << COM1A1);
	TCCR1A &= ~(1 << COM1A0);

	// TIMER1 PRESCALER = 1 (CS10=1)
	TCCR1B &= ~((1 << CS12)|(1 << CS11));
	TCCR1B |= (1 << CS10);

	// PWM FREQUENCY SET BY ICR1 (TOP): e.g., ICR1 = 255 for ~1.96kHz at F_CPU=1MHz,prescaler=1
	ICR1 = 255;

	sei();                               // ENABLE GLOBAL INTERRUPTS

	// LCD STARTUP
	LCD_INICIALIZA();
	LIMPIA_LCD();
	POS_LINEA1(5); ENVIA_CADENA(mensaje2);
	POS_LINEA2(5); ENVIA_CADENA(mensaje3);
	_delay_ms(300);
	LIMPIA_LCD();

	// =============================== MAIN LOOP =====================================
	while(1)
	{
		// CHECK IF COMPARATOR INTERRUPT HAS OCCURRED
		if (comp_flag)
		{
			LIMPIA_LCD();
			OCR1A = 200;             // SET DUTY CYCLE TO 50% WHEN EVENT OCCURS
			// Option: add display message or signal LED if desired

			comp_flag = 0;           // CLEAR FLAG FOR FUTURE EVENTS
			ACSR |= (1 << ACI);      // CLEAR COMPARATOR INTERRUPT FLAG
		}
		// Extend with additional event handling if desired
	}
}

/*
======================================================================================
END OF FILE
======================================================================================
*/