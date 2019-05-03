#include <msp430.h>
#include "ucs_cfg.h"
#include "i2c_lcd.h"

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	ucs_cfg_init();
	i2c_lcd_init();
	uint32_t i = 0;
	for ( i = 0; i < 6250000; ++i);
	uint8_t buffer[2];
    buffer[0] = 18;
    buffer[1] = 00;
	i2c_lcd_send_cmd(buffer);
    for ( i = 0; i < 6250000; ++i);
    buffer[0] = 0x03;
    buffer[1] = 0xC0;
    i2c_lcd_send_cmd(buffer);

	while(1) {

	}

}
