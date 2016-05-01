/*
 * SharedFunctions.h
 *
 *  Created on: Apr 12, 2016
 *      Author: krzys
 */


#ifndef SHAREDFUNCTIONS_H_
#define SHAREDFUNCTIONS_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_i2c.h"

static uint8_t I2C2_CONFIGURED=0;


void InitTimerTIM2ToCountInUc();
void InitI2C2();
#endif /* SHAREDFUNCTIONS_H_ */
