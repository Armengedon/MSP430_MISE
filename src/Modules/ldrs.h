#ifndef SRC_MODULES_LDRS_H_
#define SRC_MODULES_LDRS_H_

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <HAL/adc.h>

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------

typedef enum channel_t{
    LDR_LFT = CHANNEL_1,
    LDR_RGT = CHANNEL_2,
} ldrChannel_t;

// ----------------------------------- PUBLIC METHODS ---------------------------------

/**
 * Initialization of the ldrs module
 */
void ldrs_init(void);

/**
 * Reads the voltage of the selected ldr and returns it
 */
uint32_t ldrs_readVoltage(ldrChannel_t ldrToRead);

#endif /* SRC_MODULES_LDRS_H_ */
