// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdint.h>
#include <HAL/adc.h>
#include <Modules/ldrs.h>

// ------------------------------------- DEFINES --------------------------------------

#define ldr_lft_bit         BIT1
#define ldr_rgt_bit         BIT2

// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PRIVATE VARS  ----------------------------------

volatile uint16_t lecture = 0;

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------

// ----------------------------------- PUBLIC METHODS ---------------------------------

void ldrs_init(void) {
    P6SEL |= ldr_lft_bit | ldr_rgt_bit;
    adc_init();
}

uint32_t ldrs_readVoltage(ldrChannel_t ldrToRead) {
    lecture = adc_read((channel_t) ldrToRead);                          //read ADC channel 2
    return adc_conversion(lecture);
}


