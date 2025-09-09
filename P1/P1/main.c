/*
 * main.c
 *
 * Created: 9/7/2025 12:47:14 PM
 *  Author: enriq
 */ 

#define CPU_F 1000000
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>


	int i = 0xF0, j = 0b10101010, y = 85, ValorA;


int main(void)
{
	
	DDRA = 0x00;
	DDRB = 255;
	DDRC = 0xFF;
	
	
    while(1)
    {
        ValorA = PINA;
		PORTB  = ValorA & 0xF0;		//Nibble Alto
		PORTC  = ValorA & 0x0F;	    //Nibble Bajo 	
	
		
		
		 
    }
}