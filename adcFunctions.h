/*
 * adcFunctions.h
 *
 *  Created on: Nov 30, 2019
 *      Author: igorazevedo
 */

#ifndef ADCFUNCTIONS_H_
#define ADCFUNCTIONS_H_

/* variáveis globais */
uint8_t adcFinished;
uint16_t *dataPtr;
uint16_t *dataPtr2;
uint16_t *temp;
uint8_t flag_adc;
uint8_t pino_escolhido;

void adcConfig(uint8_t pin);
int mediaVar(uint16_t *ptr, uint8_t inicio, uint8_t final);
void lcdShow(uint16_t *dataPtr);
void adcPrint();


#endif /* ADCFUNCTIONS_H_ */
