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

/**
 * Initialization fo the i2c peripheral
 */
void i2c_init(void);

/**
 * Send a buffer of data to the selected address
 */
void i2c_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades);

#endif /* SRC_I2C_LCD_H_ */
