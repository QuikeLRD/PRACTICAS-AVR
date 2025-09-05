/*
 * main.c
 *
 * Created: 9/5/2025 4:45:44 PM
 *  Author: enriq
 */ 

#define  CPU_F1000000
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>


int i= 0xF0, j = 0b10101010, y = 85;


int main(void)
{
	
	DDRA = 0x00;
	DDRB = 255;
	DDRC = 0xFF;
	
    while(1)
    {
    
	i = PINA;
	_delay_ms(100);
	PORTB = i;
	_delay_ms(100);
	
		
    }
}