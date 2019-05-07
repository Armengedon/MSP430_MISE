/*
 * i2c_lcd.c
 *
 *  Created on: 2 may. 2019
 *      Author: jarmengol
 */


// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include "i2c_lcd.h"
#include <string.h>

// ------------------------------------- DEFINES --------------------------------------

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define I2C_LCD_TEXT_MAX_SIZE       17           //16 + 1 byte de commandament
#define I2C_LCD_CMD_LENGTH          2
#define DB7                         BIT7
#define DB6                         BIT6
#define DB5                         BIT5
#define DB4                         BIT4
#define DB3                         BIT3
#define DB2                         BIT2
#define DB1                         BIT1
#define DB0                         BIT0



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

static void i2c_lcd_init();

void wait_ms(uint32_t ms);

void i2c_lcd_reset(void);

// ----------------------------------- PRIVATE METHODS --------------------------------

void i2c_lcd_reset() {
    P6OUT |= BIT0;
    wait_ms(20);
    P6OUT &= ~(BIT0);

}

void wait_ms(uint32_t ms) {
    uint32_t cicles = ms * 16000;
    for ( ; cicles != 0; --cicles);
}

void i2c_lcd_i2c_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades) {
    UCB1I2CSA = addr;
    PTxData = buffer;
    TXByteCtr = n_dades;
    UCB1CTL1 |= UCTR + UCTXSTT;
    __bis_SR_register(LPM0_bits + GIE);
    __no_operation();
    while(UCB1CTL1 & UCTXSTP);
}


void i2c_lcd_init() {
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
    UCB1IE |= UCTXIE | UCRXIE;      // EnableTX i RX interrupt
}


// ----------------------------------- PUBLIC METHODS ---------------------------------

void i2c_lcd_display_init() {
    i2c_lcd_init();

    i2c_lcd_reset();
    wait_ms(20);

    uint8_t buffer_long[9];

    /* function set */
    buffer_long[0] = 0x00;
    buffer_long[1] = 0x38;
    buffer_long[2] = 0x39;

    /*Internal OSC freq */
    buffer_long[3] = 0x14;

    /* Contrast set */
    buffer_long[4] = 0x72;

    /* Power/ICON control/Contrast set */
    buffer_long[5] = 0x54;

    /* Follower control */
    buffer_long[6] = 0x6D;

    /* Display on */
    buffer_long[7] = 0x0F;

    /*Clear display*/
    buffer_long[8] = 0x01;
    i2c_lcd_i2c_send(0x3E, buffer_long, 9);

}


#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void) {
    switch (__even_in_range(UCB1IV,12)) {
    case 0: break;                                      // Vector 0: No interrupts
    case 2: break;
    case 4: break;
    case 6: break;
    case 8: break;
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
