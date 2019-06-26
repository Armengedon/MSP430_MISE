// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <HAL/wdg.h>

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PRIVATE VARS  ----------------------------------

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------

void wdg_restart(void) {
    WDTCTL = WDTPW | WDTCNTCL | WDTSSEL__ACLK | WDTIS_5;   // restart watchdog timer, set ACLK 32KHz, 16s
}

void wdg_stop(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
}
