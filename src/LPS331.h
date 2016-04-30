/*
 * LPS331.h
 *
 *  Created on: Apr 29, 2016
 *      Author: krzys
 */

#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define LPS331_adress 0b1011101
#define LPS331_PRESS_POUT_XL_REH 0x28
#define LPS331_PRESS_LOW 0x29
#define LPS331_PRESS_HIGH 0x2A
#define LPS331_CTRL_REG1 0x20

struct LPS331Result{
	int ERROR_CODE;
	float pressure;
};

typedef struct LPS331Result LPS331Result;

void LPS331_INIT();
LPS331Result LPS331_readPressure();
