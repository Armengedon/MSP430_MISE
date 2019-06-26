#ifndef SRC_MODULES_HCSR04_H_
#define SRC_MODULES_HCSR04_H_

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdint.h>

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PUBLIC METHODS ---------------------------------

/**
 * Starts a reading of the distance in front of the module
 */
uint32_t hcsr04_readDistance(void);

/**
 * Initializes the hcsr04 module
 */
void hcsr04_init(void);

#endif /* SRC_MODULES_HCSR04_H_ */
