/*
 * adc.h
 *
 *  Created on: 15 may. 2019
 *      Author: Jordi
 */

#ifndef SRC_HAL_ADC_H_
#define SRC_HAL_ADC_H_

// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <stdint.h>

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------

typedef enum {
    CHANNEL_1 = ADC10INCH_1, //01 = 1
    CHANNEL_2 = ADC10INCH_2, //10 = 2
}channel_t;

// ----------------------------------- PUBLIC METHODS ---------------------------------

uint32_t adc_conversion(uint16_t lecture);

uint16_t adc_read(channel_t);

void adc_init(void);

#endif /* SRC_HAL_ADC_H_ */
