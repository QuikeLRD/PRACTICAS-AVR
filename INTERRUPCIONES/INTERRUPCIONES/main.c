#include <avr/io.h>
#include <avr/iom16.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"
#include "Keyboard.h"

#define CPU_F 1000000 


static volatile uint8_t  flag = 0U;									//BANDERA DE INTERRUPCIÓN

char mensaje[]  = "Luis Enrique Lerdo Crisostomo";
char mensaje2[] = "PRIMER";
char mensaje3[] = "INTERRUPCION";





//FUNCION PARA OBTENER EL NUMERO DE CARACTERES REALES EN EL TEXTO
static uint8_t str_len(const char *s)
{
	uint8_t n = 0U;
	while(s[n] != '\0' ) {n++;}
	return n;
	
}

static void lcd_prepare_scroll_name(void)
{
	char linea1[40];
	for (uint8_t i = 0U; i<40U; i++){
		linea1[i] = ' ';
	}
	
	//CALCULA LA LONGITUD DEL TEXTO
	const uint8_t len = str_len(mensaje);
	const uint8_t to_copy = (len < 40U) ? len: 40U;					//COPIA EL MENSAJE DENTRO DE LAS 40 PRIMERAS POSICIONES
	for (uint8_t i = 0U; i<to_copy; i++){
		linea1[i] = mensaje[i];
	}
	
	//ESCRIBE LAS 40 POSICIONES DE LA LINEA EN DDRAM
	//PRIMERO ESCRIBIMOS LAS VISIBLES
	POS_LINEA1(0);
	for(uint8_t i = 0U; i<16U; i++){
		ENVIA_DATO(linea1[i]);
	}
	POS_LINEA1(16);
	for(uint8_t i = 16U; i<40U; i++){
		ENVIA_DATO(linea1[i]);
	}
	REGRESO_CASA();	
}


ISR(INT0_vect){
	
			
	flag = 1U;														//ACTIVAMOS INTERRUPCION
	GICR &= (uint8_t)~(1U << INT0);									//ACTIREBOTE POR SOFTWARE	DESHABILITA TEMPORALMENTE INT0
		
	
}

int main (void){
	
	//CONFIGURACIONES LCD
	DDRC = 0xFFU;
	
	//CONFIGURACIONES INTERRUPCION
	DDRD &= (uint8_t)~(1U << PD2);									//PD2 COMO ENTRADA
	PORTD |= (1U << PD2);											//PULL-UP INTERNO ACTIVADO YA QUE EL BOTON VA A GND
	
	//CONFIGURAMOS FORMA DE ACTIVAR LA INTERRUPCIÓN
	MCUCR |= (1U << ISC01);											// CONFIGURAMOS INT0 POR FLANCO DE BAJADA
	MCUCR &= (uint8_t)~(1U << ISC00);
	
	GIFR |= (1U << INTF0);											//LIMPIAMOS BANDERA DE INTERRUPCIONES (POSIBLE BANDERA PENDIENTE POR SEGURIDAD)
	GICR |= (1 << INT0);											//HABILITAMOS FUENTE DE INTERRUPCIONES INT0
	
	sei();															//HABILITAMOS INTERRUPCIONES GLOBALES
	
	LCD_INICIALIZA();												//INICIALIZAMOS LCD
	LIMPIA_LCD();													//LIMPIA LCD
	
	lcd_prepare_scroll_name();										//FUNCION PARA CARGAR  NOMBRES
	
	//DESPLAZAMIENTO INFINITO
	uint8_t shifts = 0U;											//CONTADOR DE DESPLAZAMIENTOS APLICADOS
	while(1){
		
		//RUTINA DE LA PRIMER INTERRUPCION
		if (flag == 1U){
			
			flag = 0U;
						
			LIMPIA_LCD();											//MUESTRA MENSAJE DE LA INTERRUPCION
			POS_LINEA1(0);
			ENVIA_CADENA(mensaje2);
			POS_LINEA2(0);
			ENVIA_CADENA(mensaje3);
			_delay_ms(2000);
			
			
			LIMPIA_LCD();											//RESTAURA EL NOMBRE PARA EL DESPLAZAMIENTO
			lcd_prepare_scroll_name();
			shifts = 0U;											//REINICIA EL CONTEO DE DESPLAZAMIENTOS
			
			GIFR |= (1U << INTF0);									//LIMPIAMOS LA BANDERA DE INT0 EN CASO DE RESIDUO 
			GICR |= (1U << INT0);									//VUELVE A HABILITAR INT0
			
			continue;												//CONTINUA AL SIGUIENTE CICLO SIN DESPLAZAR
		}
		
		//MODO NORMAL SIN INTERRUPCION	
		_delay_ms(100);												//VELOCIDAD DE SCROLL
		DESPLAZAMIENTO_PIZQUIERDA();								//FUNCION PARA DESPLAZAR POR IZQUIERDA
		shifts++;
		if (shifts >= 40U){
			REGRESO_CASA();
			shifts = 0U;
			
		}
		
			
	}
}

