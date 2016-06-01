/*
 * Button.h
 * test
 *  Created on: May 6, 2016
 *      Author: krzys
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include "stm32f429i_discovery_lcd.h"
#include <string.h>

enum ButtonState{
	UNCLICKED,
	CLICKED
};

struct Button{
	uint16_t x; // kolumna
	uint16_t y;
	const char * string;
};
typedef  struct Button Button;
void ButtonDraw(Button * toDraw);

int ButtonCheckIfPressed(uint16_t x,uint16_t y,Button * toCheck);

struct Label{
	uint16_t x;
	uint16_t y;
	const char * string;
};

typedef  struct Label Label;

void LabelDraw(Label * toDraw);

void DrawFloat(uint16_t x,uint16_t y,float data,int precision);
#endif /* BUTTON_H_ */
