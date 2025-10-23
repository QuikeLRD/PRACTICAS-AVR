//LERDO CRISOSTOMO LUIS ENRIQUE
//PRACTICA 5 RELOJ
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/iom16.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"


//VARIABLES

volatile uint16_t seconds = 0U;
volatile uint16_t mins = 0U;
volatile uint16_t hours = 0U;



ISR (TIMER0_OVF_vect){
	
	
	
	PORTB ^= (1 << PB5);																//TOGGLE EN PB5
	TCNT0 = 177;																		//CARGAMOS DE NUEVO EL TIMER0
	
}

int main(void)
{
	
	
	DDRB  |= (1 << PB5);																//CONFIGURAMOS PB5 COMO SALIDA
	PORTB &= ~(1 << PB5);																//APAGAMOS PB5				
	//CONFIGURAMOS EL TIMER EN MODO TEMPORIZADOR
	TCNT0 = 177;																		//PRECARGAMOS EL REGISTRO
	TIFR |= (1 <<  TOV0);																//LIMPIAMOS BANDERA	
	TIMSK |= (1 << TOIE0);																//HABILITA INTERRUPCION POR TIMER0
	TCCR0 |= (CS02 << 1)|(CS01 << 0)|(CS00 << 1);													//PRESCALER 1024
	sei();																				//HABILITA INTERRUPCIONES GLOBALES				
	while(1)
	{
		
			
		
	}
}