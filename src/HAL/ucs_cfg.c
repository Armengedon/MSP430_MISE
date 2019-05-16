/*
 * ucs_cfg.c
 *
 *  Created on: 2 may. 2019
 *      Author: jarmengol
 */


// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <HAL/ucs_cfg.h>

// ------------------------------------- DEFINES --------------------------------------


// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PRIVATE VARS  ----------------------------------


// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------


// ----------------------------------- PRIVATE METHODS --------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------

void ucs_cfg_init(void) {
    const unsigned long DCORSEL = DCORSEL_7;    //DCORSEL_6 selecciona rango de DCO de 7 a 60MHz
    const unsigned short FLLN = 487;            //Parametro N
    /*inicialització de les frequencies de rellotge del microcontrolador*/
    __bis_SR_register(SCG0);                    //Desactiva el FLL control loop
    UCSCTL0 = 0x00;                             //Posa DC0x, Modx al minim possible
    UCSCTL1 = DCORSEL;                          //Seleccionem el rang de DCO 16 MHz
    UCSCTL2 = FLLN + FLLD_1;                    //Selecciona el factor de multiplicador del DCO
    UCSCTL3 = 0;                                //Referencia FLL SELREF = XT1, divisor 1;
    /* Seleccio de la font de rellotge: ACLK el clock extern de 2 32, SMCLK i MCLK el DCO extern de 16 MHz */
    UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV  |  SELM__DCOCLKDIV;
    UCSCTL5 = DIVA__1 | DIVS__1;                //Divisor per SMCLK: f(SMCLK)/1 i  ACLK: f(ACLK)/1
    __bic_SR_register(SCG0);                    // Enable the FLL control loop
}

