/*
 * i2c_lcd.c
 *
 *  Created on: 2 may. 2019
 *      Author: jarmengol
 */


// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include "i2c_lcd.h"

// ------------------------------------- DEFINES --------------------------------------

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define I2C_LCD_TEXT_MAX_SIZE       17           //16 + 1 byte de commandament
#define I2C_LCD_CMD_LENGTH          2


// ------------------------------------- TYPEDEFS -------------------------------------
/**
 * Variable parameters value and state structure.
 */
typedef struct {
    uint8_t data[I2C_LCD_TEXT_MAX_SIZE];
    uint8_t n_dades;
} i2c_lcd_buffer_t;

// ----------------------------------- PRIVATE VARS  ----------------------------------


// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------

static void i2c_lcd_i2c_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades);

// ----------------------------------- PRIVATE METHODS --------------------------------

static void i2c_lcd_i2c_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades) {
    UCB1I2CSA = addr;
    PTxData = buffer;
    TXByteCtr = n_dades;
    UCB1CTL1 |= UCTR + UCTXSTT;
    __bis_SR_register(LPM0_bits + GIE);
    __no_operation();
    while(UCB1CTL1 & UCTXSTP);
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void) {
    switch (__even_in_range(UCB1IV,12)) {
    case 0: break;
    case 2: break;
    case 4: break;
    case 6: break;
    case 8: break;
    case 10:
        RXByteCtr--;
        if (RXByteCtr) {
            *PRxData++ = UCB1RXBUF;
            if (RXByteCtr == 1) UCB1CTL1 |= UCTXSTP;
        }
        else {
            *PRxData = UCB1RXBUF;
            __bic_SR_register_on_exit(LPM0_bits);
        }
        break;
    case 12:
        if (TXByteCtr) {
            UCB1TXBUF = *PTxData++;
            TXByteCtr--;
        }
        else {
            UCB1CTL1 |= UCTXSTP;
            UCB1IFG &= ~UCTXIFG;
            __bic_SR_register_on_exit(LPM0_bits);
        }
        break;
    default: break;
    }
}

// ----------------------------------- PUBLIC METHODS ---------------------------------

void i2c_lcd_init() {
    P4SEL |= BIT2 + BIT1;           // P4.1 iP4.2 com a USCI sifem server USCI B1
    UCB1CTL1 |= UCSWRST;            // Aturem el mòdul
    //El configurem com a master, sincroni mode i2c, per defecte, estàen single-mastermode
    UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;
    UCB1CTL1 = UCSSEL_2 + UCSWRST;  // UseSMCLK, keepSW resetUCB1BR0 = 160;              // fSCL= SMCLK(16MHz)/160 = ~100kHzUCB1BR1 = 0;UCB1CTL1 &= ~UCSWRST;     // Clear SW reset, resume operationUCB1IE |= UCTXIE | UCRXIE; // EnableTX i RX interrup
    UCB1BR0 = 160;                  // fSCL= SMCLK(16MHz)/160 = ~100kHz
    UCB1BR1 = 0;
    UCB1CTL1 &= ~UCSWRST;           // Clear SW reset, resume operation
    UCB1IE |= UCTXIE | UCRXIE;      // EnableTX i RX interrupt
}

void i2c_lcd_send_string(uint8_t *buffer) {
    i2c_lcd_i2c_send(0x3E, buffer, NELEMS(buffer));
}

void i2c_lcd_send_cmd(uint8_t *buffer) {
    i2c_lcd_i2c_send(0x3E, buffer, I2C_LCD_CMD_LENGTH);
}
