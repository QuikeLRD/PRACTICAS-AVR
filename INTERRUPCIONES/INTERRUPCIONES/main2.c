#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"

// Texto a desplazar

char mensaje2[] = "PRIMER";
char mensaje3[] = "INTERRUPCION";

char mensaje[] = "Luis Enrique Lerdo Crisostomo";

// Bandera de evento desde la ISR (volatile)
static volatile uint8_t g_alert = 0U;

// Longitud segura (hasta 255)
static uint8_t str_len(const char *s)
{
	uint8_t n = 0U;
	while (s[n] != '\0') { n++; }
	return n;
}

// Carga el nombre en todas las 40 columnas de la línea 1 (visibles + ocultas).
// Deja espacios donde no haya texto.
static void lcd_prepare_scroll_name(void)
{
	char line1[40];
	for (uint8_t i = 0U; i < 40U; i++) {
		line1[i] = ' ';
	}

	const uint8_t len = str_len(mensaje);
	const uint8_t to_copy = (len < 40U) ? len : 40U;
	for (uint8_t i = 0U; i < to_copy; i++) {
		line1[i] = mensaje[i];
	}

	// Escribe visibles 0..15
	POS_LINEA1(0);
	for (uint8_t i = 0U; i < 16U; i++) {
		ENVIA_DATO(line1[i]);
	}
	// Escribe ocultas 16..39
	POS_LINEA1(16);
	for (uint8_t i = 16U; i < 40U; i++) {
		ENVIA_DATO(line1[i]);
	}

	// Alinea el display (offset de shift = 0)
	REGRESO_CASA();
}

ISR(INT0_vect)
{
	// Marca evento y deshabilita INT0 para antirrebote
	g_alert = 1U;
	GICR &= (uint8_t)~(1U << INT0);
	// La bandera INTF0 se limpia al entrar a la ISR; si quisieras limpiar manual, sería:
	// GIFR |= (1U << INTF0);
}

int main(void)
{
	// LCD en PORTC
	DDRC = 0xFFU;

	// INT0 (PD2) como entrada con pull-up
	DDRD &= (uint8_t)~(1U << PD2);
	PORTD |= (1U << PD2);

	// Modo de disparo INT0: flanco de bajada (ISC01=1, ISC00=0)
	MCUCR |=  (1U << ISC01);
	MCUCR &= (uint8_t)~(1U << ISC00);

	// Limpia bandera previa (por seguridad)
	GIFR |= (1U << INTF0);

	// Habilita INT0 y globales
	GICR |= (1U << INT0);
	sei();

	// Inicializa LCD
	LCD_INICIALIZA();
	LIMPIA_LCD();

	// Prepara el scroll del nombre (carga 40 columnas)
	lcd_prepare_scroll_name();

	uint8_t shifts = 0U;

	for (;;)
	{
		// Si hay evento de interrupción, muestra el mensaje estático y luego regresa al scroll
		if (g_alert == 1U)
		{
			g_alert = 0U;

			// Muestra “PRIMER / INTERRUPCION” sin desplazamiento
			LIMPIA_LCD();
			POS_LINEA1(0);
			ENVIA_CADENA(mensaje2);
			POS_LINEA2(0);
			ENVIA_CADENA(mensaje3);

			// Tiempo que quieres mostrar el mensaje (ajusta)
			_delay_ms(2000);

			// Restaura el nombre para desplazamiento
			LIMPIA_LCD();
			lcd_prepare_scroll_name();
			shifts = 0U;        // reinicia el conteo de desplazamientos

			// Limpia bandera de INT0 (por si quedó) y vuelve a habilitar INT0 (fin del antirrebote)
			GIFR |= (1U << INTF0);
			GICR |= (1U << INT0);

			// Continúa al siguiente ciclo sin desplazar en este tick
			continue;
		}

		// Modo normal: desplazamiento del nombre
		_delay_ms(100);               // velocidad de scroll
		DESPLAZAMIENTO_PIZQUIERDA();  // Debe enviar 0x18
		shifts++;
		if (shifts >= 40U)
		{
			REGRESO_CASA();           // resetea el offset de shift
			shifts = 0U;
		}
	}
}