/*
 * HCSR04.h
 *
 *  Created on: 21 may. 2019
 *      Author: Jordi
 */

#ifndef SRC_MODULES_HCSR04_H_
#define SRC_MODULES_HCSR04_H_

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdint.h>

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PUBLIC METHODS ---------------------------------

uint32_t hcsr04_readDistance(void);

void hcsr04_init(void);

#endif /* SRC_MODULES_HCSR04_H_ */
