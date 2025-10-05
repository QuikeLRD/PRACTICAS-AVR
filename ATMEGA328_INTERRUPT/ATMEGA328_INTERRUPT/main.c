// PRACTICA 4 PUNTO 2
// LERDO CRISOSTOMO LUIS ENRIQUE

#define CPU_F 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

//CONFIGURACION GENERAL
#define PATTERN_CYCLES 4U																//LIMITA CADA PATRON A 4 CICLOS
#define BTN_MASK_PC  ((1U<<PC0)|(1U<<PC1)|(1U<<PC2)|(1U<<PC3)|(1U<<PC4))				

typedef enum {
    MODE_BIN = 0U,																		//MODO PRINCIPAL CUENTA DEL 0 AL 255
    MODE_MARQUEE,
    MODE_PINGPONG,
    MODE_ALTERNATE,
    MODE_FILL,
    MODE_RANDOM
} mode_t;

//BANDERAS DE LA INTERRUPCION
static volatile uint8_t g_btn_flags = 0U;												//BIT0 ...BIT4 => PC0..PC4
static volatile uint8_t g_prev_pinc = 0xFFU;
//ESTADO DEL ESTADO ACTUAL
static volatile mode_t g_mode = MODE_BIN;

//ABORTAR 
static inline uint8_t abort_requested(void) {
    return (g_btn_flags != 0U);
}

static inline void re_enable_pcint_for_index(uint8_t idx) {
    const uint8_t pcint_bit = (uint8_t)(PCINT8 + idx);									// PC0..PC4 -> PCINT8..PCINT12
    PCMSK1 |= (uint8_t)(1U << pcint_bit);
}

//DELAY INTERRUMPIBLE SI LLEGA OTRO BOTON
static void delay_abort_ms(uint16_t ms) {
    for (uint16_t t = 0U; t < ms; t++) {
        if (abort_requested()) { return; }
        _delay_ms(1);
    }
}

//VECTOR DE INTERRUPCION ISR(PCINT1)
ISR(PCINT1_vect)
{
    const uint8_t curr    = PINC;
    const uint8_t changed = (uint8_t)((g_prev_pinc ^ curr) & BTN_MASK_PC);
    const uint8_t falling = (uint8_t)(changed & (uint8_t)(~curr)); // flanco 1->0

    if (falling & (1U<<PC0)) { g_btn_flags |= (1U<<0); PCMSK1 &= (uint8_t)~(1U<<PCINT8);  }
    if (falling & (1U<<PC1)) { g_btn_flags |= (1U<<1); PCMSK1 &= (uint8_t)~(1U<<PCINT9);  }
    if (falling & (1U<<PC2)) { g_btn_flags |= (1U<<2); PCMSK1 &= (uint8_t)~(1U<<PCINT10); }
    if (falling & (1U<<PC3)) { g_btn_flags |= (1U<<3); PCMSK1 &= (uint8_t)~(1U<<PCINT11); }
    if (falling & (1U<<PC4)) { g_btn_flags |= (1U<<4); PCMSK1 &= (uint8_t)~(1U<<PCINT12); }

    g_prev_pinc = curr;
}

//	PATRONES DE LUCES


static void pattern_marquee(void)
{
    for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++) {
        PORTB = 0x55; PORTD = 0x55; delay_abort_ms(120); if (abort_requested()) return;
        PORTB = 0xAA; PORTD = 0xAA; delay_abort_ms(120); if (abort_requested()) return;
    }
}

static void pattern_pingpong(void)
{
    for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++) {
        //BARRIDO LSB -> MSB
        uint8_t v = 0x01U;
        while (!abort_requested()) {
            PORTB = 0x00; PORTD = v; delay_abort_ms(80); if (abort_requested()) return;
            if (v == 0x80U) break;
            v <<= 1;
        }
        //BARRIDO MSB -> LSB
        while (!abort_requested()) {
            PORTB = 0x00; PORTD = v; delay_abort_ms(80); if (abort_requested()) return;
            if (v == 0x01U) break;
            v >>= 1;
        }
    }
}

static void pattern_alternate(void)
{
    for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++) {
        PORTB = 0x33; PORTD = 0xCC; delay_abort_ms(150); if (abort_requested()) return;
        PORTB = 0xCC; PORTD = 0x33; delay_abort_ms(150); if (abort_requested()) return;
    }
}

static void pattern_fill(void)
{
    for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++) {
        uint8_t mask = 0x00U;
        for (uint8_t i = 0U; i < 8U && !abort_requested(); i++) {
            mask = (uint8_t)((mask << 1) | 0x01U);
            PORTB = mask; PORTD = mask; delay_abort_ms(100);
        }
        // LIMPIEZA ENTRE CICLOS
        PORTB = 0x00; PORTD = 0x00; delay_abort_ms(80); if (abort_requested()) return;
    }
}

static void pattern_random(void)
{
	// REPITE EL PATRON COMPLETO 4 VECES O EL VALOR DE PATTERN CYCLES
	for (uint8_t c = 0U; c < PATTERN_CYCLES && !abort_requested(); c++)
	{
		// AVANZAMOS PORTB DE 0 A 7
		for (uint8_t i = 0U; i < 8U && !abort_requested(); i++)
		{
			PORTD = 0x00;                                 // PORTA -> PORTD (ATmega328P)
			PORTB = (uint8_t)(1U << i);
			delay_abort_ms(80);
		}
		if (abort_requested()) return;

		// AVANZAMOS PORTD DE 0 A 7
		for (uint8_t i = 0U; i < 8U && !abort_requested(); i++)
		{
			PORTB = 0x00;
			PORTD = (uint8_t)(1U << i);
			delay_abort_ms(80);
		}
		if (abort_requested()) return;

		// REGRESAMOS DE PORTD (7 -> 0)
		for (int8_t i = 7; i >= 0 && !abort_requested(); i--)
		{
			PORTB = 0x00;
			PORTD = (uint8_t)(1U << i);
			delay_abort_ms(80);
		}
		if (abort_requested()) return;

		// REGRESAMOS DE PORTB (7 -> 0)
		for (int8_t i = 7; i >= 0 && !abort_requested(); i--)
		{
			PORTD = 0x00;
			PORTB = (uint8_t)(1U << i);
			delay_abort_ms(80);
		}
		
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
    //CONFIGURACION SALIDAS PORTD AND PORTD
    DDRB = 0xFFU;
    DDRD = 0xFFU;
    PORTB = 0x00U;
    PORTD = 0x00U;

    //ENTRADAS PC0 - PC4
    DDRC  &= (uint8_t)~BTN_MASK_PC;
    PORTC |= BTN_MASK_PC;

    //CONFIGURACION DE INTERRUPCION
    PCICR  |= (1U << PCIE1);
    PCMSK1 |= (1U << PCINT8) | (1U << PCINT9) | (1U << PCINT10) | (1U << PCINT11) | (1U << PCINT12);
    g_prev_pinc = PINC;
    PCIFR |= (1U << PCIF1);

    sei();

    uint8_t count = 0U;  //CONTADOR DE CUENTA BINARIA

    for (;;)
    {
        //SE PRESIONO EL BOTON
        if (g_btn_flags != 0U) {
            const uint8_t f = g_btn_flags;									// CAPTURA 8 BITS
            g_btn_flags = 0U;

            mode_t req = MODE_MARQUEE;
            uint8_t idx = 0U;

            if      (f & (1U<<0)) { req = MODE_MARQUEE;  idx = 0U; }
            else if (f & (1U<<1)) { req = MODE_PINGPONG; idx = 1U; }
            else if (f & (1U<<2)) { req = MODE_ALTERNATE;idx = 2U; }
            else if (f & (1U<<3)) { req = MODE_FILL;     idx = 3U; }
            else if (f & (1U<<4)) { req = MODE_RANDOM;   idx = 4U; }

            _delay_ms(25);													// BREVE ANTIREBOTE
            PCIFR |= (1U << PCIF1);
            re_enable_pcint_for_index(idx);

            g_mode = req;													//INFORMATIVO

            //EJECTUA EL PATRON LIMITADO A 4 CICLOS
            uint8_t aborted = run_pattern(req);

            if (!aborted) {
                //TERMINO LOS 4 CICLOS ASI QUE LIMPIA Y REGRESA AL PROGRAMA PRINCIPAL
                PORTB = 0x00U;
                PORTD = 0x00U;
                count = 0U;
                g_mode = MODE_BIN;
            }
            /* Si fue abortado, simplemente vuelve al lazo:
               el próximo botón se atenderá o, si no hay, sigue la cuenta */
            continue;
        }

        //PROGRAMA PRINCIPAL CUENTA BINARIA
        g_mode = MODE_BIN;
        PORTB = 0x00U;          // PORTB APAGADO DURANTE LA CUENTA
        PORTD = count;
        _delay_ms(500);
        count++;                // VUELVE A 0
    }
}