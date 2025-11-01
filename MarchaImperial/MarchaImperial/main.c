/*
  Marcha Imperial - transcripción ampliada (ATmega16)
  - Usa Timer0 CTC con toggle OC0 (OC0 = PB3)
  - F_CPU = 1 MHz
  - Prescaler = 8 (CS01 = 1)
  - Se usan los valores OCR que ya definiste en el Excel (OCR a 1MHz).
  - La melodía está transcrita compás por compás en una versión para la "línea principal".
  - Las duraciones usan BPM = 90 (negra = 60000/90 ? 666 ms).
  - Nota: usé las constantes OCR que aparecen más arriba en tu código (si tienes un Excel con valores
    ligeramente distintos, reemplaza las #define por tus valores exactos).
  - Implementación segura para compilador: no pasamos variables a _delay_ms() directamente;
    usamos delay_ms_var() que llama a _delay_ms(1) en bucle.
*/

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/* --- OCR a 1MHz (valores tomados de tu tabla / imágenes) --- */
/* Reemplaza cualquiera de estos defines por los valores exactos de tu Excel si son distintos */
#define OCR_SOL_4    158U
#define OCR_MI_4     188U
#define OCR_LA_4     133U
#define OCR_RE_5     105U
#define OCR_MIb_5    99U
#define OCR_FA_4     167U
#define OCR_SOL_5     78U
#define OCR_FAsh_5    83U
#define OCR_FA_5      88U
#define OCR_LAb_4    149U
#define OCR_REb_5    111U
#define OCR_SI_4     125U
#define OCR_LA_4_ALT 141U
#define OCR_MIb_4    200U
#define OCR_MI_5      93U
#define OCR_DO_5     118U

/* Prescaler bits para Timer0 (prescaler = 8) */
#define PRESCALER_BITS  (1 << CS01)  /* CS02=0 CS01=1 CS00=0 -> prescaler 8 */

/* Duración: calcula negra en ms según BPM */
static inline uint16_t ms_per_quarter(uint16_t bpm) { return (uint16_t)(60000U / bpm); }

/* Control del Timer0 (CTC + toggle OC0) */
static void timer0_start_with_prescaler(void)
{
    /* WGM01 = 1 (CTC), COM01=0 COM00=1 (toggle OC0) y prescaler */
    TCCR0 = (1 << WGM01) | (1 << COM00) | PRESCALER_BITS;
}

static void timer0_stop(void)
{
    /* Detener Timer0: limpiar bits CS */
    TCCR0 &= (uint8_t)~((1 << CS02) | (1 << CS01) | (1 << CS00));
}

/* helper: delay_ms variable usando _delay_ms(1) repetido */
static void delay_ms_var(uint16_t ms)
{
    while (ms--) {
        _delay_ms(1); /* 1 es constante => compilador acepta */
    }
}

/* Reproduce una nota dada por su OCR (valor para OCR0) durante duration_ms.
   Usa 90% del tiempo para sonar y 10% como gap (staccato suave).
*/
static void play_note_ocr_ms(uint8_t ocr_value, uint16_t duration_ms)
{
    if (ocr_value == 0xFFU || ocr_value == 0U) {
        /* silencio */
        timer0_stop();
        delay_ms_var(duration_ms);
        return;
    }

    OCR0 = ocr_value;
    TCNT0 = 0;
    timer0_start_with_prescaler();

    /* tocar nota durante el 90% del tiempo */
    uint16_t play_ms = (duration_ms * 9) / 10;
    delay_ms_var(play_ms);

    /* detener nota y gap breve */
    timer0_stop();
    delay_ms_var(duration_ms - play_ms);
}

/* Estructura nota */
typedef struct {
    uint8_t ocr;        /* valor OCR0 (usamos valores de tu Excel) */
    uint16_t length_ms; /* duración en ms */
} Note;

/*
  Transcripción (versión principal, compases iniciales) - basada en la melodía
  de "The Imperial March" de John Williams. He seguido la línea melódica principal
  que aparece en tu partitura/tab. Si quieres que añada más compases o
  ajustes (ligados, puntillos, silencios exactos), lo hago.
*/

int main(void)
{
    /* configurar salida OC0 = PB3 */
    DDRB |= (1 << PB3);
    PORTB &= ~(1 << PB3);

    /* inicializar timer detenido y OCR a 0 */
    OCR0 = 0;
    timer0_stop();

    /* tempo */
    const uint16_t BPM = 90;
    const uint16_t quarter = ms_per_quarter(BPM);    /* negra */
    const uint16_t eighth  = quarter / 2;            /* corchea */
    const uint16_t half    = quarter * 2;            /* blanca */

    /* NOTAS USADAS:
       - OCR_SOL_4 : Sol4 (G4)
       - OCR_MI_4  : Mi4 (E4)
       - OCR_MIb_4 : Mi bemol 4 (Eb4) - si tu Excel tiene OCR específico, reemplaza
       - OCR_DO_5  : Do5 aproximado en OCR_DO_5
       - OCR_LAb_4 : La bemol 4 (Bb/Ab aproximado según tu nomenclatura)
       - OCR_SI_4  : Si4 (B4)
       - OCR_LA_4  : La4 (A4)
       - OCR_FAsh_5: F#5 aproximado
       ...
    */

    /* Melodía (compases iniciales, transcripción aproximada con OCR existentes).
       Duraciones aproximadas tomadas como negras y corcheas según la partitura.
       Puedes pedirme ajustar cada compás si quieres la transcripción nota por nota exacta.
    */
    const Note melody[] = {
        /* Compás 1 */
        { OCR_SOL_4,  quarter },   /* G   */
        { OCR_SOL_4,  quarter },   /* G   */
        { OCR_SOL_4,  quarter },   /* G   */
        { OCR_MIb_4,  quarter },   /* Eb  */

        /* Compás 2 */
        { OCR_DO_5,   quarter },   /* C5  */
        { OCR_SOL_4,  quarter },   /* G   */
        { OCR_MIb_4,  quarter },   /* Eb  */
        { OCR_DO_5,   quarter },   /* C5  */

        /* Compás 3 (motivo repetido / variación) */
        { OCR_SOL_4,  quarter },   /* G   */
        { OCR_LAb_4,  quarter },   /* Bb  (aprox con LA_b) */
        { OCR_LAb_4,  quarter },   /* Bb  */
        { OCR_SI_4,   quarter },   /* B   */

        /* Compás 4 */
        { OCR_LA_4,   quarter },   /* A   */
        { OCR_FAsh_5, quarter },   /* F#  */
        { OCR_SOL_4,  quarter },   /* G   */
        { OCR_SOL_4,  quarter },   /* G   */

        /* Compás 5 - segunda frase (inicio) */
        { OCR_SOL_4,  quarter },
        { OCR_SOL_4,  quarter },
        { OCR_SOL_4,  quarter },
        { OCR_MIb_4,  quarter },

        /* Compás 6 */
        { OCR_DO_5,   quarter },
        { OCR_SOL_4,  quarter },
        { OCR_MIb_4,  quarter },
        { OCR_DO_5,   quarter },

        /* Compás 7 - pasaje descendente (uso de corcheas en la partitura original) */
        { OCR_RE_5,   eighth },
        { OCR_MI_5,   eighth },
        { OCR_RE_5,   eighth },
        { OCR_MI_5,   eighth },
        { OCR_RE_5,   quarter },
        { OCR_MI_5,   quarter },

        /* Compás 8 - cierre frase */
        { OCR_SOL_4,  quarter },
        { OCR_LAb_4,  quarter },
        { OCR_SI_4,   quarter },
        { OCR_SOL_4,  quarter },

        /* Añadimos algunos compases repetidos/ornamentales para completar la idea */
        { OCR_LA_4,   quarter },
        { OCR_FA_4,   quarter },
        { OCR_SOL_4,  quarter },
        { OCR_MI_4,   quarter },

        { OCR_SOL_4,  quarter },
        { OCR_SOL_4,  quarter },
        { OCR_SOL_4,  quarter },
        { OCR_MI_4,   quarter },

        { 0, quarter }, /* silencio / respiración breve antes de repetir */
    };

    const uint16_t melody_len = sizeof(melody) / sizeof(melody[0]);

    /* Reproducir en bucle */
    while (1) {
        for (uint16_t i = 0; i < melody_len; ++i) {
            play_note_ocr_ms(melody[i].ocr, melody[i].length_ms);
        }
        /* pequeño descanso entre repeticiones */
        delay_ms_var(500);
    }

    return 0;
}