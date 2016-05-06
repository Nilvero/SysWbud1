/*
 * Chart.h
 *
 *  Created on: May 6, 2016
 *      Author: krzys
 */

#ifndef CHART_H_
#define CHART_H_
#include "stm32f429i_discovery_lcd.h"
#include "Button.h"

void DrawChart(uint16_t x,uint16_t y,uint16_t w,uint16_t h,float * data,uint16_t dataSize);

#endif /* CHART_H_ */
