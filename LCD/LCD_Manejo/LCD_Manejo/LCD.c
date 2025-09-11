#include "LCD.h"
#include <util/delay.h>

// Implementación de funciones
void LCD_INICIALIZA(void)
{
	ENVIA_CMD(0x02);
	ENVIA_CMD(0x28); // DL = 0 comunicacion de 4 bits, N = 1 Pantalla de 2 lineas, F = 0 Caracteres de 5x7 puntos
	ENVIA_CMD(0x0E); // D=1 Display ON Cursor OFF
	ENVIA_CMD(0x06); // I/D = 1 Incremento Automatico S=0
	ENVIA_CMD(0x01); // Limpia LCD
	ENVIA_CMD(0x80);
}

void ENVIA_CMD(char dato)
{
	char cmd_enviar;
	cmd_enviar = (dato & 0xF0); // nibble alto
	LCD_PUERTO = cmd_enviar;
	LCD_PUERTO &= ~(1 << LCD_RS); // RS = 0
	LCD_PUERTO |= (1 << LCD_E);   // E = 1
	_delay_ms(1);
	LCD_PUERTO &= ~(1 << LCD_E);  // E = 0
	_delay_ms(10);
	cmd_enviar = ((dato << 4) & 0xF0); // nibble bajo
	LCD_PUERTO = cmd_enviar;
	LCD_PUERTO &= ~(1 << LCD_RS); // RS = 0
	LCD_PUERTO |= (1 << LCD_E);   // E = 1
	_delay_ms(1);
	LCD_PUERTO &= ~(1 << LCD_E);  // E = 0
	_delay_ms(10);
}

void ENVIA_DATO(char dato)
{
	char dto_enviar;
	dto_enviar = (dato & 0xF0); // nibble alto
	LCD_PUERTO = dto_enviar;
	LCD_PUERTO |= (1 << LCD_RS) | (1 << LCD_E); // RS = 1 E = 1
	_delay_ms(1);
	LCD_PUERTO &= ~(1 << LCD_E); // E = 0
	LCD_PUERTO &= ~(1 << LCD_RS); // RS = 0
	_delay_ms(10);
	dto_enviar = ((dato << 4) & 0xF0); // nibble bajo
	LCD_PUERTO = dto_enviar;
	LCD_PUERTO |= (1 << LCD_RS) | (1 << LCD_E); // RS = 1 E = 1
	_delay_ms(1);
	LCD_PUERTO &= ~(1 << LCD_E); // E = 0
	LCD_PUERTO &= ~(1 << LCD_RS); // RS = 0
	_delay_ms(10);
}

void ENVIA_CADENA(char *dato)
{
	int i;
	for (i = 0; dato[i] != '\0'; i++)
	ENVIA_DATO(dato[i]);
}

void LIMPIA_LCD(void)
{
	ENVIA_CMD(0x01);
}

void CURSOR_DER(void)
{
	ENVIA_CMD(0x1C);
}

void CURSOR_IZQ(void)
{
	ENVIA_CMD(0x18);
}

void REGRESO_CASA(void)
{
	ENVIA_CMD(0x02);
}

void POS_LINEA1(char posicion)
{
	char linea1 = 0x80;
	posicion = posicion | linea1;
	ENVIA_CMD(posicion);
}

void POS_LINEA2(char posicion)
{
	char linea2 = 0xC0;
	posicion = posicion | linea2;
	ENVIA_CMD(posicion);
}

void POS_LINEA3(char posicion)
{
	char linea3 = 0x94;
	posicion = posicion | linea3;
	ENVIA_CMD(posicion);
}

void POS_LINEA4(char posicion)
{
	char linea4 = 0xD4;
	posicion = posicion | linea4;
	ENVIA_CMD(posicion);
}

void CURSOR_PARPADEO(void)
{
	ENVIA_CMD(0x0F);
}

void DESPLAZAMIENTO_PIZQUIERDA(void)
{
	ENVIA_CMD(0b00000111);
}

void SIN_DESPLAZAMIENTO_INCREMENTO(void)
{
	ENVIA_CMD(0x06);
}

//=============================
//==CARACTERES ESPECIALES======
//=============================

//GUISANTE
void CAR_ESP0(void){
	ENVIA_CMD(0x40);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);//11111
	_delay_ms(1); //1 1 1
	ENVIA_DATO(0b00001110);//10101
	_delay_ms(1); //11111
	ENVIA_DATO(0b00001010);//01010
	_delay_ms(1); //10101
	ENVIA_DATO(0b00001011);//11111
	_delay_ms(1);
	ENVIA_DATO(0b00001111);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00000110);
	_delay_ms(1);
	ENVIA_DATO(0b00000001);
	_delay_ms(1);
}

//Zombi Cubeta
void CAR_ESP1(void){
	ENVIA_CMD(0x48);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00011100);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
}
//Carnivora
void CAR_ESP2(void){
	ENVIA_CMD(0x50);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00001111);
	_delay_ms(1);
	ENVIA_DATO(0b00011000);
	_delay_ms(1);
	ENVIA_DATO(0b00001111);
	_delay_ms(1);
	ENVIA_DATO(0b00001111);
	_delay_ms(1);
	ENVIA_DATO(0b00001100);
	_delay_ms(1);
	ENVIA_DATO(0b00000011);
	_delay_ms(1);
}
//GIRASOL
void CAR_ESP3(void){
	ENVIA_CMD(0x58);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00010100);
	_delay_ms(1);
	ENVIA_DATO(0b00001000);
	_delay_ms(1);
	ENVIA_DATO(0b00010100);
	_delay_ms(1);
	ENVIA_DATO(0b00000111);
	_delay_ms(1);
	ENVIA_DATO(0b00000011);
	_delay_ms(1);
}
//PROYECTIL
void CAR_ESP4(void){
	ENVIA_CMD(0x60);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00001100);
	_delay_ms(1);
	ENVIA_DATO(0b00001100);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
}

//Zombi basico
void CAR_ESP5(void){
	ENVIA_CMD(0x68);
	_delay_ms(1);
	ENVIA_DATO(0b00000010);
	_delay_ms(1);
	ENVIA_DATO(0b00000111);
	_delay_ms(1);
	ENVIA_DATO(0b00000101);
	_delay_ms(1);
	ENVIA_DATO(0b00000111);
	_delay_ms(1);
	ENVIA_DATO(0b00000010);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00000010);
	_delay_ms(1);
	ENVIA_DATO(0b00000010);
	_delay_ms(1);
}

//NUEZ
void CAR_ESP6(void){
	ENVIA_CMD(0x70);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00011111);
	_delay_ms(1);
	ENVIA_DATO(0b00010101);
	_delay_ms(1);
	ENVIA_DATO(0b00011111);
	_delay_ms(1);
	ENVIA_DATO(0b00011111);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
}

//Zombi oleada
void CAR_ESP7(void){
	ENVIA_CMD(0x78);
	_delay_ms(1);
	ENVIA_DATO(0b00000010);
	_delay_ms(1);
	ENVIA_DATO(0b00000101);
	_delay_ms(1);
	ENVIA_DATO(0b00010111);
	_delay_ms(1);
	ENVIA_DATO(0b00010111);
	_delay_ms(1);
	ENVIA_DATO(0b00011110);
	_delay_ms(1);
	ENVIA_DATO(0b00000010);
	_delay_ms(1);
	ENVIA_DATO(0b00000010);
	_delay_ms(1);
	ENVIA_DATO(0b00000010);
	_delay_ms(1);
}
