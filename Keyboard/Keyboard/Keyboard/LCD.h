#include <inttypes.h>
#include <avr/io.h>

#define LCD_PUERTO PORTC //Define el Puerto al cual van a ser conectados los pines del LCD
#define LCD_RS PC2 //Define el pin del Microcontrolador al cua se conecta el pin RS del LCD
#define LCD_E PC3 //Define el pin del Microcontrolador al cual se conecta el pin E del LCD


//Prototipos de fnción para LCD en modo de 4 bits

void LCD_INICIALIZA(); //Inicializa el LCD
void ENVIA_CMD(char dato); //Función para envío de comandos
void ENVIA_DATO(char dato);
void ENVIA_CADENA(char *dato);
void LIMPIA_LCD(); //Limpia la pantalla del LCD
void CURSOR_DER();
void CURSOR_IZQ();
void REGRESO_CASA();
void POS_LINEA1(char posicion);
void POS_LINEA2(char posicion);
void POS_LINEA3(char posicion);
void POS_LINEA4(char posicion);
void CAR_ESP0();
void CAR_ESP1();
void CAR_ESP2();
void CAR_ESP3();
void CAR_ESP4();
void CAR_ESP5();
void CAR_ESP6();
void CAR_ESP7();
void animacion();
void CURSOR_PARPADEO();
void DESPLAZAMIENTO_PIZQUIERDA();
void SIN_DESPLAZAMIENTO_INCREMENTO();



