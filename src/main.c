#include <msp430.h>
#include "ucs_cfg.h"
#include "i2c_lcd.h"
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
	i2c_lcd_display_init();
	char buffer[10] = "Hello";
	char buffer_aux[10];
	memcpy(buffer_aux + 1, buffer, strlen(buffer)+1);

	i2c_lcd_send_string((uint8_t*) buffer);
	while(1) {

	}

}
