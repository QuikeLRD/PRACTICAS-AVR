/* Keyboard.h - Macros y prototipos para teclado matricial 4x4 en PORTA
 *
 * Mapeo esperado:
 *  - Filas  : PA0 .. PA3  (entradas)
 *  - Columnas: PA4 .. PA7  (salidas)
 *
 * Las macros mantienen el comportamiento de barrido por columnas (columna activa = 0 en el bit).
 *
 * Uso:
 *  - En el main: DDRA = 0xF0;   // PA4..PA7 salidas, PA0..PA3 entradas
 *  - Inicializar columna activa: PORTA = COL1;
 *  - LEE_TECLADO() lee PINA & 0x0F y compara con FILx
 *  - BARRE_TECLADO() avanza la columna escribiendo COL1..COL4 en PORTA
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <avr/io.h>

/* Máscaras filas/columnas */
#define ROW_MASK   0x0F    /* PA0..PA3 */
#define COL_MASK   0xF0    /* PA4..PA7 */

/* Valores de lectura de filas (PA0..PA3) cuando se pulsa una fila
   Se asume que las filas tienen pull-ups y la lectura activa es 0 en el bit correspondiente.
   Ej.: FIL1 = 0x0E => PA0 == 0, PA1..PA3 == 1 */
#define FIL1  0x0E   /* 0b00001110 */
#define FIL2  0x0D   /* 0b00001101 */
#define FIL3  0x0B   /* 0b00001011 */
#define FIL4  0x07   /* 0b00000111 */

/* Valores a escribir en PORTA para activar cada columna (column active low).
   Ajusta si tu hardware usa columna activa high. */
#define COL1  0xEF   /* 0b11101111 -> PA4 = 0 (columna 1 activa), PA5..7 = 1 */
#define COL2  0xDF   /* 0b11011111 -> PA5 = 0 */
#define COL3  0xBF   /* 0b10111111 -> PA6 = 0 */
#define COL4  0x7F   /* 0b01111111 -> PA7 = 0 */

/* Prototipos */
char LEE_TECLADO(void);
void BARRE_TECLADO(void);

#endif /* KEYBOARD_H */