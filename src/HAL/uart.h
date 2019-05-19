/*
 * uart.h
 *
 *  Created on: 17 may. 2019
 *      Author: Jordi
 */

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

void uart_send_byte(byte TXData);

uint8_t uart_isByte_Recibido(void);

byte uart_getDatoLeido(void);

byte TxPacket(byte bID, byte bParameterLength, byte bInstruction, byte Parametros[16]);

void uart_init(void);


#endif /* SRC_HAL_UART_H_ */
