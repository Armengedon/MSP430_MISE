/*
 * gpio_ldrs.c
 *
 *  Created on: 7 may. 2019
 *      Author: Jordi
 */

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdint.h>
#include <Modules/ldrs.h>


// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PRIVATE VARS  ----------------------------------


// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------

// ----------------------------------- PUBLIC METHODS ---------------------------------

void ldrs_init(void) {
    P1SEL |= (ldr_lft | ldr_rgt);
    P1DIR &= ~(ldr_lft | ldr_rgt);
//    P1REN |= (gpio_ldrs_lft | gpio_ldrs_rgt);
//    P1OUT |= (gpio_ldrs_lft | gpio_ldrs_rgt);
    ADC10CTL0 |= ADC10SHT_15 | ADC10ON;
    ADC10CTL1 |= ADC10SSEL_0 + ADC10SHP;
    ADC10CTL2 |= ADC10RES;
}


