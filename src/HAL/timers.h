#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <stdint.h>
// ------------------------------------- DEFINES --------------------------------------


// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PUBLIC METHODS -----------------------------

/**
 * Starts a timeout of a certain time using the timerA
 * @param delay, timeout in ms
 */
void timerA_timeout_start(uint32_t delay);

/**
 * Stops the timeout previously started in the timerA
 */
void timerA_timeout_stop(void);

/**
 * Start capture of time in timerA1_1 pin
 */
uint32_t timerA1_1_startCapture(void);

/**
 * Checks and returns wether the timer that started a timeout reached the count or not
 */
uint8_t timers_isTimeout(void);

/**
 * Initialization of timers
 */
void timers_init(void);

/**
 * BLOCKING wait of ms
 * @param delay, delay time in ms
 */
void wait_ms(uint32_t delay);

/**
 * BLOCKING wait of us
 * @param delay, delay time in us
 */
void wait_us(uint32_t delay);

#endif /* SRC_TIMERS_H_ */
