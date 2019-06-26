#ifndef SRC_HAL_WDG_H_
#define SRC_HAL_WDG_H_

// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <stdint.h>

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PUBLIC METHODS ---------------------------------

void wdg_restart(void);

void wdg_stop(void);

#endif /* SRC_HAL_WDG_H_ */