/*
 * timers.h
 *
 *  Created on: 14 may. 2019
 *      Author: Jordi
 */

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <stdint.h>
// ------------------------------------- DEFINES --------------------------------------


// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PUBLIC METHODS -----------------------------
void timerA_timeout_start(uint32_t delay);

void timerA_timeout_stop(void);

uint32_t timerA1_1_startCapture(void);

uint8_t timers_isTimeout(void);

void timers_init(void);

void wait_ms(uint32_t delay);

void wait_us(uint32_t delay);

#endif /* SRC_TIMERS_H_ */
