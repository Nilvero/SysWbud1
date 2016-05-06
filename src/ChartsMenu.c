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

	Data.changed=1;

	float data[]={1,3,2,4,2,20,2,4,2,4,2,5,12};
	TP_STATE state;

	while(1){
		state=*IOE_TP_GetState();
			if (state.TouchDetected == 128 ) {
				if(ButtonCheckIfPressed(state.X,state.Y,&returnButton)==CLICKED){
					return;
				}
				else if(ButtonCheckIfPressed(state.X,state.Y,&temperature)==CLICKED){
					ChartPanel("Temperature [5s]",data,11);
					Data.changed=1;
				}
				else if(ButtonCheckIfPressed(state.X,state.Y,&humidity)==CLICKED){
					ChartPanel("Humidity [5s]",data,11);
					Data.changed=1;
				}
				else if(ButtonCheckIfPressed(state.X,state.Y,&pressure)==CLICKED){
					ChartPanel("Pressure [5s]",data,11);
					Data.changed=1;
				}
				else if(ButtonCheckIfPressed(state.X,state.Y,&extTemperature)==CLICKED){
					ChartPanel("Ext Temp. [5s]",data,11);
					Data.changed=1;
				}
			}

			state.TouchDetected=0;

			if(Data.changed){
				LCD_SetColors(0x0000,0xffff);
				LCD_DrawFullRect(0,0,240,320);

				ButtonDraw(&temperature);
				ButtonDraw(&humidity);
				ButtonDraw(&pressure);
				ButtonDraw(&extTemperature);
				ButtonDraw(&returnButton);

				Data.changed=0;
			}

	}
}
