

#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <Robot/robot.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    _enable_interrupt();

    robot_init();

    robot_mainMenu();

//    uint8_t buffer[10] = "@Hello";
//    i2c_lcd_i2c_send(0x3E, buffer, 6);

    while (1)
    {
    }

}
