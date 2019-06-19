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

#define I2C_LCD_TEXT_MAX_SIZE       17           //16 + 1 byte de commandament
#define LCD_LFT_ARRW_CHAR           127


// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PUBLIC METHODS ---------------------------------

void lcd_clearDisplay(void);

void lcd_2ndLineShift(void);

void lcd_shiftLeft(void);

void lcd_sendLine(uint8_t *buffer);

void lcd_init(void);

#endif /* SRC_MODULES_LCD_H_ */
