/*
 * gpio_buttons.c
 *
 *  Created on: 7 may. 2019
 *      Author: Jordi
 */

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdint.h>
#include "gpio_buttons.h"

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PRIVATE VARS  ----------------------------------


// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------
void gpio_buttons_dis_irq(void) {
    P1IE &= ~(btn_cnl | btn_sel | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
}

void gpio_buttons_en_irq(void) {
    P1IE |= btn_cnl | btn_sel | jtk_up | jtk_dwn | jtk_rgt | jtk_lft;
}

// ----------------------------------- PUBLIC METHODS ---------------------------------

void gpio_buttons_init(void) {
    P1SEL &= ~(btn_cnl | btn_sel | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1DIR &= ~(btn_cnl | btn_sel | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1REN |= btn_cnl | btn_sel | jtk_up | jtk_dwn | jtk_rgt | jtk_lft;
    P1IFG = 0;
    gpio_buttons_en_irq();
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void) {
    volatile uint8_t flags_iv;
    flags_iv = P1IV;
    P1IE &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    switch(flags_iv) {
        case btn_cnl_ifg:
            flags_iv = 0;
            break;
        case btn_sel_ifg:
            flags_iv = 0;
            break;
        case jtk_up_ifg:
            flags_iv = 0;
            break;
        case jtk_dwn_ifg:
            flags_iv = 0;
            break;
        case jtk_rgt_ifg:
            flags_iv = 0;
            break;
        case jtk_lft_ifg:
            flags_iv = 0;
            break;
        default:
            break;
    }
    P1IE |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
}


