/*
 * lcd.c
 *
 *  Created on: 15 may. 2019
 *      Author: Jordi
 */

// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <string.h>
#include <HAL/i2c.h>
#include <HAL/timers.h>
#include <Modules/lcd.h>

// ------------------------------------- DEFINES --------------------------------------

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define I2C_LCD_TEXT_MAX_SIZE       17           //16 + 1 byte de commandament


// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PRIVATE VARS  ----------------------------------


// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------

void lcd_reset(void);

// ----------------------------------- PRIVATE METHODS --------------------------------

void lcd_reset() {
    P6OUT &= ~(BIT0);
    wait_ms(2);
    P6OUT |= BIT0;
}

void lcd_resetPinInit() {
    /* Inisialisation of reset pin */
    P6SEL &= ~(BIT0);
    P6DIR |= BIT0;
    P6OUT |= BIT0;
}


// ----------------------------------- PUBLIC METHODS ---------------------------------

void lcd_init() {

    i2c_init();

    lcd_resetPinInit();
    lcd_reset();

    wait_ms(15);

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
    i2c_send(0x3E, buffer_long, 9);

    wait_ms(10);

}
