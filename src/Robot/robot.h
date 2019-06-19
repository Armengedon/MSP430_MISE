/*
 * robot.h
 *
 *  Created on: 15 may. 2019
 *      Author: Jordi
 */

#ifndef SRC_ROBOT_ROBOT_H_
#define SRC_ROBOT_ROBOT_H_

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------
typedef enum {
    MENU_CALIB_LDRS = 0,
    MENU_SEEK_LIGHT,
    MENU_MANUAL_OP,
    MENU_MEAS_DIST,
} menuSelection_t;

// ----------------------------------- PUBLIC METHODS ---------------------------------

void robot_showMenu(menuSelection_t selection);

void robot_mainMenu(void);

/**
 * Initialization routine of the different modules of the robot
 */
void robot_init(void);



#endif /* SRC_ROBOT_ROBOT_H_ */
