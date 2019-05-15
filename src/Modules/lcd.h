/*
 * lcd.h
 *
 *  Created on: 15 may. 2019
 *      Author: Jordi
 */

#ifndef SRC_MODULES_LCD_H_
#define SRC_MODULES_LCD_H_

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

void lcd_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades);

void lcd_init(void);

#endif /* SRC_MODULES_LCD_H_ */
