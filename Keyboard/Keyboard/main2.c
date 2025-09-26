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

	// Inicializa los caracteres especiales
	CAR_ESP0();
	CAR_ESP1();
	CAR_ESP2();
	CAR_ESP3();
	CAR_ESP4();
	CAR_ESP5();
	CAR_ESP6();
	CAR_ESP7();

	char tecla;
	char modo_especial = 0;

	while (1)
	{
		tecla = LEE_TECLADO();

		if (tecla != ' ')
		{
			POS_LINEA1(0);

			if (!modo_especial)
			{
				// Modo normal: imprime teclas normales
				if (tecla == 'A') {
					modo_especial = 1; // Cambia a modo especial
					LIMPIA_LCD();
					POS_LINEA1(0);
					ENVIA_CADENA("Modo especial!");
					_delay_ms(500);
					LIMPIA_LCD();
				}
				else
				{
					ENVIA_DATO(tecla);
				}
			}
			else
			{
				// Modo especial: teclas imprimen caracteres especiales
				switch (tecla)
				{
					case '1': ENVIA_DATO(0); break; // CAR_ESP0
					case '2': ENVIA_DATO(1); break; // CAR_ESP1
					case '3': ENVIA_DATO(2); break; // CAR_ESP2
					case '4': ENVIA_DATO(3); break; // CAR_ESP3
					case '5': ENVIA_DATO(4); break; // CAR_ESP4
					case '6': ENVIA_DATO(5); break; // CAR_ESP5
					case '7': ENVIA_DATO(6); break; // CAR_ESP6
					case '8': ENVIA_DATO(7); break; // CAR_ESP7
					case 'D': // Si quieres que D regrese al modo normal
					modo_especial = 0;
					LIMPIA_LCD();
					POS_LINEA1(0);
					ENVIA_CADENA("Modo normal");
					_delay_ms(500);
					LIMPIA_LCD();
					break;
					default:
					// Si quieres que otras teclas no hagan nada en modo especial
					break;
				}
			}

			// Espera a que se suelte la tecla (antirrebote)
			do {
				BARRE_TECLADO();
				_delay_ms(2);
			} while (LEE_TECLADO() != ' ');

			_delay_ms(80);
		}

		BARRE_TECLADO();
	}
}