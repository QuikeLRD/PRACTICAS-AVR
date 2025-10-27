/*
PRACTICA 6 PUNTO1
SE SOLICITA REALIZAR UN GENERADOR DE FUNCIONES QUE MEDIANTE UN INTEGRADOR 
MUESTRE UNA SEÑAL SENOIDAL DE 1KHZ, 10KHZ AND 100KHZ

DADO QUE NO ES POSIBLE CUBRIR TODOS LOS VALORES CON LA MISMA CONFIGURACION
PARA 10K Y 100K SE USARA UN PRESCALER DE 8MHZ

 */ 
#define F_CPU 8000000UL
#include <avr/io.h>																		//DEFINE INT AND OUT FROM AVR
#include <avr/interrupt.h>																//DEFINE INTERRUPT 
#include <util/delay.h>

																	

ISR(TIMER0_COMP_vect){
	
	
}



int main(void)
{
	
	//CONFIGURAMOS ENTRADAS Y SALIDAS	
	DDRB |= (1 <<PB3);																	//PB3 OUTPUT
	PORTB &= ~(1 << PB3);																//PB3 CLEAN
	
	DDRA &= ~((1 << PA0) | (1 << PA1));													//PA0 AND PA1 DEFINED AS INPUTS
	
	TCCR0 = 0;																			//CLEAN IT
	TCCR0 |= (1 << WGM01)|(1 << COM00);													//CTC MODE AND TOGGLE
	TCCR0 |= (0 << CS02)|(1 << CS01)|(1 << CS00	);										//PRESCALER EN 64
	TCNT0 = 0;			
	//MODO CTC
														
	OCR0 = 61;	
	
	//VALORES DEL OCR
	const uint8_t OCR_1KHZ	 = 61;
	const uint8_t OCR_10KHZ	 = 49;
	const uint8_t OCR_100KHZ =  4;
	
	OCR0 = OCR_1KHZ;																	//VALOR INICIAL DEL OCR 
	TCNT0 = 0;																			//LIMPIAMOS EL REGISTRO	
	
	while(1)
    {
		
		uint8_t bit0 = (PINA & (1 << PA0)) ? 1 : 0;
		uint8_t bit1 = (PINA & (1 << PA1)) ? 1 : 0;
		uint8_t selec = (bit1 << 1) | bit0; // A1 A0 binario
			
			switch(selec){
				
				
			case 1:
				OCR0 = OCR_1KHZ;														//CONFIGURA EL 0CR0
				TCCR0 |= (0 << CS02)|(1 << CS01)|(1 << CS00	);							//PRESCALER IN 64 
				TCNT0 = 0;
																						//1KHZ
			break;
			
			case 2:
				OCR0 =OCR_10KHZ;
				
				TCCR0 |= (0 << CS02)|(1 << CS01)|(0 << CS00	);							//PRESCALER IN 8
				TCNT0 = 0;
																				//10KHZ 
			break;
			
			case 3:
				OCR0 = OCR_100KHZ;
				TCCR0 |= (0 << CS02)|(1 << CS01)|(0 << CS00	);							//PRESCALER IN 8
				TCNT0 = 0;
			break;
			
			default:
			break;																//100KHZ
			}
			_delay_ms(15);        
    }
}