// ------------------------------------- INCLUDES -------------------------------------
#include "msp430.h"
#include <HAL/timers.h>
#include <Modules/hcsr04.h>

// ------------------------------------- DEFINES --------------------------------------

/* PORT 4 OUTPUT*/
#define TRIGGER             BIT0

/* PORT 2 with TA 1.1 INPUT*/
#define ECHO                BIT0
// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PRIVATE VARS  ----------------------------------

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------

// ----------------------------------- PRIVATE METHODS --------------------------------

// ----------------------------------- PUBLIC METHODS ---------------------------------


uint32_t hcsr04_readDistance(void) {
    P4OUT |= TRIGGER;               //Trigger phase, 10 us of positive pulse
    wait_us(10);
    P4OUT &= ~(TRIGGER);

    return timerA1_1_startCapture();
}

void hcsr04_init(void) {
    P4SEL &= ~(TRIGGER);
    P4DIR |= TRIGGER;

    P2DIR &= ~(ECHO);
    P2SEL |= ECHO;
}
