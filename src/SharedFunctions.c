/*
 * SharedFunctions.cpp
 *
 *  Created on: Apr 12, 2016
 *      Author: krzys
 */

#include "SharedFunctions.h"

void InitTimerTIM2ToCountInUc(){
	// enable timer 2 clock
	if((RCC->APB1ENR & RCC_APB1Periph_TIM2)==0){
		RCC->APB1ENR |= RCC_APB1Periph_TIM2;
	}

	// clear configuration to reset value timer is simple up-counter
	// using sclk by default
	TIM2->CR1 =0x0000;

	// set prescaler to make counter counting in uS
	TIM2->PSC=(uint16_t)(SystemCoreClock/2000000);

	// auto-reload if full overflow
	TIM2->ARR=0xffffffff;

	// load new values
	TIM2->EGR|=0x01;
}
