
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h" 

char mensaje[] = "Hola, LCD!";



int main(void)
{
	// Configura PORTC como salida (todos los pines)
	DDRC = 0xFF;

	// Inicializa el LCD
	LCD_INICIALIZA();

	

	// Limpia la pantalla y coloca el cursor en la posición inicial
	LIMPIA_LCD();
	POS_LINEA1(0); // Cursor en línea 1, posición 0

	// Escribe un mensaje de bienvenida
	
	ENVIA_CADENA(mensaje);

	_delay_ms(200);

	

	while (1)
	{
		
		// Opcional: Crea los caracteres especiales para la animación
		CAR_ESP0();
		CAR_ESP1();
		CAR_ESP2();
		CAR_ESP3();
		CAR_ESP4();
		CAR_ESP5();
		CAR_ESP6();
		CAR_ESP7();
		
				
				
				
		LIMPIA_LCD();
		POS_LINEA1(0);

		
		// Muestra los 8 caracteres especiales después del texto
		for (char i = 0; i < 8; i++) {
			ENVIA_DATO(i); // Visualiza el carácter especial i
			_delay_ms(300); // Pequeño retardo para verlos aparecer
		}
	
		POS_LINEA2(0);

		
		// Muestra los 8 caracteres especiales después del texto
		for (char i = 0; i < 8; i++) {
			ENVIA_DATO(i); // Visualiza el carácter especial i
			_delay_ms(300); // Pequeño retardo para verlos aparecer
		}
		
		POS_LINEA3(0);

		
		// Muestra los 8 caracteres especiales después del texto
		for (char i = 0; i < 8; i++) {
			ENVIA_DATO(i); // Visualiza el carácter especial i
			_delay_ms(300); // Pequeño retardo para verlos aparecer
		}		
		
		
	}
}