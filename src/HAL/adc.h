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
    CHANNEL_10 = ADC10INCH_10, //10 = 2
}channel_t;

// ----------------------------------- PUBLIC METHODS ---------------------------------

/**
 * Converts the passed parameter to a mV result
 * @param lecture, value read from an adc
 */
uint32_t adc_conversion(uint16_t lecture);

/**
 * Reads and returns the adc selected in a certain Channel
 * @param channel, channel of the adc pin
 */
uint16_t adc_read(channel_t channel);

/**
 * Initialization of the adc peripheral
 */
void adc_init(void);

#endif /* SRC_HAL_ADC_H_ */
