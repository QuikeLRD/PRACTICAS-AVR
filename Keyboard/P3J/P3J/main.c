#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// Configuración de puertos para LCD
#define LCD_PUERTO PORTC
#define LCD_RS PC2
#define LCD_E PC3

// Configuración teclado matricial
#define FIL1   0x0F & ~(0x01)  // 0x0E
#define FIL2   0x0F & ~(0x02)  // 0x0D
#define FIL3   0x0F & ~(0x04)  // 0x0B
#define FIL4   0x0F & ~(0x08)  // 0x07

#define COL1    0xEF  // 0xFF & ~(0x10)
#define COL2    0xDF  // 0xFF & ~(0x20)
#define COL3    0xBF  // 0xFF & ~(0x40)
#define COL4    0x7F  // 0xFF & ~(0x80)

// Variables globales
char VALOR_TECLADO = ' ';
uint8_t modo_calculadora = 0;  // 0: Normal, 1: Menú, 2: Ingreso números
char operacion = ' ';
int numero1 = 0, numero2 = 0, resultado = 0;
uint8_t paso_ingreso = 0;  // 0: Esperando primer número, 1: Esperando segundo número
char buffer_numero[10] = "";

// Prototipos de función
void LCD_INICIALIZA();
void ENVIA_CMD(uint8_t dato);
void ENVIA_DATO(uint8_t dato);
void ENVIA_CADENA(const char *dato);
void LIMPIA_LCD();
void POS_LINEA1(uint8_t posicion);
void POS_LINEA2(uint8_t posicion);
void TECLADO_INIT();
char LEE_TECLADO();
void BARRE_TECLADO();
void procesar_tecla_normal(char tecla);
void mostrar_menu();
void procesar_tecla_menu(char tecla);
void procesar_tecla_ingreso(char tecla);
void realizar_operacion();
void mostrar_resultado();
void convertir_int_a_string(int numero, char* buffer);

// Función principal
int main(void)
{
	// Configurar puertos
	DDRC = 0xFF;  // Puerto C como salida (LCD)
	TECLADO_INIT();  // Configurar teclado
	
	// Inicializar LCD
	LCD_INICIALIZA();
	LIMPIA_LCD();
	POS_LINEA1(0);
	ENVIA_CADENA("Calculadora AVR");
	POS_LINEA2(0);
	ENVIA_CADENA("Presione #");
	_delay_ms(2000);
	
	while(1)
	{
		BARRE_TECLADO();
		char tecla = LEE_TECLADO();
		_delay_ms(100);  // Anti-rebote simple
		
		if(tecla != ' ') {
			if(modo_calculadora == 0) {
				procesar_tecla_normal(tecla);
				} else if(modo_calculadora == 1) {
				procesar_tecla_menu(tecla);
				} else if(modo_calculadora == 2) {
				procesar_tecla_ingreso(tecla);
			}
		}
	}
	return 0;
}

// ========== FUNCIONES TECLADO ==========
void TECLADO_INIT()
{
	DDRD = 0xF0;    // PD4-PD7 salidas, PD0-PD3 entradas
	PORTD = 0x0F;   // Pull-ups en entradas
	PORTD = COL1;   // Empezar con columna 1 activa
}

char LEE_TECLADO()
{
	VALOR_TECLADO = ' ';
	
	switch(PORTD)
	{
		case COL1:
		switch(PIND & 0x0F)
		{
			case FIL1: VALOR_TECLADO = '1'; break;
			case FIL2: VALOR_TECLADO = '2'; break;
			case FIL3: VALOR_TECLADO = '3'; break;
			case FIL4: VALOR_TECLADO = 'A'; break;
		}
		break;
		
		case COL2:
		switch(PIND & 0x0F)
		{
			case FIL1: VALOR_TECLADO = '4'; break;
			case FIL2: VALOR_TECLADO = '5'; break;
			case FIL3: VALOR_TECLADO = '6'; break;
			case FIL4: VALOR_TECLADO = 'B'; break;
		}
		break;
		
		case COL3:
		switch(PIND & 0x0F)
		{
			case FIL1: VALOR_TECLADO = '7'; break;
			case FIL2: VALOR_TECLADO = '8'; break;
			case FIL3: VALOR_TECLADO = '9'; break;
			case FIL4: VALOR_TECLADO = 'C'; break;
		}
		break;
		
		case COL4:
		switch(PIND & 0x0F)
		{
			case FIL1: VALOR_TECLADO = '*'; break;
			case FIL2: VALOR_TECLADO = '0'; break;
			case FIL3: VALOR_TECLADO = '#'; break;
			case FIL4: VALOR_TECLADO = 'D'; break;
		}
		break;
	}
	
	return VALOR_TECLADO;
}

void BARRE_TECLADO()
{
	switch(PORTD)
	{
		case COL1: PORTD = COL2; break;
		case COL2: PORTD = COL3; break;
		case COL3: PORTD = COL4; break;
		case COL4: PORTD = COL1; break;
		default:   PORTD = COL1; break;
	}
}

// ========== FUNCIONES LCD ==========
void ENVIA_CMD(uint8_t dato)
{
	uint8_t cmd_enviar;
	cmd_enviar = (dato & 0xF0);
	LCD_PUERTO = cmd_enviar;
	LCD_PUERTO &= ~(1 << LCD_RS);
	LCD_PUERTO |= (1 << LCD_E);
	_delay_ms(1);
	LCD_PUERTO &= ~(1 << LCD_E);
	_delay_ms(2);
	
	cmd_enviar = ((dato << 4) & 0xF0);
	LCD_PUERTO = cmd_enviar;
	LCD_PUERTO &= ~(1 << LCD_RS);
	LCD_PUERTO |= (1 << LCD_E);
	_delay_ms(1);
	LCD_PUERTO &= ~(1 << LCD_E);
	_delay_ms(2);
}

void ENVIA_DATO(uint8_t dato)
{
	uint8_t dto_enviar;
	dto_enviar = (dato & 0xF0);
	LCD_PUERTO = dto_enviar;
	LCD_PUERTO |= (1 << LCD_RS);
	LCD_PUERTO |= (1 << LCD_E);
	_delay_ms(1);
	LCD_PUERTO &= ~(1 << LCD_E);
	_delay_ms(2);
	
	dto_enviar = ((dato << 4) & 0xF0);
	LCD_PUERTO = dto_enviar;
	LCD_PUERTO |= (1 << LCD_RS);
	LCD_PUERTO |= (1 << LCD_E);
	_delay_ms(1);
	LCD_PUERTO &= ~(1 << LCD_E);
	_delay_ms(2);
}

void ENVIA_CADENA(const char *dato)
{
	uint8_t i = 0;
	while(dato[i] != '\0')
	{
		ENVIA_DATO(dato[i]);
		i++;
		_delay_ms(1);
	}
}

void LCD_INICIALIZA()
{
	_delay_ms(50);
	ENVIA_CMD(0x02);
	ENVIA_CMD(0x28);
	ENVIA_CMD(0x0C);
	ENVIA_CMD(0x06);
	ENVIA_CMD(0x01);
	_delay_ms(10);
}

void LIMPIA_LCD()
{
	ENVIA_CMD(0x01);
	_delay_ms(10);
}

void POS_LINEA1(uint8_t posicion)
{
	ENVIA_CMD(0x80 | (posicion & 0x0F));
}

void POS_LINEA2(uint8_t posicion)
{
	ENVIA_CMD(0xC0 | (posicion & 0x0F));
}

// ========== LÓGICA DE LA CALCULADORA ==========
void procesar_tecla_normal(char tecla)
{
	if(tecla == '#') {
		// Entrar al modo calculadora
		modo_calculadora = 1;
		mostrar_menu();
		} else if(tecla >= '0' && tecla <= '9') {
		// Mostrar número en LCD
		LIMPIA_LCD();
		POS_LINEA1(0);
		ENVIA_CADENA("Tecla: ");
		ENVIA_DATO(tecla);
	}
}

void mostrar_menu()
{
	LIMPIA_LCD();
	POS_LINEA1(0);
	ENVIA_CADENA("Seleccione op:");
	POS_LINEA2(0);
	ENVIA_CADENA("A:+ B:- C:* D:/");
}

void procesar_tecla_menu(char tecla)
{
	if(tecla == '#') {
		// Salir del modo calculadora
		modo_calculadora = 0;
		LIMPIA_LCD();
		POS_LINEA1(0);
		ENVIA_CADENA("Modo Normal");
		POS_LINEA2(0);
		ENVIA_CADENA("Presione #");
		return;
	}
	
	if(tecla == 'A' || tecla == 'B' || tecla == 'C' || tecla == 'D') {
		operacion = tecla;
		modo_calculadora = 2;
		paso_ingreso = 0;
		numero1 = 0;
		numero2 = 0;
		
		LIMPIA_LCD();
		POS_LINEA1(0);
		ENVIA_CADENA("Num1: ");
		POS_LINEA2(0);
		ENVIA_CADENA("Separe con *");
	}
}

void procesar_tecla_ingreso(char tecla)
{
	if(tecla == '#') {
		// Volver al menú
		modo_calculadora = 1;
		mostrar_menu();
		return;
	}
	
	if(tecla >= '0' && tecla <= '9') {
		// Agregar dígito al número actual
		if(paso_ingreso == 0) {
			numero1 = numero1 * 10 + (tecla - '0');
			POS_LINEA1(6);
			convertir_int_a_string(numero1, buffer_numero);
			ENVIA_CADENA(buffer_numero);
			} else {
			numero2 = numero2 * 10 + (tecla - '0');
			POS_LINEA2(6);
			convertir_int_a_string(numero2, buffer_numero);
			ENVIA_CADENA(buffer_numero);
		}
	}
	
	if(tecla == '*') {
		if(paso_ingreso == 0) {
			// Pasar al segundo número
			paso_ingreso = 1;
			POS_LINEA2(0);
			ENVIA_CADENA("Num2: ");
			} else {
			// Calcular resultado
			realizar_operacion();
			mostrar_resultado();
			_delay_ms(3000);
			// Volver al menú
			modo_calculadora = 1;
			mostrar_menu();
		}
	}
}

void realizar_operacion()
{
	switch(operacion) {
		case 'A':  // Suma
		resultado = numero1 + numero2;
		break;
		case 'B':  // Resta
		resultado = numero1 - numero2;
		break;
		case 'C':  // Multiplicación
		resultado = numero1 * numero2;
		break;
		case 'D':  // División
		if(numero2 != 0) {
			resultado = numero1 / numero2;
			} else {
			resultado = 0;  // Evitar división por cero
		}
		break;
	}
}

void mostrar_resultado()
{
	LIMPIA_LCD();
	POS_LINEA1(0);
	
	// Mostrar operación
	switch(operacion) {
		case 'A': ENVIA_CADENA(numero1); ENVIA_CADENA(" + "); ENVIA_CADENA(numero2); break;
		case 'B': ENVIA_CADENA(numero1); ENVIA_CADENA(" - "); ENVIA_CADENA(numero2); break;
		case 'C': ENVIA_CADENA(numero1); ENVIA_CADENA(" * "); ENVIA_CADENA(numero2); break;
		case 'D': ENVIA_CADENA(numero1); ENVIA_CADENA(" / "); ENVIA_CADENA(numero2); break;
	}
	
	POS_LINEA2(0);
	ENVIA_CADENA("Resultado: ");
	convertir_int_a_string(resultado, buffer_numero);
	ENVIA_CADENA(buffer_numero);
}

void convertir_int_a_string(int numero, char* buffer)
{
	// Conversión simple de entero a string
	uint8_t i = 0;
	if(numero == 0) {
		buffer[0] = '0';
		buffer[1] = '\0';
		return;
	}
	
	// Manejar números negativos
	if(numero < 0) {
		buffer[i++] = '-';
		numero = -numero;
	}
	
	// Convertir dígitos
	char temp[10];
	uint8_t j = 0;
	while(numero > 0) {
		temp[j++] = '0' + (numero % 10);
		numero /= 10;
	}
	
	// Invertir el orden
	while(j > 0) {
		buffer[i++] = temp[--j];
	}
	buffer[i] = '\0';
}