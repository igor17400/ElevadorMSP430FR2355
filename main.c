/* TRABALHO FINAL
 * */
#include <msp430.h>
#include <stdint.h>
#include "i2cFunctions.h"
#include "lcdFunctions.h"
#include "configuracoes.h"
#include "elevador.h"
#include "adcFunctions.h"
#include "uartFunctions.h"


uint8_t recebeuHelp(char *rPtr){
    uint8_t count = 0; //count == 4 --> recebeu help
    while(*rPtr != '\0'){
        if(count == 4)
            return 1;
        if(*rPtr == 'h' || *rPtr == 'e' || *rPtr == 'l' || *rPtr == 'p' )
            count++;
    }

    return 0;
}


void debounce(volatile uint16_t dt){
    while(dt--);
    return;
}

typedef struct EstadoElevadorStruct{
    uint8_t andar_atual; //0, 1, 2
    uint8_t andar_destino; //0, 1, 2
    uint8_t isMoving; //0 - parado ou 1 - movendo
    uint8_t up_down; //0 - Nada // 1 - up // 2 - down
} EstadoElevadorStruct;

struct EstadoElevadorStruct EstadoElevador;

void escreverLCD(uint8_t up_down, uint8_t andar_destino){

    lcdCursor(1, 1);
    lcdPrint("Estado:");
    lcdCursor(1, 8);
    if(up_down == 1){
        lcdPrint(" Sub");
    } else if(up_down == 2){
        lcdPrint(" Des");
    } else if(up_down == 0){
        lcdPrint(" Nd ");
    }

    lcdCursor(2, 1);
    lcdPrint("DST: ");
    lcdCursor(2, 6);
    if(andar_destino == 3){
        lcdPrint("- ");
    } else {
        lcdInt(andar_destino);
    }
}

void pararMotor(){
    P1OUT &= ~BIT1;
    P6OUT &= ~BIT6;

    P4OUT &= ~BIT6;
    P4OUT &= ~BIT7;
//    escreverLCD(EstadoElevador.up_down, EstadoElevador.andar_destino);
}



//global
char resposta[30] = {0,};               // Aloca espaço para a resposta
char *rPtr = &resposta[0];                  // Usa ponteiro móvel para receber
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);

    i2cInit(1, 0);
    lcdInit();

    /*configuração*/
    configPins();

    /* variáveis */
    uint8_t count = 0;
    uint16_t dt = 1000;
    EstadoElevador.andar_atual = 0;
    EstadoElevador.andar_destino = 0;
    EstadoElevador.up_down = 0;

    uint16_t inputData[16];
    dataPtr = &inputData[0];


    TB0CTL = TBSSEL__ACLK  | // clock -> ACLK
            MC__UP         | // modo de contagem -> UP
            TBIE           | // Timer_B interrupt enable
            TBCLR; // clear
    TB0CCTL1 = CCIE;
    TB0CCR0 = 16383;


    pino_escolhido = 5;
    adcConfig(pino_escolhido);

    __enable_interrupt();

    while(!adcFinished);
        lcdShowMedia(dataPtr);


    andarElevador(EstadoElevador.andar_atual);
//    escreverLCD(EstadoElevador.up_down, EstadoElevador.andar_destino);
    while(1){
        escreverLCD(EstadoElevador.up_down, EstadoElevador.andar_destino);
        debounce(dt);
        while( !(P2IN & BIT0) && !(P2IN & BIT1) && !(P2IN & BIT2) );
        while(!adcFinished);
            lcdShowMedia(dataPtr);

        //se um dos dois botões for pressionado o while quebra

        //se o botão 0 foi apertado
        if( P2IN & BIT0 && (EstadoElevador.isMoving == 0)){
            EstadoElevador.isMoving = 1;
            EstadoElevador.andar_destino = 0;
            if(EstadoElevador.andar_atual == 1){
                P1OUT |= BIT1;
                P6OUT &= ~BIT6;

                P4OUT |= BIT6;
                P4OUT &= ~BIT7;
            } else if(EstadoElevador.andar_atual == 2){
                P1OUT |= BIT1;
                P6OUT &= ~BIT6;

                P4OUT |= BIT6;
                P4OUT &= ~BIT7;
            }
            EstadoElevador.up_down = 2; //descendo
        }

        //se o botão 1 foi apertado
        if( P2IN & BIT1 && (EstadoElevador.isMoving == 0)){
            EstadoElevador.isMoving = 1;
            EstadoElevador.andar_destino = 1;
            if(EstadoElevador.andar_atual == 0){
                P1OUT &= ~BIT1;
                P6OUT |= BIT6;

                P4OUT &= ~BIT6;
                P4OUT |= BIT7;
                EstadoElevador.up_down = 1; //subindo
            } else if(EstadoElevador.andar_atual == 2){
                P1OUT |= BIT1;
                P6OUT &= ~BIT6;

                P4OUT |= BIT6;
                P4OUT &= ~BIT7;
                EstadoElevador.up_down = 2; //descendo
            }
        }

        //se o botão 2 foi apertado
        if( P2IN & BIT2 && (EstadoElevador.isMoving == 0)){
            EstadoElevador.isMoving = 1;
            EstadoElevador.andar_destino = 2;
            if(EstadoElevador.andar_atual == 0){
                P1OUT &= ~BIT1;
                P6OUT |= BIT6;

                P4OUT &= ~BIT6;
                P4OUT |= BIT7;
            } else if(EstadoElevador.andar_atual == 1){
                P1OUT &= ~BIT1;
                P6OUT |= BIT6;

                P4OUT &= ~BIT6;
                P4OUT |= BIT7;
            }
            EstadoElevador.up_down = 1; //subindo
        }
    }

    return 0;
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TB0_CCRn_ISR(){

    switch(TB0IV){
        case 0x0E: //overflow

            if(!((P3IN & BIT0) == 0) && (EstadoElevador.andar_destino == 0) ){ //andar 0
                EstadoElevador.andar_atual = 0;
                andarElevador(EstadoElevador.andar_atual);
                EstadoElevador.andar_destino = 3;
                EstadoElevador.isMoving = 0;
                EstadoElevador.up_down = 0;
                pararMotor();
            }

            if( !((P4IN & BIT0) == 0) ){ //andar 1

                if((EstadoElevador.andar_destino == 1)){
                    EstadoElevador.andar_destino = 3;
                    EstadoElevador.up_down = 0;
                    pararMotor();
                }

                EstadoElevador.andar_atual = 1;
                andarElevador(EstadoElevador.andar_atual);
                EstadoElevador.isMoving = 0;

            }

            if( !((P3IN & BIT2) == 0) && (EstadoElevador.andar_destino == 2) ){ //andar 2
                EstadoElevador.andar_atual = 2;
                andarElevador(EstadoElevador.andar_atual);
                EstadoElevador.andar_destino = 3;
                EstadoElevador.isMoving = 0;
                EstadoElevador.up_down = 0;
                pararMotor();
            }
            break;
    }
}







