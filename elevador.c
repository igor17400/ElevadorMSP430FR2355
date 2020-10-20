/*
 * elevador.c
 *
 *  Created on: Nov 28, 2019
 *      Author: igorazevedo
 */

#include <msp430.h>
#include <stdint.h>
#include "i2cFunctions.h"
#include "lcdFunctions.h"
#include "configuracoes.h"

void acenderZero(){
    P2OUT |= BIT5;
    P2OUT |= BIT4;
    P3OUT |= BIT7;
    P6OUT |= BIT4;
    P6OUT |= BIT3;
    P6OUT |= BIT2;
    P6OUT &= ~BIT1;
}

void acenderUm(){
    P2OUT &= ~BIT5;
    P2OUT &= ~BIT4;
    P3OUT |= BIT7;
    P6OUT &= ~BIT4;
    P6OUT |= BIT3;
    P6OUT &= ~BIT2;
    P6OUT &= ~BIT1;
}

void acenderDois(){
    P2OUT |= BIT5;
    P2OUT |= BIT4;
    P3OUT &= ~BIT7;
    P6OUT |= BIT4;
    P6OUT |= BIT3;
    P6OUT &= ~BIT2;
    P6OUT |= BIT1;
}

void andarElevador(uint8_t andar){
    if(andar == 0){
        acenderZero();
    } else if(andar == 1){
        acenderUm();
    } else if(andar == 2){
        acenderDois();
    }
}


