//LERDO CRISOSTOMO LUIS ENRIQUE
//PRACTICA 5 RELOJ
#define CPU_F 1000000UL
#include <avr/io.h>
#include <avr/iom16.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"


//VARIABLES

volatile uint8_t seconds = 0U;
volatile uint8_t minutes = 0U;
volatile uint8_t hours = 0U;

 char mensaje[] = "00:00:00";

//BANDERA DE INTERRUPCION QUE INDICA QUE HA PASADO 1S EXACTO
volatile uint8_t second_tick = 0U;

//ESTADOS INTERNOS DEL TEMPORIZADOR
static volatile uint8_t ovf_in_second = 0;
static volatile uint8_t corr_index = 0U;

//REALIZAMOS LA PRECARGA A TCNT0 PARA COMPLETAR 1 SEG
//209, 209, 208
//PRECARGAMOS 47,47,48
static inline uint8_t preload_for_next_short(void){
	uint8_t preload = (corr_index < 2U)? 47U : 48U;
	corr_index++;
	if(corr_index >=3)	corr_index = 0U;
	return preload;
}

//FUNCION PARA IMPRIMIR
static inline void lcd_print2(uint8_t v){
	
	ENVIA_DATO((char)('0' + (v / 10)));
	ENVIA_DATO((char)('0' + (v % 10)));
}

static inline void lcd_print_time(uint8_t h, uint8_t m, uint8_t s){
	
	POS_LINEA1(0);
	lcd_print2(h);
	ENVIA_DATO(':');
	lcd_print2(m);
	ENVIA_DATO(':');
	lcd_print2(s);
	
}


ISR (TIMER0_OVF_vect){
	
	switch(ovf_in_second){
		
		case 0:						//TERMINA EL PRIMER OVERFLOW
			ovf_in_second =1;
			break;	
		
		case 1:						//TERMINA EL SEGUNDO OVERFLOW
			ovf_in_second =2;
			break;
			
		case 2:						//TERMINA EL TERCER OVERFLOW
		{
			uint8_t preload = preload_for_next_short();
			TCNT0 = preload;
			ovf_in_second = 3;
			break;	
		}
		
		case 3:						//TERMINA EL OVERFLOW CORTO Y HA PASADO 1 SEG
			second_tick = 1U;		//NOTIFICAMOS AL MAIN
			TCNT0 = 0U;				//EL TCNT0 SE VACIA PARA COMENZAR UN OVERFLOW LARGO
			ovf_in_second = 0U;		//REINICIA EL CICLO DE 4 OVERFLOWS
			break;
		
		default:
			ovf_in_second = 0U;
			break;		
	}
}

int main(void)
{
	
	// Configura PORTC como salida (todos los pines)
	DDRC = 0xFF;
	//INICIALIZAMOS
	LCD_INICIALIZA();
	// Limpia la pantalla y coloca el cursor en la posición inicial
	LIMPIA_LCD();
	
	
	POS_LINEA1(0);
	ENVIA_CADENA(mensaje);
	
	//CONFIGURACION DE LOS REGISTROS DEL TIMER0
	//REGISTRO DE CUENTA DE EVENTOS
	TCCR0 = 0;
	TCCR0  |= (1U << CS02)|(1U << CS00);							//PRESCALER 1024
	
	//REGISTRO DE LA MASCARA DE INTERRUPCION
	TIMSK |= (1U << TOIE0);														//HABILITAMOS INT TEMP0
	
	//LIMPIAMOS BANDERAS 
	TIFR |= (1U << TOV0);														//LIMPIAMOS BANDERA INT T0
	
	sei();	
	

    while(1)
    {
        
		
		
		if(second_tick){
			
			//TOMAMOS Y LIMPIAMOS LA BANDERA
			
			second_tick = 0U;
			
			//ACTUALIZAMOS HORAS, MIN, SEGS
			seconds++;
			if(seconds >= 60U){
				seconds = 0U;
				minutes++;
				if(minutes >= 60U){
					minutes = 0U;
					hours++;
					if(hours >= 24U)
					hours = 0U;
				}
			}
			
				lcd_print_time(hours, minutes, seconds);		
		}
	
		
		
	
	
		
	}
}