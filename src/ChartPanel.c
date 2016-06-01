/*
 * ChartPanel.c
 *
 *  Created on: May 6, 2016
 *      Author: krzys
 */

#include "ChartPanel.h"
void ChartPanel(char * chartName,float * pointerToDataArray,int sizeOfData){
	TP_STATE state;
	Button returnButton={5,250,"    Return    "};
	Label nameOfChart={10,10,chartName};
	results.changed=1;
	while(1){
		state=*IOE_TP_GetState();
		if (state.TouchDetected == 128 ) {
			if(ButtonCheckIfPressed(state.X,state.Y,&returnButton)==CLICKED){
				return;
			}
			state.TouchDetected=0;
		}

		if(results.changed){
			LCD_SetColors(0x0000,0xffff);
			LCD_DrawFullRect(0,0,240,320);

			LabelDraw(&nameOfChart);
			ButtonDraw(&returnButton);

			DrawChart(10,30,220,200,pointerToDataArray,sizeOfData);

			results.changed=0;

		}
		delayMSC(100);
	}
}
