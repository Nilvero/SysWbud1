/*
 * remoteController.c
 *
 *  Created on: Apr 26, 2016
 *      Author: krzys
 */
#include "remoteController.h"
#include "stm32f4xx_gpio.h"

void RC_INIT(){
	GPIO_InitTypeDef pinConfiguration;

	pinConfiguration.GPIO_Mode=GPIO_Mode_IN;
	pinConfiguration.GPIO_OType=GPIO_OType_PP;
	pinConfiguration.GPIO_Pin=GPIO_Pin_0;
	pinConfiguration.GPIO_PuPd=GPIO_PuPd_DOWN;
	pinConfiguration.GPIO_Speed=GPIO_Speed_25MHz;

	//GPIO_Init(0,pinConfiguration);

}

void EXTI0_IRQHandler(void)
{



}
