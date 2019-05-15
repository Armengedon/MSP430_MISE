

#include <msp430.h>
#include <src/Modules/buttons.h>
#include <src/Modules/lcd.h>
#include "HAL/ucs_cfg.h"
#include "HAL/timers.h"
#include <stdint.h>
#include <string.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    _enable_interrupt();

    /* init modules */
    ucs_cfg_init();
    timers_init();
    lcd_init();
    buttons_init();

//    uint8_t buffer[10] = "@Hello";
//    i2c_lcd_i2c_send(0x3E, buffer, 6);

    while (1)
    {
    }

}
