/*
======================================================================================
PROJECT:       SERVO MOTOR PWM CONTROL 
COMPANY:       UPIITA-IPN.
FILE:          SERVOCONTROL_PWM.C
AUTHOR:        [LERDO CRISOSTOMO LUIS ENRIQUE]
DATE:          [29/11/2025]

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"
#include "Keyboard.h"

// ======================== INTERRUPT AND STATE FLAGS ===============================
static volatile uint8_t  flag  = 0U;                                               // INT2 (PB2) TRIGGER FLAG
volatile uint8_t         sign;                                                     // CONTROL VARIABLE FOR INPUT STATE

// ========================== LCD MESSAGE BUFFERS ===================================
char mensaje1[]  = "CONTROL DE UN";
char mensaje2[]  = "SERVO MOTOR";
char mensaje3[]  = "ANGULO: ";
char mensaje4[]  = "NUEVO PWM:";

// ======================= KEYPAD AND DISPLAY BUFFERS ===============================
char BUF[16];                                                                      // TEMPORARY BUFFER FOR KEYPRESS
char PRO[16];                                                                      // BUFFER USED FOR CONCATENATED NUMERICAL ENTRY
char PORCENTAJE[16];                                                               // STORES FINAL USER-ENTERED DUTY STRING
char CYCLE_DUTY[16];                                                               // FOR FORMATTED LCD DISPLAY (E.G., "75%")
char S_porcentaje[] = "%";                                                         // PERCENT SYMBOL STRING

// ================================ GLOBAL VARIABLES ================================
uint16_t P_motor;                                                                  // MOTOR DUTY CYCLE (AS INTEGER %)
char     tecla;                                                                    // KEY RECEIVED FROM KEYPAD

// ================== UTILITY: CLEARS SPECIFIED CHAR ARRAY ==========================
void clearArray(char* myArray, short sizeOfMyArray) {
	memset(myArray, 0, sizeof(char) * sizeOfMyArray);                              // CLEARS THE MEMORY REGION
}

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

	DDRB  |=  (1 << PB3);                                                          // 0C0  AS OUTPUT
	DDRD  |=  (1 << PD5);														   // 0C1A AS OUTPUT 
	DDRD  |=  (1 << PD4);														   // OC1B AS OUTPUT	 
	DDRB  &= ~(1 << PB2);                                                          // PB2 AS INPUT (INT2)
	
	PORTB &= ~(1 << PB3);                                                          // PWM0 START LOW
	PORTB &= ~(1 << PD5);                                                          // PWM1 START LOW
	PORTB &= ~(1 << PD4);														   // PWM1 START LOW
	PORTB |=  (1 << PB2);                                                          // ENABLE PULL-UP RESISTOR ON PB2
	
	
	//SETINGS  TIMER1
	TCCR1A |= (1 << WGM10);														   // PWM FAST MODE 15			
	TCCR1A |= (1 << WGM11);																		
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << WGM13);

	//PRESCALER
	TCCR1B &= ~(1 << CS12);														  //PRESCALER 64
	TCCR1B |=  (1 << CS11);
	TCCR1B |=  (1 << CS10);
	//PWM MODE
	TCCR1A |=  (1 << COM1A1);													  //PWM NON INVERTED MODE
	TCCR1A &= ~(1 << COM1A0);													
	
   TCCR1A |=   (1 << COM1B1);
   TCCR1A &=  ~(1 << COM1B0);
	
	
	OCR1A = 312;
	OCR1B = 19;																 	
	
	

while(1)
{
	
}
}