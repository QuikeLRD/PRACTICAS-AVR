#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

// Tick de Timer1 con prescaler=8 -> 8 us
#define US_TO_COUNTS(us) ((uint16_t)((us) / 8))

// Ajusta estos tres según tu servo:
#define SERVO_STOP_US     1500  // Pulso neutro para detener (ajusta 1480–1520 si hace falta)
#define SERVO_FWD_US      1450  // Pulso para girar lento en un sentido (más lejos de 1500 = más velocidad)
#define STEP_TIME_MS      80    // Tiempo que mantienes el pulso de giro para aproximar ~10° (ajusta aquí)
#define PAUSE_BETWEEN_MS  10000 // Espera de 10 s entre pasos

// Cantidad de pasos de 10° que quieres dar (36 pasos ~ 360°)
#define NUM_STEPS         36

int main(void)
{
	// OC1A (PD5, pin 19) como salida
	DDRD |= (1 << PD5);

	// Timer1: Fast PWM, TOP=ICR1 (modo 14)
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	// No invertido en OC1A
	TCCR1A |= (1 << COM1A1);
	// Prescaler = 8
	TCCR1B |= (1 << CS11);

	// TOP para 20 ms: 1e6 / (8*50) = 2500 -> -1 = 2499
	ICR1 = 2499;

	// Arranca detenido
	OCR1A = US_TO_COUNTS(SERVO_STOP_US);
	_delay_ms(1000);

	while (1) {
		for (uint8_t i = 0; i < NUM_STEPS; i++) {
			// Aplica giro lento
			OCR1A = US_TO_COUNTS(SERVO_FWD_US);
			_delay_ms(STEP_TIME_MS);

			// Detén el servo
			OCR1A = US_TO_COUNTS(SERVO_STOP_US);

			// Espera 10 s
			_delay_ms(PAUSE_BETWEEN_MS);
		}

		// Opcional: después de completar los pasos, queda detenido
		OCR1A = US_TO_COUNTS(SERVO_STOP_US);
		while (1) { }
	}

	return 0;
}