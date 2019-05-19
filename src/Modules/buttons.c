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

static volatile btn_t pressed;
uint8_t pressed_read;

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------

// ----------------------------------- PRIVATE METHODS --------------------------------
void buttons_dis_irq(void)
{
    P1IE &= ~(btn_cnl | btn_sel | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
}

void buttons_en_irq(void)
{
    P1IE |= (btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
}

// ----------------------------------- PUBLIC METHODS ---------------------------------

btn_t buttons_lastPressed(void) {
    if (pressed_read) {
        pressed = BTN_INVALID;
    }
    pressed_read = 1;
    return pressed;
}

void buttons_init(void)
{
    /* init all buttons to output 0 to save power */
    P1DIR = 0xFF;
    P1OUT = 0x00;

    P2DIR = 0xFF;
    P2OUT = 0x00;

    P3DIR = 0xFF;
    P3OUT = 0x00;

    P4DIR = 0xFF;
    P4OUT = 0x00;

    P5DIR = 0xFF;
    P5OUT = 0x00;

    P6DIR = 0xFF;
    P6OUT = 0x00;
    /* -----------------------------------------------*/

    P1SEL &= ~(btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1DIR &= ~(btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1REN |= (btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1OUT |= (btn_sel | btn_cnl | jtk_up | jtk_dwn | jtk_rgt | jtk_lft);
    P1IFG = 0;
    buttons_en_irq();
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void)
{
    volatile uint8_t flags_iv = P1IV;
    P1IE &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    switch (flags_iv)
    {
    case btn_cnl_ifg:
        pressed = BTN_CNL;
        flags_iv = 0;
        break;
    case btn_sel_ifg:
        pressed = BTN_SEL;
        flags_iv = 0;
        break;
    case jtk_up_ifg:
        pressed = BTN_JTK_UP;
        flags_iv = 0;
        break;
    case jtk_dwn_ifg:
        pressed = BTN_JTK_DWN;
        flags_iv = 0;
        break;
    case jtk_rgt_ifg:
        pressed = BTN_JTK_RGT;
        flags_iv = 0;
        break;
    case jtk_lft_ifg:
        pressed = BTN_JTK_LFT;
        flags_iv = 0;
        break;
    default:
        break;
    }

    pressed_read = 0;
    __bic_SR_register_on_exit(LPM0_bits);
    P1IE |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
}

