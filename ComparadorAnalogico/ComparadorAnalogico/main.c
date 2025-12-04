/*
======================================================================================
PROJECT:           ANALOG COMPARATOR INTERRUPT PRACTICE
COMPANY:           UPIITA.
FILE:              COMPARADORANALOGICO.C
AUTHOR:            [LUIS ENRIQUE LERDO CRISOSTOMO]
DATE:              [11/20/2025]

DESCRIPTION:
-------------
THIS PROJECT IMPLEMENTS AN INTERRUPT-DRIVEN ANALOG COMPARATOR DEMONSTRATION
USING THE ATMEGA16 MCU AT 1MHZ.

- THE SYSTEM COMPARES AN ANALOG VOLTAGE (AIN0 / PB2) AGAINST A FIXED THRESHOLD (AIN1 / PB3).
- WHEN THE VOLTAGE AT AIN0 CROSSES ABOVE THE REFERENCE AT AIN1, AN INTERRUPT IS TRIGGERED.
- THE LCD DISPLAY SHOWS A MESSAGE WHEN THE INTERRUPT IS DETECTED.
- STARTUP MESSAGES ARE SHOWN TO INDICATE FUNCTIONALITY.

KEY FEATURES:
-------------
1. RELIABLE CONFIGURATION OF MCU'S ANALOG COMPARATOR MODULE AND INTERRUPT.
2. SAFE USE OF LCD DISPLAY ROUTINES OUTSIDE THE INTERRUPT SERVICE ROUTINE.
3. USE OF DIVIDER NETWORK FOR PRECISE REFERENCE VOLTAGE ON THE NEGATIVE INPUT.
4. DESIGNED FOR EASY INTEGRATION INTO LABORATORY OR EMBEDDED SYSTEM DEMOS.

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
#include "Keyboard.h"

volatile uint8_t comp_flag = 0;                           // FLAG SET BY ISR TO SIGNAL COMPARATOR EVENT

// ========================== LCD MESSAGE BUFFERS ===================================
char mensaje1[] = "SE HA ALCANZADO";                         // INTERRUPT MESSAGE
char mensaje4[] = "  EL UMBRAL";
char mensaje2[] = "PRACTICA";                             // STARTUP MESSAGE LINE 1
char mensaje3[] = "COMPARADOR";                           // STARTUP MESSAGE LINE 2

// ======================== ANALOG COMPARATOR INTERRUPT =============================
ISR(ANA_COMP_vect)
{
	comp_flag = 1;                                        // SET FLAG ON COMPARATOR EVENT
	
}

int main(void)
{
	// ============================ HARDWARE INITIALIZATION =========================
	DDRC    = 0xFFU;                                      // LCD: ALL OUTPUTS
	DDRB   &= ~((1 << PB2)|(1 << PB3));                   // AIN0 (PB2) & AIN1 (PB3) AS INPUTS
	DDRA |= (1 << PA0);									  // A0 AS OUTPUT

	PORTB  &= ~((1 << PB2)|(1 << PB3));                   // DISABLE PULL-UPS ON ANALOG INPUTS
	PORTA  &= ~(1 << PA0);								  // PA0 OFF	
	
	ADCSRA &= ~(1 << ADEN);                               // DISABLE ADC MODULE
	SFIOR  &= ~(1 << ACME);                               // DISABLE ADC MULTIPLEXER FOR COMPARATOR
	ACSR   |= (1 << ACI);                                 // CLEAR COMPARATOR INTERRUPT FLAG

	// CONFIGURE COMPARATOR: ENABLE INTERRUPT ON RISING EDGE
	// ACD = 0 (COMPARATOR ENABLED), ACIE = 1 (INTERRUPT ENABLE), ACIS1/ACIS0 = 11 (RISING EDGE)
	ACSR =  (0 << ACD)
	| (0 << ACBG)
	| (0 << ACO)
	| (1 << ACIE)
	| (1 << ACIS1)
	| (1 << ACIS0);

	sei();                                               // ENABLE GLOBAL INTERRUPTS

	// ============================= LCD STARTUP MESSAGES ===========================
	LCD_INICIALIZA();
	LIMPIA_LCD();
	POS_LINEA1(5);
	ENVIA_CADENA(mensaje2);                              // SHOW "PRACTICA"
	POS_LINEA2(5);
	ENVIA_CADENA(mensaje3);                              // SHOW "COMPARADOR"
	_delay_ms(300);
	LIMPIA_LCD();

	// =============================== MAIN APPLICATION LOOP ========================
	while(1)
	{
		if (comp_flag)
		{
			LIMPIA_LCD();
			PORTA |= (1 << PA0);						 // PA0 ON
			POS_LINEA1(0);
			ENVIA_CADENA(mensaje1);                      // SHOW INTERRUPT MESSAGE
			POS_LINEA2(0);
			ENVIA_CADENA(mensaje4);                      // SHOW INTERRUPT MESSAGE
			
			_delay_ms(1000);
			LIMPIA_LCD();
			PORTA &= ~(1 << PA0);						 // PA0 OFF
			comp_flag = 0;                               // RESET FLAG FOR FUTURE EVENTS
			ACSR |= (1 << ACI);                          // OPTIONAL: CLEAR HARDWARE INTERRUPT FLAG
		}
		// MAIN LOOP CAN BE EXTENDED WITH ADDITIONAL FUNCTIONALITY AS DESIRED
	}
}

/*
======================================================================================
END OF FILE
======================================================================================
*/