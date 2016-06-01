/*
 * SharedFunctions.cpp
 *
 *  Created on: Apr 12, 2016
 *      Author: krzys
 */

#include "SharedFunctions.h"
#include "Results.h"

void delayMSC(int ms){
InitTimerTIM2ToCountInUc();
	int i=0;
	for(i=0;i<ms;i++){
			TIM2->CNT = 0x00;
			while(TIM2->CNT<1000){
				__NOP();
			}
	}
}
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



void i2c_init(void){

                    GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as I2C1SDA and I2C1SCL

    I2C_InitTypeDef I2C_InitStruct; // this is for the I2C1 initilization

    /* Enable Clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6(I2C1_SCL) and 7(I2C1_SDA)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // the pins are configured as alternate function so the USART peripheral has access to them
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;// this defines the IO speed and has nothing to do with the baudrate!
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;// this defines the output type as open drain
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;// this activates the pullup resistors on the IO pins

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

    GPIO_Init(GPIOB, &GPIO_InitStruct);// now all the values are passed to the GPIO_Init()

       /* Set the I2C structure parameters */
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = 100000;

    I2C_Cmd(I2C1, DISABLE);
    I2C_Init(I2C1,&I2C_InitStruct);
    I2C_Cmd(I2C1, ENABLE);

}

void InitI2C3(){
	// enable i2c2 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

	// enable GPIOF clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_I2C3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_I2C3);

	// enable syscfg clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	// reset i2c2 circuit
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3,DISABLE);

	// initiate I2C pins as open-drain with no-pull resistors
	GPIO_InitTypeDef pinInitStruct;
	pinInitStruct.GPIO_Mode=GPIO_Mode_AF;
	pinInitStruct.GPIO_Pin=GPIO_Pin_8;
	pinInitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	pinInitStruct.GPIO_OType=GPIO_OType_OD;
	pinInitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&pinInitStruct);

	pinInitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOC,&pinInitStruct);

	// configure I2C as half-duty cycle with seven bit adressing using 10kHz frequency
	I2C_InitTypeDef i2cInitStruct;
	I2C_StructInit(&i2cInitStruct);
	i2cInitStruct.I2C_Mode=I2C_Mode_I2C;
	i2cInitStruct.I2C_OwnAddress1=0xA0;
	i2cInitStruct.I2C_ClockSpeed=100000;
	i2cInitStruct.I2C_DutyCycle=I2C_DutyCycle_2;
	i2cInitStruct.I2C_Ack=I2C_Ack_Enable;
	i2cInitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;

	I2C_Init(I2C3,&i2cInitStruct);
	I2C_Cmd(I2C3,ENABLE);

	I2C3_CONFIGURED=1;
}
