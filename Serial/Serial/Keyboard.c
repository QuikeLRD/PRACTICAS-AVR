#include <avr/io.h>
#include "Keyboard.h"

char LEE_TECLADO(void)
{
	// Valor por defecto: sin tecla
	char key = ' ';

	// Leemos filas (bits 0..3)
	uint8_t filas = PIND & 0x0F;

	// Evaluamos la columna ACTIVA.
	// Nota: comparamos todo el puerto (como tus macros) o, si prefieres robustez,
	// puedes comparar solo el nibble alto: (PORTD & 0xF0) == (COL1 & 0xF0), etc.
	switch (PORTD) {
		case COL1:
		switch (filas) {
			case FIL1: key = '1'; break;
			case FIL2: key = '2'; break;
			case FIL3: key = '3'; break;
			case FIL4: key = 'A'; break;
			default:   key = ' ';  break; // IMPORTANTE
		}
		break;

		case COL2:
		switch (filas) {
			case FIL1: key = '4'; break;
			case FIL2: key = '5'; break;
			case FIL3: key = '6'; break;
			case FIL4: key = 'B'; break;
			default:   key = ' ';  break; // IMPORTANTE
		}
		break;

		case COL3:
		switch (filas) {
			case FIL1: key = '7'; break;
			case FIL2: key = '8'; break;
			case FIL3: key = '9'; break;
			case FIL4: key = 'C'; break;
			default:   key = ' ';  break; // IMPORTANTE
		}
		break;

		case COL4:
		switch (filas) {
			case FIL1: key = '*'; break;
			case FIL2: key = '0'; break;
			case FIL3: key = '#'; break;
			case FIL4: key = 'D'; break;
			default:   key = ' ';  break; // IMPORTANTE
		}
		break;

		default:
		// Si PORTD no está en un estado de columna válido, no hay tecla.
		key = ' ';
		break;
	}

	return key;
}

void BARRE_TECLADO(void)
{
	// Avanza la columna activa. Mantener default para iniciar en COL1.
	switch (PORTD) {
		case COL1: PORTD = COL2; break;
		case COL2: PORTD = COL3; break;
		case COL3: PORTD = COL4; break;
		case COL4: PORTD = COL1; break; // puedes usar default también
		default:   PORTD = COL1; break;
	}
}