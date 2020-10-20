/* Resumo do programa:
 *  configuração e funções de I2C*/

#include <msp430.h>
#include <stdint.h>
#include "i2cFunctions.h"

/*------------------ código do I2C ----------------------*/
/* Inicializa a interface de comunicação serial I2C */
void i2cInit(uint8_t isMaster, uint8_t myAddr){


    UCB0CTLW0 = UCMODE_3 | UCSWRST; // Modo: I2C
    UCB0CTLW1 = UCASTP_2;           // com stop automático
    UCB0TBCNT = 1;                  // para recepção de 1 byte

    if(isMaster){ // Se for mestre:

        UCB0CTLW0 |= UCSSEL__SMCLK | UCMST; // configura como mestre, SMCLK
        UCB0BRW = 500; //bold rate divider -> estamos dividindo o SMCLK em 10 vezes
        // estamos operando em 1MHZ/10 = 100 kHz
        // SCL @ 100kHz

    } else { // Se for escravo:

        UCB0I2COA0 = UCOAEN | myAddr; // configura o endereço
        //"I2C on address"

    }

     /*--- configuração dos pinos ---*/

    //OBS:  |
    //------  //estamos fingindo que esse pinos são de entradas para que
              //  possamos setar os resistores de pull-up dentro da placa ao invés de
              //  fazer essa ligação de fora.

    //P1.2 --> UCB0SDA
    //P1.2SEL = 01
    P1SEL0 |= BIT2;
    P1SEL1  &= ~BIT3;
    P1DIR &= ~BIT2;
    P1REN |= BIT2;
    P1OUT |= BIT2;

    //P1.3 --> UCB0SCL
    //P1.3SEL = 01
    P1SEL0 |= BIT3;
    P1SEL1 &= ~BIT3;
    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;

    UCB0CTLW0 &= ~UCSWRST; //estamos resetando o RST

    // Interrupções:
    UCB0IE = UCRXIE     |       // RX --> recepção
             UCTXIE     |       // TX --> transmissão
             UCSTPIE    |       // Stop
             UCNACKIE;          // NACK
}

// queremos testar se o endereço está no barramento ou não
uint8_t i2cTst(uint8_t addr){
    UCB0.status = 0;                // Consideramos que a transmissão
                                    //   será bem sucedida

    UCB0I2CSA = addr;                       // Configura end. do escravo
    UCB0CTLW0 |= UCTR + UCTXSTT + UCTXSTP;  //   e pede start e stop simultâneos
    // Ao setarmos um start e um stop simultaneamente, não teremos transmissão de dados
    // Esse tipo de comando serve somente para testar se o endereço está no barramento ou não

    __low_power_mode_0();           // Aguarda o final da comunicação
    return UCB0.status;             //   para retornar 0:ACK ou 1:NACK
}


uint8_t i2cTstNoInterrup(uint8_t addr){
    UCB0IFG = 0; //Zera as flags, pois NACK n�o � zerado no start

    UCB0I2CSA = addr; //Configura end. do escravo
    UCB0CTLW0 |= UCTR + UCTXSTT; //Pede um start

    while(!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = 0x00; //Escreve um byte dummy para destravar o ciclo de ACK

    while(!(UCB0IFG & UCNACKIFG) && !(UCB0IFG & UCTXIFG)); //Espera ou NACK ou TXIFG

    UCB0CTLW0 |= UCTXSTP; //Pede um stop e aguarda ele ser enviado
    while(UCB0CTLW0 & UCTXSTP);

    return (UCB0IFG & UCNACKIFG); //Retorna ACK ou NACK
}


//retorna um código de sucesso se for 0, e se for insucesso um código de erro
//addr -> endereço do escravo
//data -> dados que eu quero enviar
uint8_t i2cPut(uint8_t addr, uint8_t data){
    UCB0.status = 0;                // Consideramos que a transmissão
                                    // será bem sucedida

    UCB0I2CSA = addr;               // Configura end. do escravo,
    UCB0.data = data;               // e copia byte para a interface

    UCB0CTLW0 |= UCTR + UCTXSTT;    // Pede inicio da comunicação

    __low_power_mode_0();           // Aguarda o final da comunicação
    return UCB0.status;             // para retornar 0:ACK ou 1:NACK
}

uint8_t i2cPutnBytes(uint8_t addr, uint8_t *data, uint8_t count){
    UCB0.status = 0;                // Consideramos que a transmissão
                                    // será bem sucedida

    UCB0I2CSA = addr;               // Configura end. do escravo,
    UCB0CTLW0 |= UCTR + UCTXSTT;    // Pede inicio da comunicação

    UCB0.data = *(data)++;
    count--;
    while(count--){
        UCB0.data = *(data)++;
    }

    __low_power_mode_0();           // Aguarda o final da comunicação
    return UCB0.status;             // para retornar 0:ACK ou 1:NACK
}

uint8_t i2cGet(uint8_t addr, uint8_t *data){
    UCB0.status = 0;                // Consideramos que a transmissão
                                    // será bem sucedida

    UCB0CTLW0 &= ~UCTR;             // Pede start
    UCB0CTLW0 |= UCTXSTT;           //      como receptor

    __low_power_mode_0();           // Aguarda o final da comunicação

    *data = UCB0.data;              // grava o byte no ponteiro do usuario

    return UCB0.status;             // retornar 0:ACK ou 1:NACK
}

uint8_t i2cGetnBytes(uint8_t addr, uint8_t *data, uint8_t count){
    UCB0.status = 0;                // Consideramos que a transmissão
                                    // será bem sucedida

    UCB0TBCNT = count;

    UCB0I2CSA = addr;               // Configura end. do escravo,

    UCB0CTLW0 &= ~UCTR;             // Pede start
    UCB0CTLW0 |= UCTXSTT;           //      como receptor

    __low_power_mode_0();           // Aguarda o final da comunicação

    while(count--){
        *data++ = UCB0.data;        //Realizamos a recepção de n-1 bytes
    }

    return UCB0.status;             // retornar 0:ACK ou 1:NACK
}


#pragma vector = USCI_B0_VECTOR
__interrupt void I2C_ISR(){
    switch(UCB0IV){             // Vetor de interrupções da eUSCI

        case UCIV__UCNACKIFG:       // Se recebeu um NACK
            UCB0CTLW0 |= UCTXSTP;   // Pede um stop
            UCB0.status = 1;        // E avise o usuário do NACK
            break;

        case UCIV__UCSTPIFG:                 // Se enviou um stop,
            __low_power_mode_off_on_exit();  // volta para o programa principal saindo
            break;                           //      do modo de baixo consumo

        case UCIV__UCRXIFG0:         //Se recebeu alguma coisa no buffer
            UCB0.data = UCB0RXBUF;   // de recepção, escreva no ponteiro do usuáro
            break;

        case UCIV__UCTXIFG0:        // Se for transmitir algo, pegue
            UCB0TXBUF = UCB0.data;  //  o byte da posição indicada pelo
            break;                  //  ponteiro do usuário

        default:
            break;
    }
}

/*--------------------------------------------------------------------*/
