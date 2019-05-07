/*
 * gpio_buttons.h
 *
 *  Created on: 7 may. 2019
 *      Author: Jordi
 */

#ifndef SRC_GPIO_BUTTONS_H_
#define SRC_GPIO_BUTTONS_H_

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdint.h>

// ------------------------------------- DEFINES --------------------------------------

#define btn_cnl             BIT5
#define btn_sel             BIT4
#define jtk_up              BIT3
#define jtk_lft             BIT2
#define jtk_dwn             BIT1
#define jtk_rgt             BIT0

#define btn_cnl_ifg         0x0C
#define btn_sel_ifg         0x0A
#define jtk_up_ifg          0x04
#define jtk_lft_ifg         0x02
#define jtk_dwn_ifg         0x08
#define jtk_rgt_ifg         0x06

// ------------------------------------- TYPEDEFS -------------------------------------


// ----------------------------------- PUBLIC METHODS ---------------------------------

void gpio_buttons_init(void);

#endif /* SRC_GPIO_BUTTONS_H_ */
