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

volatile int32_t counter_start;
volatile int32_t counter_finish;

volatile int32_t timers_ms_counter;
volatile int32_t timers_us_counter;
volatile timers_modes_t timerA_mode;
volatile timers_modes_t timerB_mode;

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------

void timers_init(void) {
    TA0CTL |= TASSEL__SMCLK | MC__UP;
    TA0CCR0 = 16000;                    //1 interrupt = 1 ms
    timerA_mode = MODE_STOPPED;

    TA1CTL |= TASSEL__SMCLK | MC__CONTINUOUS ;
    timerA_mode = MODE_STOPPED;
}

void timerA_timeout_stop(void) {
    TA0CCTL0 &= ~(CCIE);
}

void timerA_timeout_start(uint32_t delay) {
    timers_ms_counter = delay;
    TA0CTL |= TACLR;
    TA0CCTL0 |= CCIE;
    timerA_mode = MODE_TIMEOUT;
    __bis_SR_register(GIE);
}

uint32_t timerA1_1_startCapture(void) {
    counter_start = 0;
    counter_finish = 0;
    TA1CTL |= TACLR;
    TA1CCTL1 |= CAP | CCIS_0 | SCS |CM_3 | CCIE;
    while (counter_finish == 0 || counter_start == 0);
    return ((counter_finish - counter_start) >> 4 )/58;
}

uint8_t timers_isTimeout(void) {
    return (timers_ms_counter == 0);

}

void wait_us(uint32_t delay) {
    TB0CTL |= TASSEL__SMCLK | MC__UP;
    TB0CCR0 = 16;
    timerB_mode = MODE_STOPPED;


    timers_us_counter = delay;
    TB0CTL |= TACLR;
    TB0CCTL0 |= CCIE;
    timerB_mode = MODE_BLOCK_DELAY;

    while(timers_us_counter > 0);
}

void wait_ms(uint32_t delay) {
    timers_ms_counter = delay;
    TA0CTL |= TACLR;
    TA0CCTL0 |= CCIE;
    timerA_mode = MODE_BLOCK_DELAY;

    __bis_SR_register(LPM0_bits + GIE);
    while(timers_ms_counter > 0);

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_A0_ISR(void) {
    --timers_ms_counter;
    if (timers_ms_counter == 0) {
        if (timerA_mode == MODE_BLOCK_DELAY) __bic_SR_register_on_exit(LPM0_bits);
        TA0CCTL0 &= ~(CCIE);
    }
    TA0CCTL0 &= ~(TA0IV_TAIFG);
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void timer1_A1_ISR(void) {
    if (TA1CCTL1 & CCI) {          // Raising edge
        counter_start = TA1CCR1;      // Copy counter to variable
    } else if (TA1CCTL1 & COV) {
        counter_finish = 100;
        counter_start = 100;
    } else {                    // Falling edge
        counter_finish = TA1CCR1;
            // Formula: Distance in cm = (Time in uSec)/58
//            distance_cm = (CCR0 - counter)/58;
    }
    TA1CCTL1 &= ~TAIFG;           // Clear interrupt flag - handled
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void timer0_B0_ISR(void) {
    --timers_us_counter;
    if (timers_us_counter < 0) {
        TB0CCTL0 = 0;
    }
    TB0CCTL0 &= ~(TB0IV_TBIFG);
}



