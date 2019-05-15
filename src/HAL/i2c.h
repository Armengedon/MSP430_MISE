/*
 * i2c_lcd.h
 *
 *  Created on: 2 may. 2019
 *      Author: jarmengol
 */

#ifndef SRC_I2C_LCD_H_
#define SRC_I2C_LCD_H_

// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <stdint.h>

// ------------------------------------- DEFINES --------------------------------------

uint8_t *PTxData;
uint8_t TXByteCtr;
uint8_t *PRxData;
uint8_t RXByteCtr;
uint8_t dades_smbus[256];

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------


void i2c_init(void);

void i2c_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades);

#endif /* SRC_I2C_LCD_H_ */
