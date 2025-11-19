/*
 
 
 
 
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom16.h>
#include <util/delay.h>
#include "LCD.h"
#include "Keyboard.h"

// ========================== LCD MESSAGE BUFFERS ===================================
char mensaje1[]  = "INTERRUPCION COMP";

ISR(ANALOG_COMP_vect){

	LIMPIA_LCD();
	ENVIA_CADENA(mensaje1);
}



int main(void)
{
	
// ============================ HARDWARE INITIALIZATION =========================
	DDRC = 0xFFU;                                                                  //LCD: ALL OUTPUTS
	
	DDRB &= ~((1 << PB2)|(1 << PB3));											   //PB3 AND PB2 AS INPUT
	ACSR  =  (1 << ACIE)|(1 << ACIS1)|(1 << ACIS0);								   //ENABLE COMPARATOR INTERRUPT ON RISING EDGE	
	ACSR |=  (1 << ACD);														   //ACD DISABLE
	
	sei();
	
	
	LCD_INICIALIZA();
	LIMPIA_LCD();
		
    while(1)
    {
         
    }
}