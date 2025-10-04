#include <avr/io.h>

#define FIL1 0X0F & ~(0X01) ////0X0E
#define FIL2 0X0F & ~(0X02) ////0X0D
#define FIL3 0X0F & ~(0X04) ////0X0B
#define FIL4 0X0F & ~(0X08) ////0X07
#define COL1 0XEF//0XFF & ~(0X10) ////0XEF
#define COL2 0XDF//0XFF & ~(0X20) ////0XDF
#define COL3 0XBF//0XFF & ~(0X40) ////0XBF
#define COL4 0X7F//0XFF & ~(0X80) ////0X7F

//PROTOTIPOS DE FUNCIÓN


char LEE_TECLADO();
void BARRE_TECLADO();


