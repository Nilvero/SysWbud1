/*
 * Button.c
 *
 *  Created on: May 6, 2016
 *      Author: krzys
 */


#include "Button.h"
#include "FloatConverter.h"

void ButtonDraw(Button * toDraw){
	LCD_SetFont(&Font16x24);


	LCD_SetColors(0xffff,0x0000);
	LCD_DrawFullRect(toDraw->x-2,toDraw->y-2,strlen(toDraw->string)*16+4,24+4);

	LCD_SetTextColor(0xffff);
	int i=0;
	for(;toDraw->string[i]!='\0';++i) LCD_DisplayChar(toDraw->y,toDraw->x+i*16,toDraw->string[i]);
}

int ButtonCheckIfPressed(uint16_t x,uint16_t y,Button * toCheck){
	int fx=toCheck->x-5;
	int fy=toCheck->y-5;
	int sy=toCheck->y+24+5;
	int sx=toCheck->x+strlen(toCheck->string)*16+5;

	if(fy<y && sy>y && fx<x && sx>x){
		return CLICKED;
	}

	return UNCLICKED;
}


void LabelDraw(Label * toDraw){
	LCD_SetFont(&Font8x12);
	LCD_SetColors(0xffff,0x0000);
	int i=0;
	for(;toDraw->string[i]!='\0';++i) LCD_DisplayChar(toDraw->y,toDraw->x+i*8,toDraw->string[i]);
}

void DrawFloat(uint16_t x,uint16_t y,float data,int precision){
	char buffer[20];
	ftoa (data,buffer,precision);
	Label l={x,y,buffer};
	LabelDraw(&l);
}

