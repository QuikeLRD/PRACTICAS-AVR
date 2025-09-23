
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "Keyboard.h" 

char mensaje[] = "Practica 3";
char mensaje2[] ="Teclado";


int main(void)
{
	// Configura PORTC como salida (todos los pines)
	DDRC = 0xFF;
	DDRD = 0x0F;
	PORTD = 0b00001111;

	// Inicializa el LCD
	LCD_INICIALIZA();
	char tecla = ' ';
	
	// Limpia la pantalla y coloca el cursor en la posición inicial
	LIMPIA_LCD();

	// Escribe un mensaje de bienvenida
	POS_LINEA1(0); // Cursor en línea 1, posición 0
	ENVIA_CADENA(mensaje);
	POS_LINEA2(0); // Cursor en línea 1, posición 0
	ENVIA_CADENA(mensaje2);
	_delay_ms(200);

	CAR_ESP0();
	CAR_ESP1();
	CAR_ESP2();
	CAR_ESP3();
	CAR_ESP4();
	CAR_ESP5();
	CAR_ESP6();
	CAR_ESP7();

	LIMPIA_LCD();
	
	while (1)
	{
		
		tecla = LEE_TECLADO();
		
		ENVIA_CADENA(tecla);	
		_delay_ms(100);
		
		
		
		
	}
}
		
		
		
		
		