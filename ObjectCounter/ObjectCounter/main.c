/*
  Conteo con sensor IR en PB2 (INT2) + debounce por software (Timer1 one-shot)
  ATmega16, F_CPU = 1 MHz (ajusta si necesario).
  Sensor -> PB2 (INT2). Se asume activo LOW (lleva la línea a 0 cuando detecta).
  Si tu sensor es activo HIGH, cambia MCUCSR |= (1<<ISC2) para detectar rising edge.
*/

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"
#include "Keyboard.h"

#define DEBOUNCE_MS 200

/* Mensajes y buffers (como en tu código) */
char mensaje[]  = "Contador de";
char mensaje1[] = "Productos";
char mensaje3[] = "Productos: ";
char mensaje4[] = "Producto";
char mensaje5[] = "Terminado";
char espacio[]  = "   ";

char BUF[16];
char PRO[16];
char DATO[16];
char CONTEO_RE[16];

/* Estado y contadores */
volatile uint16_t productos = 0;
volatile uint16_t software_count = 0;
volatile uint8_t sign = 0; // 0 idle, 1 counting, 3 finished

/* --- Timer1 (debounce) helpers --- */
static void debounce_timer1_init(void)
{
    // Timer1 CTC mode (WGM12 = 1)
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);

    // Prescaler = 64, calcular OCR1A para DEBOUNCE_MS:
    uint32_t ticks = ( (uint32_t)F_CPU * (uint32_t)DEBOUNCE_MS ) / (1000UL * 64UL);
    if (ticks == 0) ticks = 1;
    if (ticks > 0xFFFF) ticks = 0xFFFF;
    OCR1A = (uint16_t)(ticks - 1);

    // Mantener timer parado; habilitación se hace al arrancar debounce
    TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
    TIMSK &= ~(1 << OCIE1A);
    TIFR |= (1 << OCF1A);
}

static inline void timer1_start_debounce(void)
{
    // Prescaler 64 -> CS11=1, CS10=1
    TCCR1B |= (1 << CS11) | (1 << CS10);
    TIMSK |= (1 << OCIE1A);
}

static inline void timer1_stop(void)
{
    TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
    TIMSK &= ~(1 << OCIE1A);
    TIFR |= (1 << OCF1A);
}

/* --- ISRs --- */
/* INT2_vect: detección del sensor en PB2 */
ISR(INT2_vect)
{
    // Deshabilitar INT2 para ignorar rebotes
    GICR &= ~(1 << INT2);

    // Incrementar conteo válido
    software_count++;

    // Si alcanzamos la meta, encendemos indicador y marcamos terminado
    if (productos > 0 && software_count >= productos) {
        PORTB |= (1 << PB5); // enciende LED en PB5
        sign = 3;
        // arrancar debounce para "limpiar" flancos remanentes pero no reactivar INT2
        TCNT1 = 0;
        TIFR |= (1 << OCF1A);
        timer1_start_debounce();
        return;
    }

    // Si no hemos terminado, arrancar debounce; al vencimiento se reactivará INT2
    TCNT1 = 0;
    TIFR |= (1 << OCF1A);
    timer1_start_debounce();
}

/* Timer1 compare A: fin del debounce */
ISR(TIMER1_COMPA_vect)
{
    // Parar timer y deshabilitar su interrupción
    timer1_stop();

    // Si seguimos en modo conteo, re-habilitar INT2
    if (sign == 1) {
        GIFR |= (1 << INTF2); // limpiar flag (nota: INTF2 depende del AVRs; si no existe usa INTF0/1 según datasheet)
        GICR |= (1 << INT2);
    }
    TIFR |= (1 << OCF1A);
}

/* limpia arrays */
void clearArray(char* myArray, short sizeOfMyArray){
    if (myArray && sizeOfMyArray > 0) memset(myArray, 0, (size_t)sizeOfMyArray);
}

/* --- main --- */
int main(void)
{
    // Configuraciones LCD/teclado tal como usas
    DDRC = 0xFFU;
    DDRD = 0xF0U;
    PORTD = COL1;

    // PB5 salida indicador
    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);

    // PB2 (INT2) como entrada con pull-up (sensor debe tirar a LOW para detectar)
    DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2); // pull-up

    // Inicializa LCD
    LCD_INICIALIZA();
    LIMPIA_LCD();
    POS_LINEA1(0);
    ENVIA_CADENA(mensaje);
    POS_LINEA2(0);
    ENVIA_CADENA(mensaje1);
    _delay_ms(800);
    LIMPIA_LCD();

    // Init debounce timer (pero detenido)
    debounce_timer1_init();

    // Configurar INT2:
    // MCUCSR bit ISC2: 0 -> falling edge, 1 -> rising edge
    // Asumimos sensor activo LOW -> detectar falling edge
    MCUCSR &= ~(1 << ISC2); // falling edge
    GIFR |= (1 << INTF2);   // limpiar flag
    GICR &= ~(1 << INT2);   // mantener deshabilitado hasta confirmar productos

    // Inicializar variables
    clearArray(PRO, sizeof(PRO));
    clearArray(DATO, sizeof(DATO));
    clearArray(BUF, sizeof(BUF));
    clearArray(CONTEO_RE, sizeof(CONTEO_RE));
    productos = 0;
    software_count = 0;
    sign = 0;

    sei(); // enable interrupts

    // Bucle principal: teclado y gestión de LCD (similar a tu flujo)
    while (1) {
        char tecla = LEE_TECLADO();

        if ((tecla != ' ') && (tecla != 'A') && (tecla != 'B') && (tecla != 'C') && (tecla != 'D') && (tecla != '#') && (tecla != '*') && (sign != 2)) {
            char tmp[2] = {0};
            tmp[0] = tecla;
            if (strlen(PRO) < sizeof(PRO) - 1) strcat(PRO, tmp);
            LIMPIA_LCD();
            ENVIA_CADENA(PRO);
            _delay_ms(200);
        }
        else if (tecla == '#') {
            // confirmar cantidad
            strncpy(DATO, PRO, sizeof(DATO)-1);
            clearArray(PRO, sizeof(PRO));
            clearArray(BUF, sizeof(BUF));
            LIMPIA_LCD();
            _delay_ms(100);

            productos = (uint16_t)atoi(DATO);
            if (productos == 0) productos = 1;

            software_count = 0;
            PORTB &= ~(1 << PB5); // apagar indicador
            sign = 1;

            // Pulsar '#' arma el conteo y habilita INT2
            POS_LINEA1(0);
            ENVIA_CADENA(mensaje3);
            POS_LINEA1(10);
            char tmpbuf[8];
            snprintf(tmpbuf, sizeof(tmpbuf), "%u", (unsigned)(productos - software_count));
            ENVIA_CADENA(tmpbuf);

            // Habilitar INT2
            GIFR |= (1 << INTF2);
            GICR  |= (1 << INT2);

            clearArray(DATO, sizeof(DATO));
        }

        if (sign == 1) {
            uint16_t restante = productos - software_count;
            static uint16_t last_rest = 0xFFFF;
            if (restante != last_rest) {
                last_rest = restante;
                POS_LINEA1(10);
                ENVIA_CADENA(espacio);
                POS_LINEA1(10);
                char buf[8];
                snprintf(buf, sizeof(buf), "%u", (unsigned)restante);
                ENVIA_CADENA(buf);
            }
        }

        if (sign == 3) {
            // ya terminado: asegurar INT2 deshabilitado y mostrar mensaje una vez
            GICR &= ~(1 << INT2);
            LIMPIA_LCD();
            POS_LINEA1(5);
            ENVIA_CADENA(mensaje4);
            POS_LINEA2(5);
            ENVIA_CADENA(mensaje5);
            _delay_ms(500);
            sign = 2;
        }

        BARRE_TECLADO();
        _delay_ms(3);
    }

    return 0;
}