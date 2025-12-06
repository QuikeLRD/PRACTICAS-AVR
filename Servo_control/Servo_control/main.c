/*
 Simple sweep calibration for continuous-rotation / unknown servo.
 Shows pulse_us on LCD and applies pulses from 1000..2000us.
 Use with caution: monitor Vbat and stop if servo/heats.
*/

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"


#include "Keyboard.h"

#define TOP_VAL 312U        // OCR1A for ~20ms with prescaler=64 @1MHz
#define PERIOD_US 20000UL

char mensaje2[]="Neutral";

static uint16_t pulse_us_to_OCR1B(uint16_t pulse_us)
{
    uint32_t ticks = (uint32_t)pulse_us * (TOP_VAL + 1) / PERIOD_US;
    if (ticks == 0) return 0;
    return (uint16_t)(ticks - 1);
}

void timer1_init_mode15(void)
{
    // PD4 = OC1B output
    DDRD |= (1 << PD4);

    // WGM13..0 = 1111 (mode 15, TOP = OCR1A)
    TCCR1A = (1<<WGM11) | (1<<WGM10);
    TCCR1B = (1<<WGM13) | (1<<WGM12);

    // Non-inverting OC1B (clear on compare)
    TCCR1A |= (1<<COM1B1);
    TCCR1A &= ~(1<<COM1B0);

    // Prescaler = 64
    TCCR1B |= (1<<CS11) | (1<<CS10);

    OCR1A = TOP_VAL; // period
}

int main(void)
{
	


    DDRC = 0xFFU;                                                                  // LCD: ALL OUTPUTS
    DDRD = 0xF0U;                                                                  // KEYPAD: PD0->PD3 INPUT, PD4->PD7 OUTPUT
    PORTD = COL1;

    char buf[16];
    LCD_INICIALIZA();
    LIMPIA_LCD();

    // inicializa Timer1 para PWM modo 15
    timer1_init_mode15();

    // safety neutral initially
    OCR1B = pulse_us_to_OCR1B(1500);
    _delay_ms(1000);

    // Sweep params
    const uint16_t start_us = 1000;
    const uint16_t end_us = 2000;
    const uint16_t step_us = 10;   // incremento
    const uint16_t hold_ms = 200;  // tiempo para observar

    while (1)
    {
        // sweep ascend
        for (uint16_t us = start_us; us <= end_us; us += step_us) {
            OCR1B = pulse_us_to_OCR1B(us);
            LIMPIA_LCD();
            POS_LINEA1(0);
            snprintf(buf, sizeof(buf), "Pulse: %4uus", (unsigned)us);
            ENVIA_CADENA(buf);
            POS_LINEA2(0);
            snprintf(buf, sizeof(buf), "OCR1B=%3u", (unsigned)OCR1B);
            ENVIA_CADENA(buf);
            _delay_ms(hold_ms);
        }

        // neutral pause
        OCR1B = pulse_us_to_OCR1B(1500);
        LIMPIA_LCD();
        POS_LINEA1(0);
        ENVIA_CADENA(mensaje2);
        _delay_ms(1000);

        // sweep descend
        for (uint16_t us = end_us; us >= start_us; us -= step_us) {
            OCR1B = pulse_us_to_OCR1B(us);
            LIMPIA_LCD();
            POS_LINEA1(0);
            snprintf(buf, sizeof(buf), "Pulse: %4uus", (unsigned)us);
            ENVIA_CADENA(buf);
            POS_LINEA2(0);
            snprintf(buf, sizeof(buf), "OCR1B=%3u", (unsigned)OCR1B);
            ENVIA_CADENA(buf);
            _delay_ms(hold_ms);
            if (us <= start_us) break;
        }

        // neutral pause again
        OCR1B = pulse_us_to_OCR1B(1500);
        _delay_ms(1000);
    }
}