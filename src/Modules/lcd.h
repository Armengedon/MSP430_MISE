#ifndef SRC_MODULES_LCD_H_
#define SRC_MODULES_LCD_H_

// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <stdint.h>

// ------------------------------------- DEFINES --------------------------------------

#define I2C_LCD_TEXT_MAX_SIZE       17          //16 + 1 byte de commandament
#define LCD_LFT_ARRW_CHAR           127         //In the internal memory of the LCD you can see
                                                //the characters it can write, and the left arrow
                                                //is the 127th so if you pass this num it will print it


// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PUBLIC METHODS ---------------------------------

/**
 * Clears the current content of the display
 */
void lcd_clearDisplay(void);

/**
 * Shifts the cursor to the initial position of the 2nd line
 */
void lcd_2ndLineShift(void);

/**
 * Shifts the contents of the lcd to the left
 */
void lcd_shiftLeft(void);

/**
 * Send a buffer of chars to the lcd to write
 */
void lcd_sendLine(uint8_t *buffer);

/**
 * Initialization of the lcd
 */
void lcd_init(void);

#endif /* SRC_MODULES_LCD_H_ */
