#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "Keyboard.h"

char mensaje[]  = "Practica 3";
char mensaje2[] = "Teclado";

int main(void)
{
	// LCD en PORTC como salida
	DDRC = 0xFF;

	// Teclado: PD7..PD4 = columnas (salida), PD3..PD0 = filas (entrada)
	DDRD  = 0xF0;
	PORTD =  0x0F;

	// Estado inicial: activar COL1 y habilitar pull-ups en filas (bits bajos en 1)
	PORTD = COL1; // Esto ya pone nibble alto = columnas (COL1) y nibble bajo = 0x0F (pull-ups)

	LCD_INICIALIZA();
	LIMPIA_LCD();

	POS_LINEA1(0);
	ENVIA_CADENA(mensaje);
	POS_LINEA2(0);
	ENVIA_CADENA(mensaje2);
	_delay_ms(700);

	LIMPIA_LCD();

	char tecla;

	while (1)
	{
		tecla = LEE_TECLADO();

		if (tecla != ' ')
		{
			ENVIA_DATO(tecla);

			// Esperar a que se suelte la tecla, pero SIGUIENDO barriendo
			do {
				BARRE_TECLADO();
				_delay_ms(2);
			} while (LEE_TECLADO() != ' ');

			_delay_ms(80); // antirrebote
		}

		// Barrer continuamente para poder detectar la próxima tecla
		BARRE_TECLADO();
		_delay_ms(2); // pequeño tiempo de asentamiento
	}
}