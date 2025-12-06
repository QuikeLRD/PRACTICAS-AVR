/*
======================================================================================
PROJECT:       SERVO POSITION CONTROL VIA KEYPAD (0..270°)
COMPANY:       UPIITA-IPN
FILE:          SERVO_KEYPAD_CONTROL.C
AUTHOR:        LUIS ENRIQUE LERDO CRISOSTOMO (adapted)
DATE:          [FECHA]

DESCRIPTION:
-------------
PROGRAMA BASADO EN LA ESTRUCTURA DE MOTORCONTROL_PWM.C PARA CONTROLAR UN SERVOMOTOR
MEDIANTE TECLADO MATRIZ (conectado a PORTA) Y MOSTRAR INFORMACIÓN EN LCD. EL USUARIO
INGRESA UN ÁNGULO EN GRADOS (0..270) Y AL PRESIONAR '#' SE MUESTRA EL ÁNGULO Y EL SERVO ES POSICIONADO.
INT2 (PB2) SE USA PARA REGRESAR EL SERVO A 0°.

HARDWARE / NOTES:
- Keypad conectado a PORTA: PA0..PA3 filas (entrada), PA4..PA7 columnas (salida).
- LCD y Keyboard libs: "LCD.h" y "Keyboard.h"
- Timer1 PWM (modo 15, TOP = OCR1A) ? OC1B en PD4 es la señal del servo.
- Si el servo es continuous-rotation no posicionará con precisión (se necesita feedback).
======================================================================================
*/

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom16.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"
#include "Keyboard.h"

// ======================== INTERRUPT AND STATE FLAGS ===============================
static volatile uint8_t flag = 0U;       // INT2 (PB2) trigger flag
volatile uint8_t sign;                   // input state (0 = idle, 1 = angle entered)

// ========================== LCD MESSAGE BUFFERS ===================================
char mensaje1[]  = "CONTROL DE UN";
char mensaje2[]  = "   SERVO 360";
char mensaje3[]  = "ANGULO (0..270):";
char mensaje4[]  = "REGRESANDO A 0";
char mensajeM[]  = "MOVIENDO A";
char mensajeA[] = "ANGULO INGRESADO:";
char cero[] = "0 deg";
// ======================= KEYPAD AND DISPLAY BUFFERS ===============================
char BUF[16];                           // temporary buffer for single char
char PRO[16];                           // concatenated digits while typing
char ANGULO_STR[16];                    // final entered angle string
char SHOW_ANGLE[16];                    // formatted display "ANG: xxx"4


// ================================ GLOBAL VARIABLES ================================
uint16_t angle_val = 0;                 // angle in degrees (0..270)
char tecla;                             // key read from keypad

// ========================= SERVO / PWM PARAMETERS ================================
#define TOP_VAL          312U         // OCR1A -> ~20 ms period (F_CPU=1MHz, presc=64)
#define PERIOD_US        20000UL
#define PULSE_MIN_US     1000U        // 0 degrees
#define PULSE_MAX_US     2000U        // 270 degrees

// ================== UTILITY: CLEARS SPECIFIED CHAR ARRAY ==========================
void clearArray(char* myArray, short sizeOfMyArray) {
    memset(myArray, 0, sizeof(char) * sizeOfMyArray);
}

// ======================== EXTERNAL INTERRUPT: INT2 (PB2) ==========================
ISR(INT2_vect) {
    flag = 1;                                      // request return-to-zero
    GICR &= (uint8_t)~(1 << INT2);                 // disable INT2 until processed
}

// ------------------ PWM / HELPER FUNCTIONS ---------------------------------------
/* Convert pulse width in microseconds to OCR1B value for mode 15 (TOP = OCR1A).
   ticks = pulse_us * (OCR1A + 1) / PERIOD_US
   OCR = ticks - 1  (if ticks>0) */
static uint16_t pulse_us_to_OCR1B(uint16_t pulse_us)
{
    uint32_t ticks = (uint32_t)pulse_us * (uint32_t)(TOP_VAL + 1) / PERIOD_US;
    if (ticks == 0) return 0;
    return (uint16_t)(ticks - 1);
}

/* Map angle (0..270) to pulse microseconds (PULSE_MIN_US..PULSE_MAX_US) */
static uint16_t angle_to_pulse_us(uint16_t angle)
{
    if (angle > 270) angle = 270;
    uint32_t span = (uint32_t)PULSE_MAX_US - (uint32_t)PULSE_MIN_US;
    uint32_t p = (uint32_t)PULSE_MIN_US + ( (uint32_t)angle * span ) / 270U;
    return (uint16_t)p;
}

/* Initialize Timer1 mode 15 (WGM13..0 = 1111), non-inverting OC1B (PD4), prescaler 64 */
static void timer1_servo_init(void)
{
    DDRD |= (1 << PD4); // PD4 = OC1B output

    // Set WGM13..0 = 1111 (mode 15, TOP = OCR1A)
    TCCR1A = (1 << WGM11) | (1 << WGM10);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    // Non-inverting on OC1B
    TCCR1A |= (1 << COM1B1);
    TCCR1A &= ~(1 << COM1B0);

    // Prescaler = 64 (CS11=1, CS10=1)
    TCCR1B |= (1 << CS11) | (1 << CS10);

    OCR1A = TOP_VAL; // set period for 20 ms
}

// ================================ MAIN ============================================
int main(void) {
    // ============================ HARDWARE INITIALIZATION =========================
    // LCD
    DDRC = 0xFFU;                // LCD: all outputs

    // Keypad on PORTA: PA0..PA3 inputs (rows), PA4..PA7 outputs (cols)
    DDRA = 0xF0U;                // PA4..PA7 outputs, PA0..PA3 inputs
    PORTA = COL1;                // initialize column 1 active and enable pull-ups on rows

    // Keep compatibility with original structure (some pins reserved)
    DDRB  |=  (1 << PB3);       // optional output (not used)
    DDRD  |=  (1 << PD5);       // optional PD5 output (unused)
    PORTB &= ~(1 << PB3);
    PORTD &= ~(1 << PD5);

    // INT2 pin (PB2)
    DDRB  &= ~(1 << PB2);       // PB2 as input (INT2)
    PORTB |=  (1 << PB2);       // enable pull-up on PB2

    // Initialize Timer1 for servo PWM
    timer1_servo_init();

    // Ensure neutral (0°) at start
    uint16_t init_pulse = angle_to_pulse_us(0);
    OCR1B = pulse_us_to_OCR1B(init_pulse);

    // INT2: rising edge trigger (same as reference code)
    MCUCSR |=  (1 << ISC2);
    GIFR   |=  (1 << INTF2);
    GICR   |=  (1 << INT2);

    // LCD welcome
    LCD_INICIALIZA();
    LIMPIA_LCD();
    POS_LINEA1(0);
    ENVIA_CADENA(mensaje1);
    POS_LINEA2(0);
    ENVIA_CADENA(mensaje2);
    _delay_ms(800);
    LIMPIA_LCD();

    sei(); // enable global interrupts

    // ensure buffers are empty
    clearArray(PRO, sizeof(PRO));
    clearArray(BUF, sizeof(BUF));
    clearArray(ANGULO_STR, sizeof(ANGULO_STR));
    clearArray(SHOW_ANGLE, sizeof(SHOW_ANGLE));
    sign = 0;

    // ======================== MAIN APPLICATION CYCLE ==============================
    while (1) {
        tecla = LEE_TECLADO();

        // Accept only numeric keys 0..9 while typing angle (ignore A,B,C,D,#,* in this stage)
        if ((tecla >= '0' && tecla <= '9') && (sign != 2)) {
            // append digit (avoid overflow)
            if (strlen(PRO) < (sizeof(PRO)-1)) {
                sprintf(BUF, "%c", tecla);
                strcat(PRO, BUF);
            }
            LIMPIA_LCD();
            POS_LINEA1(0);
            ENVIA_CADENA(mensaje3);
            POS_LINEA2(0);
            ENVIA_CADENA(PRO);    // show digits as typed
            _delay_ms(200);

        } else if (tecla == '#') {   // finalize entry
            // move PRO -> ANGULO_STR
            if (strlen(ANGULO_STR) == 0) {
                strcat(ANGULO_STR, PRO);
            } else {
                clearArray(ANGULO_STR, sizeof(ANGULO_STR));
                strcat(ANGULO_STR, PRO);
            }
            clearArray(PRO, sizeof(PRO));
            clearArray(BUF, sizeof(BUF));
            LIMPIA_LCD();
            _delay_ms(100);

            // Convert and validate angle
            angle_val = (uint16_t)atoi(ANGULO_STR);
            if (angle_val > 270) angle_val = 270; // clamp

            // Display chosen angle and prepare to update PWM
            POS_LINEA1(0);
            ENVIA_CADENA(mensajeA);
            POS_LINEA2(0);
            sprintf(SHOW_ANGLE, "%u deg", (unsigned)angle_val);
            ENVIA_CADENA(SHOW_ANGLE);
            _delay_ms(700);

            sign = 1; // next stage: update servo
        }

        BARRE_TECLADO();    // advance column for next scan
        BARRE_TECLADO();    // call twice for stability if cycle is slow (adjust if needed)
        _delay_ms(3);

        // ----------- SERVO UPDATE: set OCR1B based on angle -----------
        if (sign == 1) {
            // Map angle to pulse (microseconds) and then to OCR1B
            uint16_t pulse = angle_to_pulse_us(angle_val);
            uint16_t ocr = pulse_us_to_OCR1B(pulse);

            // Update display and output
            LIMPIA_LCD();
            POS_LINEA1(0);
            ENVIA_CADENA(mensajeM);
            POS_LINEA2(0);
            sprintf(SHOW_ANGLE, "%u deg", (unsigned)angle_val);
            ENVIA_CADENA(SHOW_ANGLE);

            OCR1B = ocr;      // set servo pulse

            // stay in this state until user requests new entry via INT2
            sign = 2;         // lock until interrupt returns to 0
        }

        // ----------- INTERRUPT: return servo to 0 degrees -----------
        if (flag == 1) {
            // move servo to 0°
            uint16_t pulse0 = angle_to_pulse_us(0);
            OCR1B = pulse_us_to_OCR1B(pulse0);

            // show message
            LIMPIA_LCD();
            POS_LINEA1(0);
            ENVIA_CADENA(mensaje4);
            POS_LINEA2(0);
            ENVIA_CADENA(cero);
            _delay_ms(600);

            // clear buffers and states for new input
            clearArray(PRO, sizeof(PRO));
            clearArray(BUF, sizeof(BUF));
            clearArray(ANGULO_STR, sizeof(ANGULO_STR));
            clearArray(SHOW_ANGLE, sizeof(SHOW_ANGLE));
            sign = 0;
            flag = 0;

            // re-enable INT2
            GICR |= (1 << INT2);
            _delay_ms(300); // debounce
        }

        tecla = ' ';
    } // END MAIN LOOP
}

/*
======================================================================================
END OF FILE
======================================================================================
*/