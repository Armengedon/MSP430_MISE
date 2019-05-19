/*
 * AX12.h
 *
 *  Created on: 18 may. 2019
 *      Author: Jordi
 */

#ifndef SRC_MODULES_AX12_H_
#define SRC_MODULES_AX12_H_

// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <stdint.h>
#include <HAL/uart.h>

// ------------------------------------- DEFINES --------------------------------------

#define AX12_WRITE          0x03

#define AX12_LEFT           0x01
#define AX12_RIGHT          0x04
#define AX12_BROADCAST      0xFE

#define AX12_LFT_FORWARD    0x00
#define AX12_LFT_BACKWARD   0x04

#define AX12_RGT_FORWARD    0x04
#define AX12_RGT_BACKWARD   0x00

#define AX12_SPD_FAST       0x02FF
#define AX12_SPD_MED        0x0100
#define AX12_SPD_SLW        0x0050
#define AX12_SPD_STP        0x0000

// ------------------------------------- TYPEDEFS -------------------------------------

// ----------------------------------- PUBLIC METHODS ---------------------------------

void stopMotors();

void AX12_motorControl(uint8_t moduleAddr, uint16_t direction, uint16_t speed);

void AX12_turnOnLeds(uint8_t moduleAddr);

void AX12_turnOffLeds(uint8_t moduleAddr);

void AX12_init(void);

#endif /* SRC_MODULES_AX12_H_ */
