/*
 * main.c
 *
 * Created: 9/5/2025 12:45:09 PM
 *  Author: enriq
 */ 

#define CPU_F1000000
#include <avr/io.h>
#include <util/delay.h>	
#include <xc.h>


int i = 0xF0, j = 0b10101010, y = 85;	//Variables





int main(void)
{
    
	DDRA = 0x00;						//Puerto A como entrada
	DDRB = 255;							//Puerto B como salida
	DDRC = 0xFF;						//Puerto C como salida
	
	
	while(1)
    {
      PORTB = j;						//Escribimos j en el puerto B	
	  PORTC = y;						//Escribimos y en el puerto C
	  _delay_ms(500);					//Espera 500ms
    }
	
	
}