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

void InitI2C2(){
	// enable i2c2 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	// enable GPIOF clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);

	GPIO_PinAFConfig(GPIOF,GPIO_PinSource1,GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource0,GPIO_AF_I2C2);

	// enable syscfg clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	// reset i2c2 circuit
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2,DISABLE);

	// initiate I2C pins as open-drain with no-pull resistors
	GPIO_InitTypeDef pinInitStruct;
	pinInitStruct.GPIO_Mode=GPIO_Mode_AF;
	pinInitStruct.GPIO_Pin=GPIO_Pin_1;
	pinInitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	pinInitStruct.GPIO_OType=GPIO_OType_OD;
	pinInitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOF,&pinInitStruct);

	pinInitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOF,&pinInitStruct);

	// configure I2C as half-duty cycle with seven bit adressing using 10kHz frequency
	I2C_InitTypeDef i2cInitStruct;
	I2C_StructInit(&i2cInitStruct);
	i2cInitStruct.I2C_Mode=I2C_Mode_I2C;
	i2cInitStruct.I2C_OwnAddress1=0xA0;
	i2cInitStruct.I2C_ClockSpeed=10000;
	i2cInitStruct.I2C_DutyCycle=I2C_DutyCycle_2;
	i2cInitStruct.I2C_Ack=I2C_Ack_Enable;
	i2cInitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;

	I2C_Init(I2C2,&i2cInitStruct);
	I2C_Cmd(I2C2,ENABLE);

	I2C2_CONFIGURED=1;
}
