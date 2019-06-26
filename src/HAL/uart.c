// ------------------------------------- INCLUDES -------------------------------------
#include "msp430.h"
#include <stdint.h>
#include <HAL/uart.h>
#include <HAL/timers.h>
#include <Modules/buttons.h>

// ------------------------------------- DEFINES --------------------------------------

//PORT 4
#define UART_TX_BIT             BIT4
#define UART_RX_BIT             BIT5

#define TXD1_READY (UCA1IFG & UCTXIFG)


// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PRIVATE VARS  ----------------------------------

byte DatoLeido_UART;
uint8_t Byte_Recibido;

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------

void TxUAC1(byte TXData);

// ----------------------------------- PRIVATE METHODS --------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------

/* funció TxUAC1(byte): envia un byte de dades per la UART 0 */
void uart_send_byte(byte TXData) {
    while(!TXD1_READY); // Espera a que estigui preparat el buffer de transmissió
    UCA1TXBUF = TXData;
}


uint8_t uart_isByte_Recibido(void) {

    return Byte_Recibido;
}

byte uart_getDatoLeido(void) {
    Byte_Recibido = 0;
    return DatoLeido_UART;
}


void uart_init(void) {

    UCA1CTL1 |= UCSWRST;    //Fem un reset de la USCI i queda "inoperativa"
    UCA1CTL0 = 0x00;        //UCSYNC=0 mode asíncron
                            //UCMODEx=0 seleccionem mode UART
                            //UCSPB=0 nomes 1 stop bit
                            //UC7BIT=0 8 bits de dades
                            //UCMSB=0 bit de menys pes primer
                            //UCPAR=x no es fa servir bit de paritat
                            //UCPEN=0 sense bit de paritat
    UCA1CTL1 |= UCSSEL__SMCLK;  //Triem SMCLK (16MHz) com a font del clock BRCLK
    UCA1BR0 = 1;            //Prescaler de BRCLK fixat a 1 (LO byte = 1, ...
    UCA1BR1 = 0;            //... HI byte = 0).
    UCA1MCTL = UCOS16;      // No necessitem modulació (divisió entera), però
                            // si oversampling => bit 0 = UCOS16 = 1.
                            // El Baud rate és BRCLK/16=1MBps
    P4SEL |= UART_RX_BIT | UART_TX_BIT;   //I/O funció: P4.4 = UART0TX, P4.5 = UART0RX
    P4REN |= UART_RX_BIT;          //amb resistència activada de pull-up l’entrada P4.5
    P4OUT |= UART_RX_BIT;          //

    P4DIR |= UART_DATA_DIR;          //Port P4.7 com sortida (Data Direction: selector Tx/Rx)
    P4SEL &= ~UART_DATA_DIR;         //Port 4.7 com I/O digital
    P4OUT &= ~UART_DATA_DIR;         //Inicialitzem el port P4.7 a 0 (Sentit Dades Rx)
    UCA1CTL1 &= ~UCSWRST;   //Reactivem la línia de comunicacions sèrie
    UCA1IE |= UCRXIE;    //Això només s’ha d’activar quan tinguem la rutina de recepció
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) { //interrupcion de recepcion en la uart A0
    UCA1IE &= ~UCRXIE; //Interrupciones desactivadas en RX
    DatoLeido_UART = UCA1RXBUF;
    Byte_Recibido = 1;
    UCA1IE |= UCRXIE; //Interrupciones reactivadas en RX
}

