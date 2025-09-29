#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "Keyboard.h"

char mensaje[]  = "Practica 3";
char mensaje2[] = "Teclado";

int main(void)
{
	
	DDRC = 0xFF;					// PORTC SALIDAS

									// TECLADO: PD7..PD4 COLUMNAS (SALIDA), PD3..PD0 FILAS (ENTRADA)
	DDRD  = 0xF0;
	PORTD = COL1;					// COLUMNA INICIAL + PULL UPS FILAS (NIBBLE BAJO = 0x0F)

	LCD_INICIALIZA();
	LIMPIA_LCD();

	POS_LINEA1(0);
	ENVIA_CADENA(mensaje);
	POS_LINEA2(0);
	ENVIA_CADENA(mensaje2);
	_delay_ms(800);
	LIMPIA_LCD();

	// CARGAR CARACTERES ESPECIALES
	CAR_ESP0();
	CAR_ESP1();
	CAR_ESP2();
	CAR_ESP3();
	CAR_ESP4();
	CAR_ESP5();
	CAR_ESP6();
	CAR_ESP7();

	char tecla;
	uint8_t modo_especial = 0;     // 0 = NORMAL, 1 = ESPECIAL
	char tecla_anterior = ' ';     // ANTIREBOTE
	uint8_t cursor_pos = 0;        // AVANZAR EN LA LINEA

	while (1)
	{
		tecla = LEE_TECLADO();		//LEEMOS TECLADO

		if (tecla != ' ' && tecla != tecla_anterior)
		{
			// --------- TECLA 'A' CAMBIA EL MODO ----------
			if (tecla == 'A')
			{
				modo_especial = !modo_especial; 
				LIMPIA_LCD();
				cursor_pos = 0;
			}
			else
			{
				// --------- MODO ESPECIAL ----------
				if (modo_especial == 1)
				{
					
					if (tecla >= '1' && tecla <= '7')
					{
						uint8_t idx = tecla - '0';  // '1'->1 ... '7'->7
						// MOVER CURSOR 
						
						POS_LINEA1(cursor_pos);
						ENVIA_DATO(idx);            // MOSTRAR CARACTER ESPECIAL
						if (cursor_pos < 15) cursor_pos++;
					}
					else if (tecla == '8')
					{
						// IMPRIMIMOS 8 NORMAL
						POS_LINEA1(cursor_pos);
						ENVIA_DATO('8');
						if (cursor_pos < 15) cursor_pos++;

						
					}
					else
					{
						// IMPRIMIMOS NORMAL CUALQUIER OTRA TECLA
						POS_LINEA1(cursor_pos);
						ENVIA_DATO(tecla);
						if (cursor_pos < 15) cursor_pos++;
					}
				}
				// --------- MODO NORMAL ----------
				else
				{
					POS_LINEA1(cursor_pos);
					ENVIA_DATO(tecla);
					if (cursor_pos < 15) cursor_pos++;
				}
			}

			// GUARDA LA TECLA ACTUAL PARA EVITAR REPETICIONES
			tecla_anterior = tecla;
		}
		else if (tecla == ' ')
		{
			// CUANDO NO HAY TECLA PODEMOS RESETEAR LA TECLA ANTERIOR
			tecla_anterior = ' ';
		}

		// BARRIDO Y ANTIREBOTE
		BARRE_TECLADO();
		_delay_ms(3);
	}
}