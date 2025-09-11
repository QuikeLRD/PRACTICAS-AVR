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


	int i, j = 0b10101010, y = 85;


int main(void)
{
	
	DDRA = 0xFF;
	DDRB = 0xFF;
	
	
	
    while(1)
    {
        
		//AVANZAMOS PORTB DE 0 A 7  
		
		for(i = 0; i<8; i++ )
		{
			PORTA = 0x00;
			PORTB = (1<<i);
			_delay_ms(80);
		}
		
		//AVANZAMOS PORTA DE 0 A 7
		for(i =0; i<8; i++)
		{
			PORTA = (1<<i);
			PORTB = 0x00;
			_delay_ms(80);
		} 
		
		//REGRESAMOS DE PORTA 
		
		for(i=7; i>=0; i--)
		{
			PORTA = (1<<i);
			PORTB = 0x00;
			_delay_ms(80);
		}
	
	
		//REGRESAMOS DE PORTB
		
		for(i=7; i>=0; i--)
		{
			PORTA = 0x00;
			PORTB = (1<<i);
			_delay_ms(80);
		}
	
		
		 
    }
}