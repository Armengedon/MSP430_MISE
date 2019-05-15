/*
 * gpio_ldrs.h
 *
 *  Created on: 7 may. 2019
 *      Author: Jordi
 */

#ifndef SRC_MODULES_LDRS_H_
#define SRC_MODULES_LDRS_H_

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"

// ------------------------------------- DEFINES --------------------------------------

#define ldr_lft             BIT7
#define ldr_rgt             BIT6

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------

void ldrs_init(void);

#endif /* SRC_MODULES_LDRS_H_ */
