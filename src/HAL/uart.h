#ifndef SRC_HAL_UART_H_
#define SRC_HAL_UART_H_

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdint.h>

// ------------------------------------- DEFINES --------------------------------------

#define UART_DATA_DIR           BIT6

// ------------------------------------- TYPEDEFS -------------------------------------

typedef unsigned char byte;

// ----------------------------------- PUBLIC METHODS ---------------------------------

/**
 * Send a byte of data throught the UART
 */
void uart_send_byte(byte TXData);

/**
 * Check and return if a byte is received
 */
uint8_t uart_isByte_Recibido(void);

/**
 * Receive data
 */
byte uart_getDatoLeido(void);

/**
 *Initialization of UART peripheral
 */
void uart_init(void);


#endif /* SRC_HAL_UART_H_ */
