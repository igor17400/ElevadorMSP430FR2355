/*
 * adcFunctions.c
 *
 *  Created on: Nov 30, 2019
 *      Author: igorazevedo
 *
 *
 *  Explicação: O motivo para estarmos usando os pinos P1.6 e P1.7
 */


#include <msp430.h>
#include <stdint.h>
#include "lcdFunctions.h"
#include "i2cFunctions.h"
#include "adcFunctions.h"


/*------------------ código do ADC ----------------------*/


/* Pinos:
 * P1.4 -> A4
 * P1.5 -> A5
 * */
void adcConfig(uint8_t pin){

    ADCCTL0 = ADCON; //modo rajada => 1 gatilho, N conversões
    ADCCTL1 = ADCSHS_2 | ADCCONSEQ_2;
    ADCCTL2 = ADCRES_0;                         // resolução de 12 bits

    ADCMCTL0 = pin;                             // pino escolhido pelo usuário
    P1SEL0 |= 1 << pin;                         // entrada BITn
    P1SEL1 |= 1 << pin;                         // configuração do pino dada entrada do usuário

    ADCCTL0 |= ADCENC;                          // enable conversion
    ADCIE = ADCIE0;

    TB1CTL = MC__UP | TBCLR | TBSSEL__SMCLK;

    // N0 = tempo/Tclk
    TB1CCR0 = 10484; //A cada 0.01 segundos
    TB1CCR1 = 1362;

    TB1CCTL1 = OUTMOD_7;
}

int mediaVar(uint16_t *ptr, uint8_t inicio, uint8_t final){
    volatile uint8_t i = 0;
    volatile int total = 0;

    for(i = inicio; i < final; i = i + 1){
        total += ptr[i];
    }

    return total>>3;
}

void lcdShowMedia(uint16_t *dataPtr){

    uint8_t i;
    float media0 = 0, media1 = 0;
    float media0_float = 0, media1_float = 0;

    media0 = mediaVar(dataPtr, 0, 8);
    media0_float = 3.3*(media0/255);

    media1 = mediaVar(dataPtr, 8, 16);
    media1_float = 3.3*(media1/255);

    media0_float = media0_float - media1_float;

    lcdCursor(2, 8);
    lcdPrint("Te:");
    lcdCursor(2, 11);
    lcdFloat1(media0_float);

    lcdCursor(2, 16);
    lcdChar('V');

}

/*-------------------------------------------------------*/

/*------------------ interrupção do ADC ----------------------*/
uint8_t flag_canal = 1;
uint8_t coringa = 0;
#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(){
    static uint8_t dataCnt = 0;

    *(dataPtr + dataCnt) = ADCMEM0;
    if(dataCnt == 7 || dataCnt == 15){
        if(dataCnt == 15){
            adcFinished = 1;
            dataCnt = 0;
        }
        if(pino_escolhido == 5){
            pino_escolhido = 4;
            adcConfig(pino_escolhido);
        }else{
            pino_escolhido = 5;
            adcConfig(pino_escolhido);
        }
    }
    dataCnt++;
}




