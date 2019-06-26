#ifndef SRC_ROBOT_ROBOT_H_
#define SRC_ROBOT_ROBOT_H_

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"

// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------

/**
 * Menu types
 */
typedef enum {
    MENU_CALIB_LDRS = 0,
    MENU_SEEK_LIGHT,
    MENU_MANUAL_OP,
    MENU_MEAS_DIST,
} menuSelection_t;

// ----------------------------------- PUBLIC METHODS ---------------------------------

/**
 * When calling this, the robot will the menu passed in the argument of type menuSelection_t
 */
void robot_showMenu(menuSelection_t selection);

/**
 * The robot displays the main menu and waits for actions from the user
 */
void robot_mainMenu(void);

/**
 * Initialization routine of the different modules of the robot
 */
void robot_init(void);



#endif /* SRC_ROBOT_ROBOT_H_ */
