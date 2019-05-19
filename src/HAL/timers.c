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
typedef enum timers_modes_t {
    MODE_STOPPED,
    MODE_BLOCK_DELAY,
    MODE_TIMEOUT,

}timers_modes_t;

// ----------------------------------- PRIVATE VARS  ----------------------------------
volatile int32_t timers_ms_counter;
volatile timers_modes_t timer_mode;

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------

void timers_init(void) {
    TA0CTL |= TASSEL__SMCLK | MC__UP;
    TA0CCR0 = 16000;                    //1 interrupt = 1 ms
    timer_mode = MODE_STOPPED;
}

void timers_timeout_stop(void) {
    TA0CCTL0 &= ~(CCIE);
}

void timers_timeout_start(uint32_t delay) {
    timers_ms_counter = delay;
    TA0CTL |= TACLR;
    TA0CCTL0 |= CCIE;
    timer_mode = MODE_TIMEOUT;
    __bis_SR_register(GIE);
}

uint8_t timers_isTimeout(void) {
    return (timers_ms_counter == 0);

}

void wait_ms(uint32_t delay) {
    timers_ms_counter = delay;
    TA0CTL |= TACLR;
    TA0CCTL0 |= CCIE;
    timer_mode = MODE_BLOCK_DELAY;

    __bis_SR_register(LPM0_bits + GIE);
    while(timers_ms_counter > 0);

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_A0_ISR(void) {
    --timers_ms_counter;
    if (timers_ms_counter == 0) {
        if (MODE_BLOCK_DELAY) __bic_SR_register_on_exit(LPM0_bits);
        TA0CCTL0 &= ~(CCIE);
    }
    TA0CCTL0 &= ~(TA0IV_TA0IFG);
}



