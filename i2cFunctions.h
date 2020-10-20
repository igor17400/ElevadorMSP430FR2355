/* Resumo do programa:
 *  header do programa i2cFunctions*/

#include <msp430.h>
#include <stdint.h>

struct{
    uint8_t status; // status da interface --> se houve ou não ACK
    uint8_t data;   // dado que eu quero enviar ou receber bytes
}UCB0;

void i2cInit(uint8_t isMaster, uint8_t myAddr);

uint8_t i2cTst(uint8_t addr);

uint8_t i2cPut(uint8_t addr, uint8_t data);

uint8_t i2cGet(uint8_t addr, uint8_t *data);

uint8_t i2cGetnBytes(uint8_t addr, uint8_t *data, uint8_t count);
