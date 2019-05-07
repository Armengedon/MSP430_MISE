#include <msp430.h>
#include "ucs_cfg.h"
#include "i2c_lcd.h"
#include "gpio_buttons.h"
#include <stdint.h>
#include <string.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	ucs_cfg_init();
//	i2c_lcd_display_init();
//	char buffer[10] = "@Hello";
	gpio_buttons_init();
    _enable_interrupt();

	while(1) {

	}

}
