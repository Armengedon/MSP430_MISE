

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
    __enable_interrupt();
    robot_init();

    robot_mainMenu();

    while (1)
    {
    }

}
