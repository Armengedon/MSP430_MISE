/*
 * timers.c
 *
 *  Created on: 14 may. 2019
 *      Author: Jordi
 */

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdint.h>
#include <HAL/timers.h>

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PRIVATE VARS  ----------------------------------
volatile int32_t counter;

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------

void timers_init(void) {
    TA0CTL |= TASSEL__SMCLK | MC__UP;
    TA0CCR0 = 16000;
}

void wait_ms(uint32_t delay) {
    counter = delay;
    TA0CTL |= TACLR;
    TA0CCTL0 |= CCIE;

    __bis_SR_register(LPM0_bits + GIE);
    while(counter > 0);

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_A0_ISR(void) {
    --counter;
    if (counter == 0) {
        __bic_SR_register_on_exit(LPM0_bits);
        TA0CCTL0 &= ~(CCIE);
    }
    TA0CCTL0 &= ~(TA0IV_TA0IFG);
}





