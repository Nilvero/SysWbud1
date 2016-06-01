/*
 * ChartsMenu.c
 *
 *  Created on: May 6, 2016
 *      Author: krzys
 */
#include "ChartsMenu.h"

void ChartsMenu(){

	LCD_SetColors(0x0000,0xffff);
	LCD_DrawFullRect(0,0,240,320);
	Button temperature={5,30,"  Temperature "};
	Button humidity={5,80,"   Humidity   "};
	Button pressure={5,130,"   Pressure   "};
	Button extTemperature={5,180,"   Ext Temp   "};
	Button returnButton={5,230,"    Return    "};

	results.changed=1;

	TP_STATE state;

	while(1){
		state=*IOE_TP_GetState();
			if (state.TouchDetected == 128 ) {
				if(ButtonCheckIfPressed(state.X,state.Y,&returnButton)==CLICKED){
					return;
				}
				else if(ButtonCheckIfPressed(state.X,state.Y,&temperature)==CLICKED){
					ChartPanel("Temperature [5s]",results.temperaturesHistory,BUFFER_SIZE);
					results.changed=1;
				}
				else if(ButtonCheckIfPressed(state.X,state.Y,&humidity)==CLICKED){
					ChartPanel("Humidity [5s]",results.humidityHistory,BUFFER_SIZE);
					results.changed=1;
				}
				else if(ButtonCheckIfPressed(state.X,state.Y,&pressure)==CLICKED){
					ChartPanel("Pressure [5s]",results.pressureHistory,BUFFER_SIZE);
					results.changed=1;
				}
				else if(ButtonCheckIfPressed(state.X,state.Y,&extTemperature)==CLICKED){
					ChartPanel("Ext Temp. [5s]",results.extTemperaturesHistory,BUFFER_SIZE);
					results.changed=1;
				}
			}

			state.TouchDetected=0;

			if(results.changed){
				LCD_SetColors(0x0000,0xffff);
				LCD_DrawFullRect(0,0,240,320);

				ButtonDraw(&temperature);
				ButtonDraw(&humidity);
				ButtonDraw(&pressure);
				ButtonDraw(&extTemperature);
				ButtonDraw(&returnButton);

				results.changed=0;
			}

			delayMSC(100);
	}
}
