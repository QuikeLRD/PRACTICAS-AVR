#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"
#include "Keyboard.h"

/* --------------------------------------------------------------------------
   NOTA (servo de rotación continua):
   - No hay posición absoluta; solo velocidad/sentido.
   - “Ángulo” se simula con tiempo de giro. Debes CALIBRAR:
       SERVO_FWD_US  : qué tan lejos del neutro (1500 us) para girar.
       MS_POR_GRADO  : ms para ~1 grado en tu servo.
   - Coloca manualmente el servo en 0° antes de cada secuencia.
   -------------------------------------------------------------------------- */

// Calibración servo continuo
#define SERVO_STOP_US     1500
#define SERVO_FWD_US      1400
#define MS_POR_GRADO      15

// PWM Timer1 (50 Hz)
#define US_TO_COUNTS(us) ((uint16_t)((us) / 8))  // F_CPU=1 MHz, prescaler=8
#define ICR1_TOP 2499  // 20 ms

static volatile uint8_t flag_reset = 0;   // INT2 flag
char buf[16] = {0};
char tecla   = ' ';

// Mensajes
char msg_intro1[]   = "Angulo 0-270";
char msg_intro2[]   = "D=Enter INT2=Clr";
char msg_reset1[]   = "Reset. Ingresa";
char msg_reset2[]   = "angulo y D";
char msg_ready1[]   = "Listo. INT2=0";
char msg_ready2[]   = "Ingresa nuevo";
char msg_sel1[]     = "Angulo sel:";

ISR(INT2_vect) {
    flag_reset = 1;
    GICR &= ~(1 << INT2);   // deshabilita INT2 hasta que el main la re-habilite
}

static inline void clearArray(char *a, uint8_t n) { memset(a, 0, n); }

int main(void) {
    // LCD en PORTC
    DDRC = 0xFF;

    // Teclado: PD0..PD3 entradas, PD4..PD7 salidas
    DDRD = 0xF0;
    PORTD = COL1;
    PORTD |= 0x0F;   // pull-ups para PD0..PD3 (filas)

    // PWM servo en OC1A (PD5)
    DDRD |= (1 << PD5);

    // INT2 en PB2 con pull-up
    DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2);

    // Timer1 Fast PWM, TOP=ICR1 (modo 14)
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1A |= (1 << COM1A1);   // no invertido en OC1A
    TCCR1B |= (1 << CS11);     // prescaler 8
    ICR1 = ICR1_TOP;

    // INT2 flanco de subida
    MCUCSR |= (1 << ISC2);
    GIFR   |= (1 << INTF2);
    GICR   |= (1 << INT2);

    LCD_INICIALIZA();
    LIMPIA_LCD();
    POS_LINEA1(0); ENVIA_CADENA(msg_intro1);
    POS_LINEA2(0); ENVIA_CADENA(msg_intro2);
    _delay_ms(800);
    LIMPIA_LCD();

    sei();

    while (1) {
        // Barre antes de leer
        BARRE_TECLADO();
        tecla = LEE_TECLADO();

        // Entrada de dígitos
        if ((tecla >= '0' && tecla <= '9')) {
            char tmp[2] = {tecla, 0};
            strcat(buf, tmp);
            LIMPIA_LCD();
            ENVIA_CADENA(buf);   // muestra lo que llevas escrito
            _delay_ms(150);
        }
        // Enter con 'D'
        else if (tecla == 'D') {
            int ang = atoi(buf);
            if (ang < 0) ang = 0;
            if (ang > 270) ang = 270;

            // Muestra ángulo confirmado
            char msg[16];
            LIMPIA_LCD();
            POS_LINEA1(0); ENVIA_CADENA(msg_sel1);
            POS_LINEA2(0);
            snprintf(msg, sizeof(msg), "%3d deg", ang);
            ENVIA_CADENA(msg);

            // Calcula tiempo de giro simulado
            uint16_t tiempo_ms = (uint16_t)(ang * MS_POR_GRADO);

            // Gira en un sentido (solo positivo 0..270)
            OCR1A = US_TO_COUNTS(SERVO_FWD_US);

            // _delay_ms exige constante ? bucle de 1 ms
            for (uint16_t i = 0; i < tiempo_ms; i++) {
                _delay_ms(1);
            }

            // Detiene
            OCR1A = US_TO_COUNTS(SERVO_STOP_US);

            // Espera a que el usuario pulse INT2 para “regresar a 0” lógico
            POS_LINEA1(0); ENVIA_CADENA(msg_ready1);
            POS_LINEA2(0); ENVIA_CADENA(msg_ready2);
            while (!flag_reset) {
                OCR1A = US_TO_COUNTS(SERVO_STOP_US);
            }

            // Limpia para nueva entrada
            clearArray(buf, sizeof(buf));
            flag_reset = 0;
            GICR |= (1 << INT2);  // re-habilita INT2
            LIMPIA_LCD();
        }

        // Si INT2 llegó en cualquier momento, limpia y detiene
        if (flag_reset) {
            OCR1A = US_TO_COUNTS(SERVO_STOP_US);
            clearArray(buf, sizeof(buf));
            flag_reset = 0;
            GICR |= (1 << INT2);
            LIMPIA_LCD();
            POS_LINEA1(0); ENVIA_CADENA(msg_reset1);
            POS_LINEA2(0); ENVIA_CADENA(msg_reset2);
            _delay_ms(300);
        }

        _delay_ms(3);
        tecla = ' ';
    }
    return 0;
}