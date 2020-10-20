/*
 * uartFunctions.c
 *
 *  Created on: Dec 2, 2019
 *      Author: igorazevedo
 */

/* Estamos nos comunicando com um Arduino, por isso é crucial definir os padrões de comunicação
 * */


#include <msp430.h>
#include <stdint.h>

void uartConfigPin(){
    P1SEL0 |= BIT6;
    P1SEL1 &= ~BIT6;

    P1SEL0 |= BIT7;
    P1SEL1 &= ~BIT7;
}

void uartConfig(uint32_t baudrate, uint8_t isLSBfirst, uint8_t parity, uint8_t nStops){

    UCA0CTLW0 = UCSSEL__SMCLK |            // Escolhe SMCLK
//               UCPEN         |            // Habilita paridade
//               UCPAR         |            // Paridade - 0:impar, 1:par
//               UCMSB         |            // MSB first
//               UCSPB         |            // nStops - 0: 1 stop, 1: 2 stops
                 UCMODE_0      |            // Modo: UART
                 UCSWRST;                   // RST para configurar


    if(!isLSBfirst)
        UCA0CTLW0 |= UCMSB; //MSB first select. Controls the direction of the receive and transmit shift register.
    if(parity){
        UCA0CTLW0 |= UCPEN;
        if(parity != 1){
            UCA0CTLW0 |= UCPAR; //Parity select. UCPAR is not used when parity is disabled.
        }
    }

    if(nStops == 2){
        UCA0CTLW0 |= UCSPB;
    }

    P1SEL0 |= (BIT6 | BIT7);
    P1SEL1 &= ~(BIT6 | BIT7);

    UCA0BRW = 1048756/baudrate;

    if(UCA0BRW > 16){
        UCA0BRW /= 16;
        UCA0MCTLW = UCOS16;
    }

    uint32_t BRF = (((1048756<<4)/baudrate)/16);
    BRF &= 0x0F; //filtrando a parte menos significativa
    UCA0MCTLW |= BRF << 4;

    /* o cálculo de BRF precisa ser feito na mão e olhando a tabela do User guide*/
    uint32_t BRS = 0x22;
    UCA0MCTLW |= BRS << 8;


    UCA0CTLW0 &= ~UCSWRST;
}

void uartSendByte(uint8_t byte){
    while(!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = byte;
}

void uartSendWord(char *rPtr){
    while(*rPtr){
        while(!(UCA0IFG & UCTXIFG));
        UCA0TXBUF = *rPtr++;
    }
}

uint8_t uartGetByte(){
    while(!(UCA0IFG & UCRXIFG));
    return UCA0RXBUF;
}

void uartGetWord(char *rPtr){

    uint8_t count = 0;

    // Recebe a resposta
    do {
        while(!(UCA0IFG & UCRXIFG));
        *rPtr = UCA0RXBUF;
    } while(*rPtr++ != '\n');
}





