/*
PRACTICA 6 PUNTO2
*/ 
#define F_CPU 8000000UL
#include <avr/io.h>																		//DEFINE INT AND OUT FROM AVR
#include <avr/interrupt.h>																//DEFINE INTERRUPT 
#include <util/delay.h>
												
int main(void)
{
	
	
	//CONFIGURAMOS ENTRADAS Y SALIDAS	
	DDRB |= (1 <<PB3);																	//PB3 OUTPUT
	PORTB &= ~(1 << PB3);																//PB3 CLEAN
	
	
	DDRA &= ~(1 << DDA0);																//PA0 AS INPUT
	DDRA &= ~(1 << DDA1);																//PA1 AS INPUT
	
	
	TCNT0 = 0;																			//CLEAN REGISTER TCNT0
	OCR0 = 61;																			//INITIAL VALUE OCR0
																				
	TCCR0 = (1 << WGM01)|(0 << WGM00)|(0 << COM01)|(1 << COM00)|						//CTC MODE AND TOGGLE
			(0 << CS02)|(1 << CS01)|(1 << CS00);										//PRESCALER EN 64
						
	
		
	while(1)
    {
		
		selec = 0;
						
		if(!(PINA & (1 << PINA1))){																					//SI PA1 ESTA LOW (CERRADO A GND)
			selec +=2;
		}
		if (!(PINA & (1 << PINA0))){																				//SI PA0 ESTA LOW
			selec += 1;
		}
		
		
		if(selec != selec_anterior)
		{
					
			switch(selec){
				
				
			case 1:
				TCCR0 = (1 << WGM01) | (1 << COM00) | (0 << CS02) | (1 << CS01) | (1 << CS00);						//PRESCALER IN 64 
				OCR0 = OCR_1KHZ;																					//CONFIGURA EL 0CR0
				break;																								//1KHZ
			
			
			case 2:
				
				TCCR0 = (1 << WGM01) | (1 << COM00) | (0 << CS02) | (1 << CS01) | (0 << CS00);						//PRESCALER 8				
				OCR0 =OCR_10KHZ;
				break;	
						
			
			case 3:
				
				TCCR0 = (1 << WGM01) | (1 << COM00) | (0 << CS02) | (1 << CS01) | (0 << CS00);						//PRESCALER 8
				OCR0 = OCR_100KHZ;
				break;	
			default:
				TCCR0 = (1 << WGM01) | (1 << COM00) | (0 << CS02) | (1 << CS01) | (1 << CS00);						//PRESCALER 64
				OCR0 = OCR_1KHZ;
				break;															
			}
			TCNT0 = 0;																								//REINICIAR EL CONTADOR AL CAMBIAR DE MODO, AYUDA A SINCRONIZAR
			selec_anterior = selec;																					//GUARDA EL ESTADO ACTUAL PARA LA PROXIMA COMPARACIÓN
		}
			_delay_ms(20);																							//PEQUEÑO DELAY PARA NO SATURAR LA CPU
    }
}


//CASE 3: 1KHZ			R= 1.2K, 2R= 2.4k C= 100nF 2C =200nF 0.5C = 50nF
//CASE 1: 10KHZ			R= 1.5K, 2R= 3.3k C= 10nF  2C = 20nF 0.5C = 5nF
//CASE 0: 100KHZ		R= 1.5K, 2R= 3.3k C= 1nF   2C = 2nF  0.5C = 0.5nF

//CASE 2: 1KHZ


//RESULTADOS REALES:

//CASE 1: 1.225kHz	 0.124khz
//CASE 0: 12.507 KHZ
//CASE 3: 0.124 Khz