/*
 * ChartPanel.h
 *
 *  Created on: May 6, 2016
 *      Author: krzys
 */

#ifndef CHARTPANEL_H_
#define CHARTPANEL_H_
#include "Button.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"
#include "Chart.h"
#include "Results.h"
void ChartPanel(char * chartName,float * pointerToDataArray,int sizeOfData);


#endif /* CHARTPANEL_H_ */
