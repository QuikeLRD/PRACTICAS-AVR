/*
 * main.c
 *
 * Created: 9/7/2025 12:47:14 PM
 *  Author: enriq
 */ 

#define CPU_F1000000
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>


	int i = 0xF0, j = 0b10101010, y = 85;


int main(void)
{
	
	DDRA = 0x00;
	DDRB = 0x255;
	DDRC = 0xFF;
	
	
    while(1)
    {
        
		PORTB = j;
		PORTC = y;
		_delay_ms(500);
		PORTB = y;
		PORTC = j;
		_delay_ms(500);
		
		 
    }
}