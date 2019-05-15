/*
 * i2c_lcd.c
 *
 *  Created on: 2 may. 2019
 *      Author: jarmengol
 */


// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <string.h>
#include <HAL/timers.h>
#include <HAL/i2c.h>


// ------------------------------------- DEFINES --------------------------------------

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define I2C_LCD_TEXT_MAX_SIZE       17           //16 + 1 byte de commandament

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PRIVATE VARS  ----------------------------------


// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------

void i2c_lcd_reset(void);

// ----------------------------------- PRIVATE METHODS --------------------------------

void i2c_lcd_reset() {
    P6OUT &= ~(BIT0);
    wait_ms(2);
    P6OUT |= BIT0;
}

void i2c_init() {
    /* Inisialisation of reset pin */
    P6SEL &= ~(BIT0);
    P6DIR |= BIT0;
    P6OUT |= BIT0;


    P4SEL |= BIT2 + BIT1;           // P4.1 iP4.2 com a USCI sifem server USCI B1
    UCB1CTL1 |= UCSWRST;            // Aturem el mÃ²dul
    //El configurem com a master, sincroni mode i2c, per defecte, estÃ en single-mastermode
    UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;
    UCB1CTL1 = UCSSEL_2 + UCSWRST;  // UseSMCLK, keepSW resetUCB1BR0 = 160;              // fSCL= SMCLK(16MHz)/160 = ~100kHzUCB1BR1 = 0;UCB1CTL1 &= ~UCSWRST;     // Clear SW reset, resume operationUCB1IE |= UCTXIE | UCRXIE; // EnableTX i RX interrup
    UCB1BR0 = 160;                  // fSCL= SMCLK(16MHz)/160 = ~100kHz
    UCB1BR1 = 0;
    UCB1CTL1 &= ~UCSWRST;           // Clear SW reset, resume operation
    UCB1IE |= UCTXIE ;      // EnableTX i RX interrupt
}


// ----------------------------------- PUBLIC METHODS ---------------------------------

void i2c_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades) {
    UCB1I2CSA = addr;                   //Coloquem l'adreça de slave en mode write
    PTxData = buffer;                   //adreça del bloc de dades a transmetre
    TXByteCtr = n_dades;                //carreguem el número de dades a transmetre;
    UCB1CTL1 |= UCTR + UCTXSTT;         //I2C TX, enviem la condicio de start
    __bis_SR_register(LPM0_bits + GIE); //Enter LPM0, enable interrupts
    __no_operation();                   //Remain in LPM0 until all data are TX
    while(UCB1CTL1 & UCTXSTP);          //Ens assegurem que s'ha enviat la condicio de stop
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void) {
    switch (__even_in_range(UCB1IV,12)) {
    case 0: break;                                      // Vector 0: No interrupts
    case 2: break;                                      // Vector 2: ALIFG
    case 4: break;                                      // Vector 4: NACKIFG
    case 6: break;                                      // Vector 6: STTIFG
    case 8: break;                                      // Vector 8: STPIFG
    case 10:                                            // Vector 10: RXIFG
        RXByteCtr--;                                    // Decrement RX byte counter
        if (RXByteCtr) {
            *PRxData++ = UCB1RXBUF;                     // Mou la dada rebuda a l’adreça PRxData
            if (RXByteCtr == 1) UCB1CTL1 |= UCTXSTP;    // Genera I2C stop condition
        }
        else {
            *PRxData = UCB1RXBUF;                       // Mou la dada rebuda a l’adreça PRxData
            __bic_SR_register_on_exit(LPM0_bits);       // Exit del mode baix consum LPM0, activa la CPU
        }
        break;
    case 12:                                            // Vector 12: TXIFG
        if (TXByteCtr) {                                // Check TX byte counter
            UCB1TXBUF = *PTxData++;                     // Carrega el TX buffer amb la dada a enviar
            TXByteCtr--;                                // Decrementa TX byte counter
        }
        else {
            UCB1CTL1 |= UCTXSTP;                        // I2C stop condition
            UCB1IFG &= ~UCTXIFG;                        // Clear USCI_B1 TX int flag
            __bic_SR_register_on_exit(LPM0_bits);       // Exit del mode baix consum LPM0, activa la CPU
        }
        break;
    default: break;
    }
}
