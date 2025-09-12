
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h" 

char mensaje[] = "Bienvenido, a";
char mensaje2[] ="PlantasVS Zombis";


int main(void)
{
	// Configura PORTC como salida (todos los pines)
	DDRC = 0xFF;

	// Inicializa el LCD
	LCD_INICIALIZA();

	
	// Limpia la pantalla y coloca el cursor en la posición inicial
	LIMPIA_LCD();

	// Escribe un mensaje de bienvenida
	POS_LINEA1(0); // Cursor en línea 1, posición 0
	ENVIA_CADENA(mensaje);
	POS_LINEA2(0); // Cursor en línea 1, posición 0
	ENVIA_CADENA(mensaje2);
	_delay_ms(200);

	CAR_ESP0();
	CAR_ESP1();
	CAR_ESP2();
	CAR_ESP3();
	CAR_ESP4();
	CAR_ESP5();
	CAR_ESP6();
	CAR_ESP7();

	while (1)
	{
		
				
		LIMPIA_LCD();
		
		//GIRASOL1
		POS_LINEA1(0);
		ENVIA_DATO(3);
		_delay_ms(500);
		
		//GIRASOL2
		POS_LINEA2(0);
		ENVIA_DATO(3);
		_delay_ms(500);
		
		//ZOMBI1 SPAM
		POS_LINEA1(15);
		ENVIA_DATO(5);
		_delay_ms(500);
		
		//BORRO ZOMBI1
		POS_LINEA1(15);
		ENVIA_DATO(' ');  // Escribe espacio para borrar
		_delay_ms(10);
		//ZOMBI P1
		POS_LINEA1(14);
		ENVIA_DATO(5);
		_delay_ms(500);
		
		//BORRO ZOMBI P1
		POS_LINEA1(14);
		ENVIA_DATO(' ');  
		_delay_ms(10);
		//ZOMBI1 P2
		POS_LINEA1(13);
		ENVIA_DATO(5);
		_delay_ms(500);
		
		//PONGO PLANTA
		POS_LINEA1(1);
		ENVIA_DATO(0);
		//BORRO ZOMBI1 P2
		POS_LINEA1(13);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI1 P3
		POS_LINEA1(12);
		ENVIA_DATO(5);
		_delay_ms(500);
				
		//DISPARO 1
		POS_LINEA1(2);
		ENVIA_DATO(4);
		//BORRO ZOMBI1 P3
		POS_LINEA1(12);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI1 P4
		POS_LINEA1(11);
		ENVIA_DATO(5);
		_delay_ms(500);
		
		//BORRO DISPARO1
		POS_LINEA1(2);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO 2
		POS_LINEA1(3);
		ENVIA_DATO(4);
		//BORRO ZOMBI1 P4
		POS_LINEA1(11);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI1 P5
		POS_LINEA1(10);
		ENVIA_DATO(5);
		_delay_ms(500);
		
		
		//BORRO DISPARO2
		POS_LINEA1(3);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO 3
		POS_LINEA1(4);
		ENVIA_DATO(4);
		//BORRO ZOMBI1 P4
		POS_LINEA1(10);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI2 P1
		POS_LINEA2(15);
		ENVIA_DATO(7);
		//ZOMBI P6
		POS_LINEA1(9);
		ENVIA_DATO(5);
		_delay_ms(500);
				
		
		//BORRO DISPARO3
		POS_LINEA1(4);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO4
		POS_LINEA1(5);
		ENVIA_DATO(4);
		//BORRO ZOMBI P2
		POS_LINEA1(9);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//BORRO ZOMBI2 P2
		POS_LINEA2(15);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI P3
		POS_LINEA1(8);
		ENVIA_DATO(5);
		//ZOMBI2 P1
		POS_LINEA2(14);
		ENVIA_DATO(7);		
		//BORRO ZOMBI2 P2
		_delay_ms(500);
		
		
		//PONGO PLANTA2
		POS_LINEA2(1);
		ENVIA_DATO(0);
		//BORRO ZOMBI P4
		POS_LINEA1(8);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MUEVO ZOMBI1 P4
		POS_LINEA1(7);
		ENVIA_DATO(5);	
		//BORRO DISPARO4
		POS_LINEA1(5);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO 5
		POS_LINEA1(6);
		ENVIA_DATO(4);
		//BORRO ZOMBI P4
		POS_LINEA2(14);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//ZOMBI2 P3
		POS_LINEA2(13);
		ENVIA_DATO(7);
		_delay_ms(500);
		
		//ZOMBI1 Golpeado
		POS_LINEA1(6);
		ENVIA_DATO(2);
		//BORRAR ZOMBI1
		POS_LINEA1(7);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO PLANTA2
		POS_LINEA2(2);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI 2 (13)
		POS_LINEA2(13);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI 2
		POS_LINEA2(12);
		ENVIA_DATO(7);
		_delay_ms(500);
		
		//MUERE ZOMBI1
		POS_LINEA1(6);
		ENVIA_DATO(' ');
		//BORRO DISPARO1 PLANTA 2
		POS_LINEA2(2);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO PLANTA2
		POS_LINEA2(3);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI 2 (12)
		POS_LINEA2(12);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI 2 (11)
		POS_LINEA2(11);
		ENVIA_DATO(7);
		_delay_ms(500);
		
		//COLOCAMOS NUEZ
		POS_LINEA2(8);
		ENVIA_DATO(6);
		//BORRAMOS ZOMBI 2 (11)
		POS_LINEA2(11);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI 2 (10)
		POS_LINEA2(10);
		ENVIA_DATO(7);
		//BORRO DISPARO2 PLANTA2
		POS_LINEA2(3);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO3  PLANTA2
		POS_LINEA2(4);
		ENVIA_DATO(4);
		_delay_ms(500);
		
		
		//ZOMBI DE CUBETA
		POS_LINEA2(14);
		ENVIA_DATO(1);
		//BORRAMOS ZOMBI 2 (9)
		POS_LINEA2(10);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI 2 (11)
		POS_LINEA2(9);
		ENVIA_DATO(7);
		//BORRO DISPARO3 PLANTA2
		POS_LINEA2(4);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO PLANTA2
		POS_LINEA2(5);
		ENVIA_DATO(4);
		_delay_ms(500);
		
		
		//ZOMBI DE CUBETA 2
		POS_LINEA1(14);
		ENVIA_DATO(1);
		//BORRO DISPARO3 PLANTA2
		POS_LINEA2(5);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO PLANTA2
		POS_LINEA2(6);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI 2 (9)
		POS_LINEA2(9);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI 2 (11)
		POS_LINEA2(8);
		ENVIA_DATO(7);
		//BORRAMOS ZOMBI DE CUBETA (1)
		POS_LINEA2(14);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA
		POS_LINEA2(13);
		ENVIA_DATO(1);
		_delay_ms(500);
		
		//NUEVO LANZA GUISANTES
		POS_LINEA2(2);
		ENVIA_DATO(0);
		//BORRO DISPARO PLANTA2
		POS_LINEA2(6);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARO PLANTA2
		POS_LINEA2(7);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI 2 (8)
		POS_LINEA2(8);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI 2 (11)
		POS_LINEA2(7);
		ENVIA_DATO(5);
		//BORRAMOS ZOMBI DE CUBETA (1)
		POS_LINEA2(13);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA
		POS_LINEA2(12);
		ENVIA_DATO(1);
		//BORRAMOS ZOMBI DE CUBETA2 
		POS_LINEA1(14);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA 2
		POS_LINEA1(13);
		ENVIA_DATO(1);
		_delay_ms(500);
		
		//DISPARO PLANTA3
		POS_LINEA2(3);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI 2 (7)
		POS_LINEA2(7);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI 2 (6)
		POS_LINEA2(6);
		ENVIA_DATO(5);
		//BORRAMOS ZOMBI DE CUBETA (2)
		POS_LINEA2(12);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA
		POS_LINEA2(11);
		ENVIA_DATO(1);
		//BORRAMOS ZOMBI DE CUBETA2
		POS_LINEA1(13);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA 2
		POS_LINEA1(12);
		ENVIA_DATO(1);
		_delay_ms(500);
		
		//DISPARO PLANTA3
		POS_LINEA2(4);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI 2 (6)
		POS_LINEA2(6);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI 2 (5)
		POS_LINEA2(5);
		ENVIA_DATO(5);
		//BORRAMOS ZOMBI DE CUBETA (3)
		POS_LINEA2(11);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA
		POS_LINEA2(10);
		ENVIA_DATO(1);
		//BORRAMOS ZOMBI DE CUBETA2
		POS_LINEA1(12);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA 2
		POS_LINEA1(11);
		ENVIA_DATO(1);
		_delay_ms(500);
		
		//BORRAMOS DISPARO PLANTA 2
		POS_LINEA1(3);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//BORRAMOS ZOMBI 2 (5)
		POS_LINEA2(5);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI 2 (4)
		POS_LINEA2(4);
		ENVIA_DATO(2);
		//BORRAMOS ZOMBI DE CUBETA (4)
		POS_LINEA2(10);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA
		POS_LINEA2(9);
		ENVIA_DATO(1);
		//BORRAMOS ZOMBI DE CUBETA2
		POS_LINEA1(11);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA 2
		POS_LINEA1(10);
		ENVIA_DATO(1);
		_delay_ms(500);
		
		//MUERE ZOMBI
		POS_LINEA2(4);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//BORRAMOS DISPARO PLANTA 2
		POS_LINEA2(3);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARA PLANTA
		POS_LINEA2(4);
		ENVIA_DATO(4);
		//DISPARA PLANTA
		POS_LINEA1(2);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI DE CUBETA (5)
		POS_LINEA2(9);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA
		POS_LINEA2(8);
		ENVIA_DATO(1);
		//BORRAMOS ZOMBI DE CUBETA2
		POS_LINEA1(10);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA 2
		POS_LINEA1(9);
		ENVIA_DATO(1);
		_delay_ms(500);
		
		//BORRAMOS DISPARO PLANTA 2
		POS_LINEA2(4);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARA PLANTA
		POS_LINEA2(5);
		ENVIA_DATO(4);
		//BORRAMOS DISPARO PLANTA 4
		POS_LINEA1(2);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARA PLANTA
		POS_LINEA1(3);
		ENVIA_DATO(4);
		//DISPARA PLANTA1
		POS_LINEA2(3);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI DE CUBETA (6)
		POS_LINEA2(8);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA
		POS_LINEA2(7);
		ENVIA_DATO(1);
		//BORRAMOS ZOMBI DE CUBETA2
		POS_LINEA1(9);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA 2
		POS_LINEA1(8);
		ENVIA_DATO(1);
		_delay_ms(500);
		
		//BORRAMOS DISPARO PLANTA 2
		POS_LINEA2(5);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//BORRAMOS ZOMBI CUBETA
		POS_LINEA2(7);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARA PLANTA
		POS_LINEA2(6);
		ENVIA_DATO(7);
		//BORRAMOS DISPARO PLANTA 4
		POS_LINEA1(3);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARA PLANTA
		POS_LINEA1(4);
		ENVIA_DATO(4);
		//BORRAMOS DISPARO PLANTA 1
		POS_LINEA2(3);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARA PLANTA1
		POS_LINEA2(4);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI DE CUBETA2
		POS_LINEA1(8);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA 2
		POS_LINEA1(7);
		ENVIA_DATO(1);
		_delay_ms(500);
		
		
		//BORRAMOS DISPARO PLANTA 2
		POS_LINEA2(6);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//BORRAMOS DISPARO PLANTA 2
		POS_LINEA2(4);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//BORRAMOS ZOMBI CUBETA
		POS_LINEA2(5);
		ENVIA_DATO(5);
		//BORRAMOS DISPARO PLANTA 4
		POS_LINEA1(4);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//DISPARA PLANTA
		POS_LINEA1(5);
		ENVIA_DATO(4);
		//BORRAMOS ZOMBI DE CUBETA2
		POS_LINEA1(7);
		ENVIA_DATO(' ');
		_delay_ms(10);
		//MOVEMOS ZOMBI DE CUBETA 2
		POS_LINEA1(6);
		ENVIA_DATO(1);
		_delay_ms(500);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
					
		
	}
}