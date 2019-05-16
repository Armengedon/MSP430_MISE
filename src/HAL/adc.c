/*
 * adc.c
 *
 *  Created on: 15 may. 2019
 *      Author: Jordi
 */


// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <string.h>
#include <HAL/timers.h>
#include <HAL/adc.h>

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PRIVATE VARS  ----------------------------------
static uint32_t converted;

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------

uint32_t adc_conversion(uint16_t lecture) {
    converted = 3300 - (( (uint32_t) lecture*3300)/(1<<10));
    return converted;
}

uint16_t adc_read(channel_t channel) {

    ADC10CTL0 &= ~(ADC10ENC);                //Stop ADC
    ADC10MCTL0 = channel;
    while (ADC10CTL1 & ADC10BUSY);          // Wait if ADC10 core is active
    ADC10CTL0 |= ADC10ENC | ADC10SC;        // Sampling and conversion start
    ADC10IE |= ADC10IE0;
    __bis_SR_register(LPM0_bits + GIE);        // LPM0, ADC10_ISR will force exit

    uint32_t result = ADC10MEM0;
    return result;
}

void adc_init(void) {

    ADC10CTL1 |= ADC10SSEL_0 + ADC10SHP;// MODCLK sampling timer, modo single
    ADC10CTL0 |= ADC10SHT_15 | ADC10ON; // 1024 muestras, ADC10ON, Interrupt enable
    ADC10CTL2 |= ADC10RES;              //Resolucio de 10 bits

}


#pragma vector=ADC10_VECTOR
__interrupt void adc10_isr(void) {
    ADC10IFG &= ~(ADC10IFG0);
    __bic_SR_register_on_exit(LPM0_bits);

}
