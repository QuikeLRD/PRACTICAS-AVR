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
	PORTD = COL1;

	LCD_INICIALIZA();
	LIMPIA_LCD();

	POS_LINEA1(0);
	ENVIA_CADENA(mensaje);
	POS_LINEA2(0);
	ENVIA_CADENA(mensaje2);
	_delay_ms(800);

	 LIMPIA_LCD();


	// Inicializa los caracteres especiales (¡importante!)
	CAR_ESP0();
	CAR_ESP1();
	CAR_ESP2();
	CAR_ESP3();

	char tecla;

	while (1)
	{
		tecla = LEE_TECLADO();

		if (tecla != ' ')
		{
			POS_LINEA1(0);

			// Mostrar carácter especial según la tecla
			if (tecla == 'A') {
				ENVIA_DATO(0); // CAR_ESP0
				} else if (tecla == 'B') {
				ENVIA_DATO(1); // CAR_ESP1
				} else if (tecla == 'C') {
				ENVIA_DATO(2); // CAR_ESP2
				} else if (tecla == 'D') {
				ENVIA_DATO(3); // CAR_ESP3
				} else {
				ENVIA_DATO(tecla); // Caracter normal
			}

		BARRE_TECLADO();
		_delay_ms(10);
	}
}
}