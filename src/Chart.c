/*
 * Chart.c
 *
 *  Created on: May 6, 2016
 *      Author: krzys
 */

#include "Chart.h"

void DrawChart(uint16_t x,uint16_t y,uint16_t w,uint16_t h,float * data,uint16_t dataSize){
	float min=10000000;
	float max=-10000000;
	int i=0;
	int minID,maxID;
	for(;i<dataSize;i++){
		if(data[i]<min){
			min=data[i];
			minID=i;
		}
		if(data[i]>max){
			max=data[i];
			maxID=i;
		}
	}

	i=0;

	LCD_SetTextColor(0x0000);
	LCD_DrawFullRect(x,y,w,h);


	LCD_SetTextColor(0xffff);
	LCD_DrawRect(x,y,h,w);

	LCD_SetTextColor(0x07E0);

	h=h-10;
	y=y+5;

	for(;i<w;i++){
		int dataIndex=i*(((float)dataSize)/((float)w));
		float valueScaled=(float)h*((float)data[dataIndex]-min)/((float)(max-min));

		LCD_DrawLine(x+i,(y+h)-valueScaled, 1, LCD_DIR_HORIZONTAL);

	}

	LCD_SetTextColor(0xffff);

	Label label={x+5,y+5,"Min: "};
	LabelDraw(&label);
	DrawFloat(x+45,y+5,(float)min,2);

	Label label2={x+5,y+25,"Max: "};
	LabelDraw(&label2);
	DrawFloat(x+45,y+25,(float)max,2);
}
