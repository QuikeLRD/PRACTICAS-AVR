// PRACTICA 4 PUNTO 2
// LERDO CRISOSTOMO LUIS ENRIQUE


#define CPU_F 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

//CONFIGURACION GENERAL
#define PATTERN_CYCLES 4U
#define BTN_MASK_PC  ((1U<<PC0)|(1U<<PC1))    // Solo PC0, PC1
#define BTN_MASK_PB  (1U<<PB5)                // PB5 usa PCINT (D13)

typedef enum {
	MODE_BIN = 0U,                   //MODO PRINCIPAL CUENTA DEL 0 AL 255
	MODE_MARQUEE,
	MODE_PINGPONG,
	MODE_ALTERNATE,
	MODE_FILL,
	MODE_RANDOM
} mode_t;

//BANDERAS DE LA INTERRUPCION
static volatile uint8_t g_btn_flags = 0U;    //BIT0..BIT4 => PC0, PC1, PB5, INT0, INT1
static volatile uint8_t g_prev_pinc = 0xFFU;
static volatile uint8_t g_prev_pinb = 0xFFU;
static volatile mode_t g_mode = MODE_BIN;

// Helper: escribe LEDs en PORTB preservando PB5 en '1' (pull-up activado)
static inline void LED_WRITE(uint8_t v) {
	PORTB = (uint8_t)((v & (uint8_t)~BTN_MASK_PB) | BTN_MASK_PB);
}

//ABORTAR
static inline uint8_t abort_requested(void) {
	return (g_btn_flags != 0U);
}

static inline void re_enable_pcint_for_index(uint8_t idx) {
	if (idx < 2U) { // PCINT1 (PC0, PC1)
		const uint8_t pcint_bit = (uint8_t)(PCINT8 + idx);  // PC0..PC1 -> PCINT8..PCINT9
		PCMSK1 |= (uint8_t)(1U << pcint_bit);
		} else if (idx == 2U) { // PCINT0 (PB5)
		PCMSK0 |= (1U << PCINT5);
	}
}

static void delay_abort_ms(uint16_t ms) {
	for (uint16_t t = 0U; t < ms; t++) {
		if (abort_requested()) { return; }
		_delay_ms(1);
	}
}

// --- VECTORES DE INTERRUPCION ---
// PB5 -> PCINT0
ISR(PCINT0_vect)
{
	const uint8_t curr    = PINB;
	const uint8_t changed = (uint8_t)((g_prev_pinb ^ curr) & BTN_MASK_PB);
	const uint8_t falling = (uint8_t)(changed & (uint8_t)(~curr)); // flanco 1->0

	if (falling & (1U<<PB5)) { g_btn_flags |= (1U<<2); PCMSK0 &= (uint8_t)~(1U<<PCINT5); }

	g_prev_pinb = curr;
}

// PC0-PC1 -> PCINT1
ISR(PCINT1_vect)
{
	const uint8_t curr    = PINC;
	const uint8_t changed = (uint8_t)((g_prev_pinc ^ curr) & BTN_MASK_PC);
	const uint8_t falling = (uint8_t)(changed & (uint8_t)(~curr)); // flanco 1->0

	if (falling & (1U<<PC0)) { g_btn_flags |= (1U<<0); PCMSK1 &= (uint8_t)~(1U<<PCINT8);  }
	if (falling & (1U<<PC1)) { g_btn_flags |= (1U<<1); PCMSK1 &= (uint8_t)~(1U<<PCINT9);  }

	g_prev_pinc = curr;
}

// INT0 (PD2) - activa MODE_FILL (bit 3 en g_btn_flags)
ISR(INT0_vect) {
	g_btn_flags |= (1U << 3);
	EIMSK &= (uint8_t)~(1U << INT0);
}

// INT1 (PD3) - activa MODE_RANDOM (bit 4 en g_btn_flags)
ISR(INT1_vect) {
	g_btn_flags |= (1U << 4);
	EIMSK &= (uint8_t)~(1U << INT1);
}

// --- PATRONES DE LUCES SOLO EN PORTB ---
static void pattern_marquee(void)
{
	for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++) {
		LED_WRITE(0x55); delay_abort_ms(120); if (abort_requested()) return;
		LED_WRITE(0xAA); delay_abort_ms(120); if (abort_requested()) return;
	}
}

static void pattern_pingpong(void)
{
	for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++) {
		// Dos LEDs viajando juntos LSB -> MSB
		uint8_t v = 0x03U; // 0b00000011
		for (uint8_t i = 0; i < 7 && !abort_requested(); i++) {
			LED_WRITE(v);
			delay_abort_ms(80);
			v <<= 1;
		}
		if (abort_requested()) return;
		// Dos LEDs viajando juntos MSB -> LSB
		v = 0xC0U; // 0b11000000
		for (uint8_t i = 0; i < 7 && !abort_requested(); i++) {
			LED_WRITE(v);
			delay_abort_ms(80);
			v >>= 1;
		}
	}
}

static void pattern_alternate(void)
{
	for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++) {
		LED_WRITE(0x33); delay_abort_ms(150); if (abort_requested()) return;
		LED_WRITE(0xCC); delay_abort_ms(150); if (abort_requested()) return;
	}
}

static void pattern_fill(void)
{
	for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++) {
		uint8_t mask = 0x00U;
		for (uint8_t i = 0U; i < 8U && !abort_requested(); i++) {
			mask = (uint8_t)((mask << 1) | 0x01U);
			LED_WRITE(mask);
			delay_abort_ms(100);
		}
		LED_WRITE(0x00); delay_abort_ms(80); if (abort_requested()) return;
	}
}

static void pattern_random(void)
{
	for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++)
	{
		// AVANZAMOS PORTB DE 0 A 7
		for (uint8_t i = 0U; i < 8U && !abort_requested(); i++)
		{
			LED_WRITE((uint8_t)(1U << i));
			delay_abort_ms(80);
		}
		if (abort_requested()) return;

		// REGRESAMOS DE PORTB (7 -> 0)
		for (int8_t i = 7; i >= 0 && !abort_requested(); i--)
		{
			LED_WRITE((uint8_t)(1U << i));
			delay_abort_ms(80);
		}
		if (abort_requested()) return;
	}
}

//EJECTUA EL PATRON SOLICITADO Y DEVUELVE 1 SI FUE ABORTADO, 0 SI TERMINO LOS 4 CICLOS
static uint8_t run_pattern(mode_t m)
{
	switch (m) {
		case MODE_MARQUEE:   pattern_marquee();   break;
		case MODE_PINGPONG:  pattern_pingpong();  break;
		case MODE_ALTERNATE: pattern_alternate(); break;
		case MODE_FILL:      pattern_fill();      break;
		case MODE_RANDOM:    pattern_random();    break;
		default: break;
	}
	return abort_requested() ? 1U : 0U;
}

// ----------------------------- MAIN-----------------------------
int main(void)
{
	// Salidas: todos PB como salida excepto PB5 (botón)
	DDRB  = (uint8_t)(0xFFU & (uint8_t)~BTN_MASK_PB); // => PB5 entrada, resto salida
	PORTB = 0x00U;
	// Pull-up en PB5
	PORTB |= BTN_MASK_PB;

	// PC0 y PC1 como entradas con pull-up
	DDRC  &= (uint8_t)~BTN_MASK_PC;
	PORTC |= BTN_MASK_PC;

	// INT0 (PD2) e INT1 (PD3) como entradas con pull-up
	DDRD  &= (uint8_t)~((1U << PD2) | (1U << PD3));
	PORTD |= (1U << PD2) | (1U << PD3);

	// PCINT en C (grupo 1) y en B (grupo 0)
	PCICR  |= (1U << PCIE1) | (1U << PCIE0);
	PCMSK1 |= (1U << PCINT8) | (1U << PCINT9); // PC0, PC1
	PCMSK0 |= (1U << PCINT5);                  // PB5

	// Captura estados iniciales y limpia flags
	g_prev_pinc = PINC;
	g_prev_pinb = PINB;
	PCIFR |= (1U << PCIF1) | (1U << PCIF0);

	// INT0/INT1 flanco de bajada
	EICRA |=  (1U << ISC01);
	EICRA &= ~(1U << ISC00);
	EICRA |=  (1U << ISC11);
	EICRA &= ~(1U << ISC10);

	EIFR  |= (1U << INTF0) | (1U << INTF1);
	EIMSK |= (1U << INT0)  | (1U << INT1);

	sei();

	uint8_t count = 0U;

	for (;;)
	{
		if (g_btn_flags != 0U) {
			const uint8_t f = g_btn_flags;
			g_btn_flags = 0U;

			mode_t req = MODE_MARQUEE;
			uint8_t idx = 0U;

			if      (f & (1U<<0)) { req = MODE_MARQUEE;  idx = 0U; }
			else if (f & (1U<<1)) { req = MODE_PINGPONG; idx = 1U; }
			else if (f & (1U<<2)) { req = MODE_ALTERNATE;idx = 2U; }
			else if (f & (1U<<3)) { req = MODE_FILL;     idx = 3U; }
			else if (f & (1U<<4)) { req = MODE_RANDOM;   idx = 4U; }

			_delay_ms(25);
			if (idx < 2U) {
				PCIFR |= (1U << PCIF1);
				re_enable_pcint_for_index(idx);
				} else if (idx == 2U) {
				PCIFR |= (1U << PCIF0);
				re_enable_pcint_for_index(idx);
				} else if (idx == 3U) {
				EIFR |= (1U << INTF0);
				EIMSK |= (1U << INT0);
				} else if (idx == 4U) {
				EIFR |= (1U << INTF1);
				EIMSK |= (1U << INT1);
			}

			g_mode = req;

			uint8_t aborted = run_pattern(req);

			if (!aborted) {
				LED_WRITE(0x00U);
				count = 0U;
				g_mode = MODE_BIN;
			}
			continue;
		}

		g_mode = MODE_BIN;
		LED_WRITE(count);
		_delay_ms(500);
		count++;
	}
}