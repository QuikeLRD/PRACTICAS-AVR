#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "LCD.h"

/* Cronómetro: overflow cada ?10.11 ms (preload 178 o 177 según calibración)
   Se cuentan 100 overflows ? 1.01 s -> seg++ y reinicio de contador de sub-ovf */

volatile uint8_t seg = 0;        // sub-contador de overflows (0..99)
volatile uint8_t seconds = 0;    // 0..59
volatile uint8_t mins = 0;       // 0..59
volatile uint8_t hours = 0;      // 0..23 (si quieres usar más, cambia tipo)

char titulo[] = "CRONOMETRO";
 char mensaje[] = "00:00:00";

ISR (TIMER0_OVF_vect)
{
    // cada overflow ? 10.1 ms (con preload 178 y prescaler 1024 @8MHz)
    seg++;
    if (seg >= 100) {           // ? 100 * 10.1 ms ? 1.01 s (ajusta preload si quieres exactamente 1.000s)
        seg = 0;
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            mins++;
            if (mins >= 60) {
                mins = 0;
                hours++;
                if (hours >= 24) hours = 0; // opcional rollover
            }
        }
    }
    TCNT0 = 178;  // precarga para repetir el mismo intervalo (ajusta a 177 o 178 según quieras)
}

int main(void)
{
    // GPIO
    DDRB |= (1 << PB5);     // PB5 como salida (LED opcional)
    PORTB &= ~(1 << PB5);

    // LCD
    DDRC = 0xFF;
    LCD_INICIALIZA();
    LIMPIA_LCD();
    POS_LINEA1(0);
    ENVIA_CADENA(titulo);
    _delay_ms(600);
    LIMPIA_LCD();

    // Timer0: preload y configuracion
    TCNT0 = 178;                       // precarga inicial
    TIFR  |= (1 << TOV0);              // limpia bandera si había
    TIMSK |= (1 << TOIE0);             // habilita overflow interrupt

    // Arranca timer con prescaler /1024 -> CS02=1, CS01=0, CS00=1
    TCCR0 = (1 << CS02) | (1 << CS00);

    sei(); // habilita interrupciones globales

    char timebuf[9] = "00:00:00";      // HH:MM:SS
    char lastbuf[9] = "";              // para evitar reescrituras si no cambia
    uint8_t s, m, h;

    // Mostrar inicial
    POS_LINEA1(0);
    ENVIA_CADENA(mensaje);

    while (1) {
        // Lectura atómica de variables compartidas por ISR
        uint8_t oldSREG = SREG;
        cli();
        s = seconds;
        m = mins;
        h = hours;
        SREG = oldSREG; // restaura bandera de interrupciones (equivale a sei())

        // Formatear tiempo con ceros a la izquierda
        snprintf(timebuf, sizeof(timebuf), "%02u:%02u:%02u", (unsigned)h, (unsigned)m, (unsigned)s);

        // Actualizar LCD solo si cambió (evita parpadeo)
        if (strncmp(timebuf, lastbuf, sizeof(timebuf)) != 0) {
            POS_LINEA1(0);
            ENVIA_CADENA(timebuf);
            memcpy(lastbuf, timebuf, sizeof(timebuf));
        }

        
    }

    return 0;
}