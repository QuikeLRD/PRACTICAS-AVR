//PRACTICA CONTADOR DE OBJETOS
//PASOS QUE TENGO QUE HACER PARA RESOLVER LA PRACTICA

//CONFIGURAR REGISTROS PARA EL TECLADO
//REGISTROS DE TIMER PARA EL CONTADOR

#define  F_CPU 1000000UL
#include <avr/io.h>
#include <avr/iom16.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"
#include "Keyboard.h"


//VARIABLES DE LA LCD
char mensaje[]  = "Contador de";
char mensaje1[] = "Productos";
char mensaje3[] = "Productos: ";


//VARIABLES PARA EL TECLADO
char tecla;																						//ALMACENA EL CARACTER DETECTADO		
char tecla_anterior = ' ';
char cursor_pos = 0;


//VARIABLES PARA ALMAENAR LA CUENTA
 char BUF[16];																					//BUFER PARA RECIBIR CADA CARACTER QUE LE ESCRIBAMOS
 char PRO[16];																					//ALMACENA EL CARACTER FINAL CONCATENADO
 uint16_t productos;																			//NUMERO DE PRODUCTOS
 char DATO[16];																					//DATO DEL NUMERO DE PRODUCTOS A GUARDAR
 char DATO_LCD[16];																				//DATO MOSTRADO EN CADENA PARA LA LCD
 uint16_t sign;																					//VARIABLE DE CONTROL

//LIMPIA LOS ARREGLOS QUE SE NECESITEN
void clearArray(char* myArray, short sizeOfMyArray){
		memset(myArray, 0, sizeof(char)*16);													//LIMPIA EL ESPACIO QUE APUNTE 
		
	
}


ISR(TIMER0_OVF_vect){
	
	PORTB ^= (1 << PB5);																		//CAMBIA DE ESTADO CADA INTERUPCION
	TCNT0 = 256-productos;																				//CARGAMOS EL REGISTRO A 1 DEL DESBORDAMIENTO
	
}



int main(void)
{
	//CONFIGURACIONES DE LCD Y TECLADO
	DDRC = 0xFFU;																				//PORTC COMO SALIDA
	DDRD = 0xF0U;																				//TECLADO PD0->PD3 (ENTRADA)  PD4->PD7 (SALIDA)
	PORTD = COL1;
	
	//CONFIGURACION DE GPIO
	DDRB |= (1 << PB5);																			//CONFIGURAMOS PB5 COMO SALIDA
	DDRB &= ~(1 << PB0);																		//CONFIGURAMOS PB0 COMO ENTRADA
	PORTB |=  (1 << PB0);																		// PULL-UP INTERNO											
	
	PORTB &= ~(1 << PB5);																		//INICIA APAGADO
	//CONFIGURACION DE REGISTROS DEL TIMER0
	TCCR0 = 0; 
	TCCR0 |= (1 << CS02)|(1 << CS01)|(0 << CS00);												//FUENTE EXTERNA POR FLANCO DE BAJADA
	
	TIFR |= (1<<TOV0);																			//LIMPIAMOS BANDERA
	TIMSK |= (1 << TOIE0);																		//HABILITAMOS INTERRUPCION T0
	sei();
	
	//INICIO
	LCD_INICIALIZA();
	LIMPIA_LCD();
	
	POS_LINEA1(0);
	ENVIA_CADENA(mensaje);
	POS_LINEA2(0);
	ENVIA_CADENA(mensaje1);
	_delay_ms(800);
	LIMPIA_LCD();
	
		
    while(1)
    {
        tecla = LEE_TECLADO();
		
		if((tecla != ' ') && (tecla != 'A') && (tecla != 'B') && (tecla != 'C') && (tecla != 'D') && (tecla != '#') && (tecla != '*') && (sign !=2)){
			
			sprintf(BUF, "%c", tecla);															//GUARDA EL CARACTER EN UN STRING BUFFER
			strcat(PRO, BUF);																	//CONCATENA EL CARACTER AL STRING QUE GUARDA EL DATO
			LIMPIA_LCD();
			ENVIA_CADENA(PRO);																	//IMPRIMI EN PANTALLA LA CADENA CONCATENADA
			
			_delay_ms(200);																		//ANTIREBOTE PARA NO ESCRIBIR MUCHAS VECES																	
		}	
		else if(tecla == '#'){
				 
				strcat(DATO,PRO);																//PASAMOS LA CADENA GUARDADA EN PRO A DATO
				clearArray(PRO,16);																//LIMPIAMOS ALMACENADOR DE TECLADO	
				clearArray(BUF,16);																//LIMPIAMOS BUFFER
				LIMPIA_LCD();
				sign = 1;																		//VARIABLE DE CONTROL
				_delay_ms(100);
				productos = atoi(DATO);															//CONVIERTE MI CADENA A ENTERO
				TCNT0 = 256-productos;															//CARGAMOS EL TIMER PARA QUE SE DESBORDE CON EL SIGUIENTE PULSO
				TIFR |= (1 << TOV0);
				sei();
		}
		BARRE_TECLADO();
		_delay_ms(3);
		
		
		if(sign == 1){
		sprintf(DATO_LCD, "%d", productos);														//CONVIERTO MI NUMERO A CADENA PARA MOSTRAR EN LA LCD															
		POS_LINEA1(0);
		ENVIA_CADENA(mensaje3);
		POS_LINEA1(10);
		ENVIA_CADENA(DATO_LCD);
		_delay_ms(100);
		}
		
		tecla = ' ';
		
		
				
	}
}