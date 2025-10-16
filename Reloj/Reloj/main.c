//LERDO CRISOSTOMO LUIS ENRIQUE
//PRACTICA 5 "Reloj"
#include <avr/io.h>
#include <avr/iom16.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"


#define CPU_F 1000000
//CONFIGURACIONES LCD

DDRC = 0XFFU;

//BANDERAS DE INTERRUPCION


ISR (TIMER0 OVF vect){
	
	
		
	
	
}




int main(void)
{
	
	//CONFIGURACION DE LOS REGISTROS DEL TIMER0
	
	
	
	
	
	sei();	
	
	
	
    while(1)
    {
        
    }
}