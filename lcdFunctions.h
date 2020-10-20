/* Resumo do programa:
 *  header do programa lcdFunctions*/

#include <msp430.h>
#include <stdint.h>


void lcdWriteNibble(uint8_t nibble, uint8_t isChar);

void lcdWriteByte(uint8_t byte, uint8_t isChar);

uint8_t lcdReadNibble(uint8_t nibble, uint8_t isChar);

uint8_t lcdReadByte(uint8_t byte, uint8_t isChar);

void lcdBusy();

uint8_t lcdOcupado();

void lcdInit();

void lcdPrint(char *str);

void lcdChar(char caracter);

void lcdHex8(char value);

void lcdFloat1(float value);

void lcdCursor(uint8_t line, uint8_t column);
