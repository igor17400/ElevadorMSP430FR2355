/* Resumo do programa:
 *  configuração e funções do LCD*/

#include <msp430.h>
#include <stdint.h>
#include "lcdFunctions.h"
#include "i2cFunctions.h"

const uint16_t cursorMap[2][16] = { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
                                    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF };

/*------------------ código do LCD ----------------------*/

uint8_t lcdAddr = 0x27;
const uint8_t lcdRS = 0x01;
const uint8_t lcdRW = 0x02;
const uint8_t lcdEN = 0x04;
const uint8_t lcdBL = 0x08; //Backlight

void lcdWriteNibble(uint8_t nibble, uint8_t isChar){
     nibble <<= 4;                  // Ajusta a posição do nibble
     nibble |= lcdBL | isChar;      // Backlight on

     i2cPut(lcdAddr, nibble | 0);         // Gera pulso
     i2cPut(lcdAddr, nibble | lcdEN);     // em Enable
     i2cPut(lcdAddr, nibble | 0);         //
}

void lcdWriteByte(uint8_t byte, uint8_t isChar){
    lcdWriteNibble(byte >> 4, isChar);
    lcdWriteNibble(byte     , isChar);
}

uint8_t lcdReadNibble(uint8_t nibble, uint8_t isChar){

    nibble |= lcdBL | isChar;
    uint8_t aux;

    i2cPut(lcdAddr, nibble | 0);        // Gera pulso
    i2cPut(lcdAddr, nibble | lcdRW);    // em Enable
    i2cPut(lcdAddr, nibble | lcdEN);    // em Enable
    i2cGet(lcdAddr, &aux);
    i2cPut(lcdAddr, nibble | 0);

    return aux;
}

uint8_t lcdReadByte(uint8_t byte, uint8_t isChar){
    uint8_t lsNibble = lcdReadNibble(byte >> 4, isChar);
    uint8_t msNibble = lcdReadNibble(byte     , isChar);
    msNibble <<= 4;

    byte = lsNibble | msNibble;

    return byte;
}

void lcdBusy(){
//    while(lcdReadByte(byte, isChar) & 0x80);
}

uint8_t lcdOcupado(){
    if(i2cTst(lcdAddr) == 0){
        return 0; //LCD disponivel
    }
    return 1; //LCD indisponivel
} /* lcdOcupado */


void lcdInit(){
    //detecção automatica de qual LCD estou usando pelo endereço lcdAddr default
    if(i2cTst(lcdAddr))
        lcdAddr = 0x27;

    lcdWriteNibble(0x3, 0);   // 1)
    lcdWriteNibble(0x3, 0);   // 1)  (1) Entra em modo 8 bits
    lcdWriteNibble(0x3, 0);   // 1)
    lcdWriteNibble(0x2, 0);   // Entra em modo 4 bits

    lcdWriteByte(0x28, 0);   // Configura: Duas linhas
    lcdWriteByte(0x0C, 0);   // Display ON, Cursor ON
    lcdWriteByte(0x06, 0);   // Modo de incremento

    lcdWriteByte(0x01, 0);   // Limpa a tela
    while(lcdOcupado());     // Espera ficar disponível
}

void lcdPrint(char *str){
    while(*str)
        lcdWriteByte(*str++, 1);
}

void lcdChar(char caracter){
    lcdWriteByte(caracter, 1);
}

void lcdInt(uint8_t num){
    char caracter = num + 0x30; //tabela ascii para 0
    lcdWriteByte(caracter, 1);
}

void lcdHex8_0X(char value){
    uint8_t i = 0;
    uint8_t conv[2];
    char string[5];
    string[0] = '0';
    string[1] = 'x';

    conv[0] = (value & 0xF0) >> 4;  //MSB
    conv[1] = value & 0x0F;         //LSB
    for (i = 0; i < 2; i++) {

        if ((conv[i] >= 0x00) && (conv[i] <= 0x09)) {
            conv[i] += 0x30;
        }

        if ((conv[i] >= 0x0A) && (conv[i] <= 0x0F)) {
            conv[i] += 0x37;
        }
    }
    string[2] = conv[0];
    string[3] = conv[1];
    string[4] = '\0';
    lcdPrint(string);
}

void lcdHex8(char value){
    uint8_t i = 0;
    uint8_t conv[2];
    char string[3];

    conv[0] = (value & 0xF0) >> 4;  //MSB
    conv[1] = value & 0x0F;         //LSB
    for (i = 0; i < 2; i++) {

        if ((conv[i] >= 0x00) && (conv[i] <= 0x09)) {
            conv[i] += 0x30;
        }

        if ((conv[i] >= 0x0A) && (conv[i] <= 0x0F)) {
            conv[i] += 0x37;
        }
    }
    string[0] = conv[0];
    string[1] = conv[1];
    string[2] = '\0';
    lcdPrint(string);
}

void lcdHex16(int value){
    int value2 = value>>8;
    value = value & 0x00FF;
    lcdHex8(value2);
    lcdHex8(value);
}


void lcdFloat1(float value){
    uint8_t conv[4];
    uint8_t i = 0;
    //lógica de conversão
    for (i = 0 ; i < 4; i++){
        conv[i] = value/1;
        value = value - conv[i];
        value *= 10;
    }

    char string[7];
    string[0] = conv[0] + 0x30; //tabela ascii para 0
    string[1] = '.';

    for(i = 2; i < 5; i++){
        string[i] = conv[i-1] + 0x30;
    }
    string[6] = '\0';

    lcdPrint(&string[0]);
}

void lcdCursor(uint8_t line, uint8_t column){
    lcdWriteByte(cursorMap[line-1][column-1], 0);
}

/*-------------------------------------------------------*/
