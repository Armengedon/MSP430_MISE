

#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <HAL/wdg.h>
#include <Robot/robot.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/**
 * main.c
 */
int main(void)
{
    wdg_stop();
    robot_init();
    wdg_restart();

    while (1)
    {
        robot_mainMenu();
    }

}
