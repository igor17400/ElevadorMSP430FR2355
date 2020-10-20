#include <msp430.h>
#include <stdint.h>
#include "configuracoes.h"

/*-- configuração --*/
void configPins(){

    /*------- configuração LEDs --------*/
    //configuracão do PWM
    //P5.0: DIR = 1
    //      OUT = 0
    P5DIR |= BIT0;
    P5OUT &= ~BIT0;
    /*------------------------------------*/

    /*------- configuração LEDs --------*/

    //configuracão do LED 1
    //P1.0: DIR = 1
    //      OUT = 0
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    //configuracão do LED 2
    //P6.6: DIR = 1
    //      OUT = 0
    P6DIR |= BIT6;
    P6OUT &= ~BIT6;

    /*------------------------------------*/

    /*------- configuração do sensor --------*/

    //P3.0: DIR = 0
    //      REN = 1
    //      OUT = 0
    P3DIR &= ~BIT0;
    P3REN |= BIT0;
    P3OUT &= ~BIT0; //pull down

    //P3.2: DIR = 0
    //      REN = 1
    //      OUT = 0
    P3DIR &= ~BIT2;
    P3REN |= BIT2;
    P3OUT &= ~BIT2; //pull down

    //P4.0: DIR = 0
    //      REN = 1
    //      OUT = 0
    P4DIR &= ~BIT0;
    P4REN |= BIT0;
    P4OUT &= ~BIT0; //pull down

    /*------------------------------------*/

    /*------- configuração botões --------*/

    //configuracão SW1
    //P4.1: DIR = 0
    //      REN = 1
    //      OUT = 1
    P4DIR &= ~BIT1;
    P4REN |= BIT1;
    P4OUT |= BIT1; //pull up

    //configuracão SW2
    //P2.3: DIR = 0
    //      REN = 1
    //      OUT = 1
    P2DIR &= ~BIT3;
    P2REN |= BIT3;
    P2OUT |= BIT3; //pull up

    //configuracão P2.0
    //P2.0: DIR = 0
    //      REN = 0 --> pull down externo
    P2DIR &= ~BIT0;
    P2REN &= ~BIT0;

    //configuracão P2.1
    //P2.1: DIR = 0
    //      REN = 0 --> pull down externo
    P2DIR &= ~BIT1;
    P2REN &= ~BIT1;

    //configuracão P2.2
    //P2.1: DIR = 0
    //      REN = 0 --> pull down externo
    P2DIR &= ~BIT2;
    P2REN &= ~BIT2;


    /*------------------------------------*/

    /*------- configuração motor --------*/
    //configuracão P4.6 --> elevador desce
    //P4.6: DIR = 0
    //      REN = 0 --> pull down externo
    P4DIR |= BIT6;
    P4OUT &= ~BIT6;

    //configuracão P4.7 --> elevador sube
    //P4.7: DIR = 0
    //      REN = 0 --> pull down externo
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
    /*------------------------------------*/


    /*------- configuração 7Segment --------*/

    //configuracão P2.5 --> saida
    P2DIR |= BIT5;
    P2OUT &= ~BIT5;

    //configuracão P2.4 --> saida
    P2DIR |= BIT4;
    P2OUT &= ~BIT4;

    //configuracão P3.7 --> saida
    P3DIR |= BIT7;
    P3OUT &= ~BIT7;

    //configuracão P6.4 --> saida
    P6DIR |= BIT4;
    P6OUT &= ~BIT4;

    //configuracão P6.3 --> saida
    P6DIR |= BIT3;
    P6OUT &= ~BIT3;

    //configuracão P6.2 --> saida
    P6DIR |= BIT2;
    P6OUT &= ~BIT2;

    //configuracão P6.1 --> saida
    P6DIR |= BIT1;
    P6OUT &= ~BIT1;

    /*------------------------------------*/
}







