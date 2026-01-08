#define F_CPU 8000000UL
#include <xc.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

// ================= CONFIGURA QUIÉN ES EL MICRO =================
#define MICRO_ID 2        // CAMBIAR A 2 EN EL OTRO MICRO

// =================== LCD ====================
#define LCD_PUERTO PORTC
#define LCD_RS PC2
#define LCD_E  PC3

void LCD_INICIALIZA();
void ENVIA_CMD(uint8_t dato);
void ENVIA_DATO(uint8_t dato);
void ENVIA_CADENA(const char *dato);
void LIMPIA_LCD();
void POS_LINEA1(char pos);
void POS_LINEA2(char pos);

// =================== TECLADO (PORTB) ====================
#define FIL1 (0x0F & ~0x01)
#define FIL2 (0x0F & ~0x02)
#define FIL3 (0x0F & ~0x04)
#define FIL4 (0x0F & ~0x08)

#define COL1 0xEF
#define COL2 0xDF
#define COL3 0xBF
#define COL4 0x7F

char LEE_TECLADO();
void BARRE_TECLADO();

// =================== USART ====================
void USART_INIT(void);
void USART_TX(char dato);
char USART_RX(void);

// =================== TECLADO ====================
char LEE_TECLADO()
{
	switch(PORTB)
	{
		case COL1:
		switch(PINB & 0x0F)
		{
			case FIL1: return '1';
			case FIL2: return '2';
			case FIL3: return '3';
			case FIL4: return 'A';
		} break;

		case COL2:
		switch(PINB & 0x0F)
		{
			case FIL1: return '4';
			case FIL2: return '5';
			case FIL3: return '6';
			case FIL4: return 'B';
		} break;

		case COL3:
		switch(PINB & 0x0F)
		{
			case FIL1: return '7';
			case FIL2: return '8';
			case FIL3: return '9';
			case FIL4: return 'C';
		} break;

		case COL4:
		switch(PINB & 0x0F)
		{
			case FIL1: return '*';
			case FIL2: return '0';
			case FIL3: return '#';
			case FIL4: return 'D';
		} break;
	}
	return ' ';
}

void BARRE_TECLADO()
{
	switch(PORTB)
	{
		case COL1: PORTB = COL2; break;
		case COL2: PORTB = COL3; break;
		case COL3: PORTB = COL4; break;
		default:   PORTB = COL1;
	}
}

// =================== USART ====================
void USART_INIT(void)
{
	UBRRL = 51; // 9600 bps @ 8 MHz
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
}

void USART_TX(char dato)
{
	while(!(UCSRA & (1<<UDRE)));
	UDR = dato;
}

char USART_RX(void)
{
	if(UCSRA & (1<<RXC))
	return UDR;
	return 0;
}

// =================== LCD ====================
void LCD_INICIALIZA()
{
	ENVIA_CMD(0x02);
	ENVIA_CMD(0x28);
	ENVIA_CMD(0x0E);
	ENVIA_CMD(0x06);
	ENVIA_CMD(0x01);
	ENVIA_CMD(0x80);
}

void ENVIA_CMD(uint8_t dato)
{
	LCD_PUERTO = dato & 0xF0;
	LCD_PUERTO &= ~(1<<LCD_RS);
	LCD_PUERTO |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PUERTO &= ~(1<<LCD_E);

	LCD_PUERTO = (dato<<4) & 0xF0;
	LCD_PUERTO &= ~(1<<LCD_RS);
	LCD_PUERTO |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PUERTO &= ~(1<<LCD_E);
	_delay_ms(2);
}

void ENVIA_DATO(uint8_t dato)
{
	LCD_PUERTO = dato & 0xF0;
	LCD_PUERTO |= (1<<LCD_RS) | (1<<LCD_E);
	_delay_ms(1);
	LCD_PUERTO &= ~(1<<LCD_E);

	LCD_PUERTO = (dato<<4) & 0xF0;
	LCD_PUERTO |= (1<<LCD_RS) | (1<<LCD_E);
	_delay_ms(1);
	LCD_PUERTO &= ~(1<<LCD_E);
	_delay_ms(2);
}

void ENVIA_CADENA(const char *dato)
{
	while(*dato) ENVIA_DATO(*dato++);
}

void LIMPIA_LCD(){ ENVIA_CMD(0x01); }
void POS_LINEA1(char pos){ ENVIA_CMD(0x80 | pos); }
void POS_LINEA2(char pos){ ENVIA_CMD(0xC0 | pos); }

// =================== MAIN ====================
int main(void)
{
	DDRC = 0xFF;    // LCD
	DDRB = 0xF0;    // Teclado en PORTB
	PORTB = COL1;

	LCD_INICIALIZA();
	USART_INIT();

	LIMPIA_LCD();
	POS_LINEA1(0);

	#if MICRO_ID == 1
	ENVIA_CADENA("MICRO 1");
	#else
	ENVIA_CADENA("MICRO 2");
	#endif

	POS_LINEA2(0);
	ENVIA_CADENA("Tecla:");

	char tecla, rx;

	while(1)
	{
		tecla = LEE_TECLADO();
		if(tecla != ' ')
		{
			POS_LINEA2(7);
			ENVIA_DATO(tecla);
			USART_TX(tecla);
			_delay_ms(300);
		}

		rx = USART_RX();
		if(rx)
		{
			POS_LINEA1(9);
			ENVIA_DATO(rx);
		}

		BARRE_TECLADO();
		_delay_ms(100);
	}
}
