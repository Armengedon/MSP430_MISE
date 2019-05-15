/*
 * gpio_buttons.c
 *
 *  Created on: 7 may. 2019
 *      Author: Jordi
 */

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdint.h>
#include <Modules/buttons.h>

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PRIVATE VARS  ----------------------------------


// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------
void buttons_dis_irq(void) {
    P1IE &= ~(btn_cnl | btn_sel | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
}

void buttons_en_irq(void) {
    P1IE |= (btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
}

// ----------------------------------- PUBLIC METHODS ---------------------------------

void buttons_init(void) {
    P1SEL &= ~(btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1DIR &= ~(btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1REN |= (btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1OUT |= (btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1IFG = 0;
    buttons_en_irq();
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void) {
    volatile uint8_t flags_iv = P1IV;
    P1IE &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    switch(flags_iv) {
        case btn_cnl_ifg:
            //TODO: add state change
            flags_iv = 0;
            break;
        case btn_sel_ifg:
            //TODO: add state change
            flags_iv = 0;
            break;
        case jtk_up_ifg:
            //TODO: add state change
            flags_iv = 0;
            break;
        case jtk_dwn_ifg:
            //TODO: add state change
            flags_iv = 0;
            break;
        case jtk_rgt_ifg:
            //TODO: add state change
            flags_iv = 0;
            break;
        case jtk_lft_ifg:
            //TODO: add state change
            flags_iv = 0;
            break;
        default:
            break;
    }
    P1IE |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
}


