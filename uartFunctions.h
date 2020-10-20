/*
 * uartFunctions.h
 *
 *  Created on: Dec 2, 2019
 *      Author: igorazevedo
 */

#ifndef UARTFUNCTIONS_H_
#define UARTFUNCTIONS_H_

#include <stdint.h>

struct {
    uint8_t data;   // dado que eu quero enviar ou receber bytes
} UCA0;

void uartConfigPin();

void uartConfig(uint32_t baudrate, uint8_t isLSBfirst, uint8_t parity, uint8_t nStops);

void uartSend(uint8_t byte);

uint8_t uartGet();




#endif /* UARTFUNCTIONS_H_ */
