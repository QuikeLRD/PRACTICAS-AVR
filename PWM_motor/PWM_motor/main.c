/*
CONTROL DE UN MOTOR MEDIANTE PWM
 */ 
#define F_CPU 1000000UL
#include <avr./io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



int main(void)
{
	
	//CONFIGURACIONES GPIO
	
	DDRB  |=  (1 << PB3);																				//CONFIGURAMOS PB3 COMO SALIDA
	DDRD  |=  (1 << PD5);																				//			   PD5 COMO SALIDA		
	PORTB &= ~(1 << PB3);																				//INICIAMOS PB3 EN 0
	PORTD &= ~(1 << PD5);																				//INICIAMOS PD5 EN 0
	
	
	
	//CONFIGURACIONES TIMER0
	TCCR0 &= ~(1 << WGM01);																				//PWM FASE CORRECTA
	TCCR0 |= (1 << WGM00);
	//PRESCALER
	TCCR0 &= ~(1 << CS02);																				//PRESCALER 1
	TCCR0 &= ~(1 << CS01);
	TCCR0 |=  (1 << CS00);		
	//MODO PWM
	TCCR0 |= (1 << COM01);																				//MODO NO INVERTIDO
	TCCR0 &= ~(1 << COM00);																					
	OCR0 = 127;
	//CONFIGURACIONES TIMER1
	TCCR1A &= ~(1 << WGM10);
	TCCR1A |=  (1 << WGM11);																			//PWM FASE CORRECTA
	TCCR1B &= ~(1 << WGM12);
	TCCR1B |=  (1 << WGM13);				
			
	//PRESCALER
	TCCR1B &= ~((1 << CS12)|(1 << CS11));																//PRESCALER 1
	TCCR1B |= (1 << CS10);
	//MODO PWM
	TCCR1A |=  (1 << COM1A1);																			//PWM MODO NO INVERTIDO
	TCCR1A &= ~(1 << COM1A0);																			
	ICR1 = 255;																							//FRECUENCIA PWM =1959.97
	OCR1A = 127;																						
	
	
		
	
	
	

	
    while(1)
    {
        
    }
}