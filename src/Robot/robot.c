/*
 * robot.c
 *
 *  Created on: 15 may. 2019
 *      Author: Jordi
 */

// ------------------------------------- INCLUDES -------------------------------------

#include "msp430.h"
#include <stdio.h>
#include <stdlib.h>
#include <HAL/ucs_cfg.h>
#include <HAL/timers.h>
#include <Modules/buttons.h>
#include <Modules/lcd.h>
#include <Modules/ldrs.h>
#include <Modules/AX12.h>
#include <Robot/robot.h>


// ------------------------------------- DEFINES --------------------------------------

// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PRIVATE VARS  ----------------------------------

/**
 * Actual menu we are in
 */
static volatile menuSelection_t menuSelection;
/**
 * Last button pressed
 */
static volatile btn_t btn_prsd;


// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------

/**
 * From both inputs , light difference threshold and the light difference read from both LDRs the function
 * decides what the AX12 should do in order to follow the light
 */
void robot_motorDecision(int32_t lightDiffTHD,int32_t lightDiff);

/**
 * Enters the seekLight functionality. Follow the light with the two LDRs function
 */
void robot_seekLight(void);

/**
 * Enters the calibration ADCs of the LDRs function. In this menu we can read both LDRs voltage lecture in the
 * LCD so we can calibrate them with the 2 potentiometers
 */
void robot_calibADC(void);

/**
 * Decides which menu to go depending on the last button pressed and the actual menu we are in
 */
void robot_changeMenu(void);

// ----------------------------------- PRIVATE METHODS --------------------------------

void robot_calibADC(void) {

    uint8_t buffer[20] = {0};

    while (BTN_CNL != btn_prsd) {
        wait_ms(30);
        lcd_clearDisplay();

        sprintf((char *) buffer, "@LDR LEFT  %dV", (uint16_t) ldrs_readVoltage(LDR_LFT));  //prepare char pointer to send to the lcd

        wait_ms(10);//If this is removed instead of LDR.... the lcd shows LR....

        lcd_sendLine(buffer);
        lcd_2ndLineShift();

        sprintf((char *) buffer, "@LDR RIGHT %dV", (uint16_t) ldrs_readVoltage(LDR_RGT));  //prepare char pointer to send to the lcd

        lcd_sendLine(buffer);
        wait_ms(50);

        btn_prsd = buttons_lastPressed();
    }

    robot_mainMenu();
}

void robot_changeMenu() {
    switch (menuSelection) {
    case MENU_CALIB_LDRS:
        switch (btn_prsd) {
        case BTN_JTK_RGT:
            robot_showMenu(MENU_SEEK_LIGHT);
            break;

        case BTN_JTK_LFT:
            robot_showMenu(MENU_MANUAL_OP);
            break;

        }
        break;

    case MENU_SEEK_LIGHT:
        switch (btn_prsd) {
        case BTN_JTK_RGT:
            robot_showMenu(MENU_MANUAL_OP);
            break;

        case BTN_JTK_LFT:
            robot_showMenu(MENU_CALIB_LDRS);
            break;

        }
        break;

    case MENU_MANUAL_OP:
        switch (btn_prsd) {
        case BTN_JTK_RGT:
            robot_showMenu(MENU_CALIB_LDRS);
            break;

        case BTN_JTK_LFT:
            robot_showMenu(MENU_SEEK_LIGHT);
            break;

        }

        break;
    default:
        break;
    }

}

void robot_motorDecision(int32_t lightDiffTHD, int32_t lightDiff) {

    /*Go Forward */
    if (abs(lightDiff) < lightDiffTHD) {
        AX12_motorControl(AX12_LEFT, AX12_LFT_FORWARD, AX12_SPD_MED);
        AX12_motorControl(AX12_RIGHT, AX12_RGT_FORWARD, AX12_SPD_MED);
    }
    /*Go left */
    else if (lightDiff < (0 - lightDiffTHD)) {
        AX12_motorControl(AX12_LEFT, AX12_LFT_FORWARD, AX12_SPD_FAST);
        AX12_motorControl(AX12_RIGHT, AX12_RGT_FORWARD, AX12_SPD_SLW);
    }
    /*Go right*/
    else {
        AX12_motorControl(AX12_LEFT, AX12_LFT_FORWARD, AX12_SPD_SLW);
        AX12_motorControl(AX12_RIGHT, AX12_RGT_FORWARD, AX12_SPD_FAST);
    }

}


void robot_seekLight(void) {
    uint8_t buffer[20] = {0};
    uint32_t leftLight = 0;
    uint32_t rightLight = 0;
    int32_t lightDiff = 0;
    int32_t lightDiffTHD = 200;

    while (BTN_CNL != btn_prsd) {

        wait_ms(20);
        lcd_clearDisplay();
        wait_ms(3);
        lcd_sendLine("@-SEEKING LIGHT-");
        wait_ms(3);
        lcd_2ndLineShift();
        wait_ms(3);

        sprintf((char *) buffer, "@THOLD:  %d", (uint16_t) lightDiffTHD);  //prepare char pointer to send to the lcd
        lcd_sendLine(buffer);

        if (BTN_JTK_UP == btn_prsd) {
            if (lightDiffTHD != 400) lightDiffTHD += 20;
        } else if (BTN_JTK_DWN == btn_prsd){
            if (lightDiffTHD != 20) lightDiffTHD -= 20;
        }

        leftLight = ldrs_readVoltage(LDR_LFT);
        rightLight = ldrs_readVoltage(LDR_RGT);

        lightDiff = rightLight - leftLight;

        robot_motorDecision(lightDiffTHD, lightDiff);

        btn_prsd = buttons_lastPressed();
    }

    AX12_motorControl(AX12_BROADCAST, AX12_LFT_FORWARD, AX12_SPD_STP);
    robot_mainMenu();
}



// ----------------------------------- PUBLIC METHODS ---------------------------------

void robot_mainMenu(void) {
    menuSelection = MENU_CALIB_LDRS;
    btn_prsd = buttons_lastPressed();
    robot_showMenu(menuSelection);
    while (BTN_SEL != btn_prsd) {
        __bis_SR_register(LPM0_bits | GIE);     //enter low power mode waiting for buttons to be pressed
        btn_prsd = buttons_lastPressed();
        robot_changeMenu();
    }

    switch (menuSelection) {
        case MENU_CALIB_LDRS:
            robot_calibADC();
            break;
        case MENU_SEEK_LIGHT:
            robot_seekLight();
            break;
        case MENU_MANUAL_OP:
            break;
        default:
            break;
    }
}


void robot_showMenu(menuSelection_t selection) {

    menuSelection = selection;
    uint8_t buffer[I2C_LCD_TEXT_MAX_SIZE] = {0};

    lcd_clearDisplay();
    wait_ms(30);
    lcd_sendLine("@---MAIN MENU---");
    wait_ms(30);
    lcd_2ndLineShift();
    wait_ms(30);

    switch(selection) {
    case MENU_CALIB_LDRS:
        sprintf((char *) buffer, "@%c  CALIB LDRS ~", LCD_LFT_ARRW_CHAR);
        lcd_sendLine(buffer);
        break;

    case MENU_SEEK_LIGHT:
        sprintf((char *) buffer, "@%c  SEEK LIGHT ~", LCD_LFT_ARRW_CHAR);  //prepare char pointer to send to the lcd
        lcd_sendLine(buffer);
        break;

    case MENU_MANUAL_OP:
        sprintf((char *) buffer, "@%c  MANUAL OP  ~", LCD_LFT_ARRW_CHAR);  //prepare char pointer to send to the lcd
        lcd_sendLine(buffer);
        break;
    default:
        break;
    }

}


void robot_init(void) {
    /* init modules */
    ucs_cfg_init();
    timers_init();
    buttons_init();
    lcd_init();
    ldrs_init();
    AX12_init();

}
