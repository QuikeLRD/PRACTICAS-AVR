#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "Keyboard.h"

char mensaje[] = "Practica 3";
char mensaje2[] ="Teclado";

int main(void)
{
	DDRC = 0xFF;
	DDRD = 0xF0;
	
	LCD_INICIALIZA();
	LIMPIA_LCD();

	POS_LINEA1(0);
	ENVIA_CADENA(mensaje);
	POS_LINEA2(0);
	ENVIA_CADENA(mensaje2);
	_delay_ms(800);

	LIMPIA_LCD();

	char tecla = ' ';
	while (1)
	{
		tecla = LEE_TECLADO();
		if (tecla != ' ')
		{
			ENVIA_DATO(tecla);
			while(LEE_TECLADO() != ' '); // Espera a que se suelte
			_delay_ms(100);
		}
		BARRE_TECLADO();
		_delay_ms(5);
	}
}