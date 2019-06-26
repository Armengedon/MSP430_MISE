#ifndef SRC_MODULES_BUTTONS_H_
#define SRC_MODULES_BUTTONS_H_

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

typedef enum btn_t{
    BTN_CNL,
    BTN_SEL,
    BTN_JTK_UP,
    BTN_JTK_LFT,
    BTN_JTK_DWN,
    BTN_JTK_RGT,
    BTN_INVALID,
} btn_t;

// ----------------------------------- PUBLIC METHODS ---------------------------------

/**
 * Returns the btn_t corresponding to the last button that was pressed
 */
btn_t buttons_lastPressed(void);

/**
 * Initialization of buttons
 */
void buttons_init(void);

#endif /* SRC_MODULES_BUTTONS_H_ */
