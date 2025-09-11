
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h" 

char mensaje[] = "Hola, LCD!";



int main(void)
{
	// Configura PORTC como salida (todos los pines)
	DDRC = 0xFF;

	// Inicializa el LCD
	LCD_INICIALIZA();

	
	// Limpia la pantalla y coloca el cursor en la posición inicial
	LIMPIA_LCD();
	POS_LINEA1(0); // Cursor en línea 1, posición 0

	// Escribe un mensaje de bienvenida
	ENVIA_CADENA(mensaje);
	_delay_ms(200);

	CAR_ESP0();
	CAR_ESP1();
	CAR_ESP2();
	CAR_ESP3();
	CAR_ESP4();
	CAR_ESP5();
	CAR_ESP6();
	CAR_ESP7();

	while (1)
	{
		
				
		LIMPIA_LCD();
		
		//GIRASOL1
		POS_LINEA1(0);
		ENVIA_DATO(3);
		_delay_ms(500);
		
		//GIRASOL2
		POS_LINEA2(0);
		ENVIA_DATO(3);
		_delay_ms(500);
		
		//ZOMBI1 SPAM
		POS_LINEA1(15);
		ENVIA_DATO(5);
		_delay_ms(500);
		//BORRO ZOMBI1
		POS_LINEA1(15);
		ENVIA_DATO(' ');  // Escribe espacio para borrar
		_delay_ms(10);
				
		//ZOMBI P1
		POS_LINEA1(14);
		ENVIA_DATO(5);
		_delay_ms(500);
		//BORRO ZOMBI P1
		POS_LINEA1(14);
		ENVIA_DATO(' ');  
		_delay_ms(10);
		
		//ZOMBI1 P2
		POS_LINEA1(13);
		ENVIA_DATO(5);
		_delay_ms(500);
		
		//PONGO PLANTA
		POS_LINEA1(1);
		ENVIA_DATO(0);
		//BORRO ZOMBI1 P2
		POS_LINEA1(13);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI1 P3
		POS_LINEA1(12);
		ENVIA_DATO(5);
		_delay_ms(500);
				
		//DISPARO
		POS_LINEA1(2);
		ENVIA_DATO(4);
		//BORRO ZOMBI1 P3
		POS_LINEA1(12);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI1 P4
		POS_LINEA1(11);
		ENVIA_DATO(5);
		_delay_ms(500);
		
		//DISPARO
		POS_LINEA1(3);
		ENVIA_DATO(4);
		//BORRO ZOMBI1 P4
		POS_LINEA1(11);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI1 P5
		POS_LINEA1(10);
		ENVIA_DATO(5);
		_delay_ms(500);
		
		//DISPARO
		POS_LINEA1(4);
		ENVIA_DATO(4);
		//BORRO ZOMBI1 P4
		POS_LINEA1(10);
		ENVIA_DATO(' ');
		_delay_ms(10);
		
		//ZOMBI2 P1
		POS_LINEA2(15);
		ENVIA_DATO(7);
		
		//ZOMBI P6
		POS_LINEA1(9);
		ENVIA_DATO(5);
				
		//DISPARO
		POS_LINEA1(5);
		ENVIA_DATO(4);
		//BORRO ZOMBI P2
		POS_LINEA1(9);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//BORRO ZOMBI2 P2
		POS_LINEA1(15);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI P3
		POS_LINEA1(8);
		ENVIA_DATO(5);
		//ZOMBI2 P1
		POS_LINEA2(14);
		ENVIA_DATO(7);		
		_delay_ms(500);
		
		
		
		
		
		
		
	}
}