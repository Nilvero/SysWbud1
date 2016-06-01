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
#include "DHT11.h"
#include "ds18b20.h"
#include "LPS331.h"

static uint8_t I2C3_CONFIGURED=0;

void collectDataFromSensors();
void InitTimerTIM2ToCountInUc();
void InitI2C3();
void delayMSC(int ms);
#endif /* SHAREDFUNCTIONS_H_ */
