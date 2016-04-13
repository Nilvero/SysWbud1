/*
 * ds18b20.cpp
 *
 *  Created on: Apr 12, 2016
 *      Author: krzys
 */

#include "ds18b20.h"
#include "SharedFunctions.h"

void ds18b20PinConfig(){
	// high logic level by default (pull-up line)
	GPIOG->ODR |= (1<<5);

	// set as output
	GPIOG->MODER |= (uint32_t)(1<<10);
	GPIOG->MODER &= ~((uint32_t)(1<<11));

	// high speed output enable
	GPIOG->OSPEEDR|=(uint32_t)(1<<11);
	GPIOG->OSPEEDR&= ~((uint32_t)(1<<10));

	// open-drain
	GPIOG->OTYPER |= ((uint32_t)(1<<5));

	// no pull-up (external pull-up)
	GPIOG->PUPDR &= ~((uint32_t)(1<<10));
	GPIOG->PUPDR &= ~((uint32_t)(1<<11));
}

void ds18b20Reset(){
	// pull data line down && wait 480us
	GPIOG->ODR&=~((uint32_t)(1<<5));
	TIM2->CNT = 0x00;
	while(TIM2->CNT<480){
		__NOP();
	}

	// let line to pull up
	GPIOG->ODR|=((uint32_t)(1<<5));
}
/*
 * 1-OK 0-NoRespoce
 */
uint8_t ds18b20WaitForPresenceResponce(){
	uint8_t detected=0;
	TIM2->CNT = 0x00;
	while(TIM2->CNT<480){
		if(!(GPIOG->IDR&(1<<5))){
			detected=1;
		}
		__NOP();
	}
	return detected;
}

void ds18b20WriteBit(uint8_t state){
	// pull line down for 1 us
	GPIOG->ODR&=~((uint32_t)(1<<5));
	TIM2->CNT = 0x00;
	while(TIM2->CNT<1){
		__NOP();
	}

	// if one let line to pull up
	if(state){
		GPIOG->ODR|=((uint32_t)(1<<5));
	}

	// wait 60 us to let sensor catch state
	TIM2->CNT=0x00;
	while(TIM2->CNT<60){
		__NOP();
	}

	// release line
	GPIOG->ODR|=((uint32_t)(1<<5));
}

uint8_t ds18b20ReadBit(void){
	uint8_t bit=0;

	// pull line down for 1 us
	GPIOG->ODR&=~((uint32_t)(1<<5));
	TIM2->CNT = 0x00;
	while(TIM2->CNT<1){
		__NOP();
	}

	//release line
	GPIOG->ODR|=((uint32_t)(1<<5));

	// wait 14 uS to enter sample area
	TIM2->CNT = 0x00;
	while(TIM2->CNT<14){
		__NOP();
	}

	// make sample
	if(GPIOG->IDR &(1<<5)){
		bit=1;
	}
	else{
		bit=0;
	}
	// wait 45 uS to end bit reading procedure
	TIM2->CNT = 0x00;
	while(TIM2->CNT<45){
		__NOP();
	}

	return bit;
}

void ds18b20WriteByte(uint8_t byte){
	uint8_t index=0;
	while(index<8){
		ds18b20WriteBit(byte&0x01);
		byte=byte>>1;
		index++;
	}
}

uint8_t ds18b20ReadByte(void){
	uint8_t index=0;
	uint8_t value=0;
	while(index<8){
		value = value>>1;
		value |= (ds18b20ReadBit()<<7);
		index++;
	}
	return value;
}

ds18b20Result ds18b20_read(){
	// check if port D enable if not enable it
	if((RCC->AHB1ENR & RCC_AHB1Periph_GPIOG)==0){
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOG;
	}

	ds18b20Result result;

	InitTimerTIM2ToCountInUc();

	// start timer
	TIM2->CR1 |= 0x01;

	ds18b20PinConfig();
	ds18b20Reset();
	if(ds18b20WaitForPresenceResponce()==0){
		result.ERROR_CODE=ds18b20_NO_RESPONSE;
		return result;
	}

	// command send to all devices on line (only one on line)
	ds18b20WriteByte(DS18B20_CMD_SKIPROM);

	// read scratchpad command
	ds18b20WriteByte(DS18B20_CMD_RSCRATCHPAD);

	// read two registers
	uint8_t temperature_l = ds18b20ReadByte();
    uint8_t temperature_h = ds18b20ReadByte();

    // combine data
    result.temperature=(((0x07 &temperature_h) << 8) + temperature_l ) * 0.0625;

    // check sign
    if(temperature_h & (1<<4)){
    	result.temperature*=-1;
    }

    // check if value out of range
    if(result.temperature<-55 || result.temperature>125){
    	result.ERROR_CODE=ds18b20_STRANGE_VALUE;
    	return result;
    }

    result.ERROR_CODE=ds18b20_OK;
    return result;
}

uint8_t ds18b20_beginConversion(){
	// check if port D enable if not enable it
	if((RCC->AHB1ENR & RCC_AHB1Periph_GPIOG)==0){
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOG;
	}

	InitTimerTIM2ToCountInUc();

	// start timer
	TIM2->CR1 |= 0x01;

	ds18b20PinConfig();
	ds18b20Reset();
	if(ds18b20WaitForPresenceResponce()==0){
		return ds18b20_NO_RESPONSE;
	}

	// command send to all devices on line (only one on line)
	ds18b20WriteByte(DS18B20_CMD_SKIPROM);

	// start conversion
	ds18b20WriteByte(DS18B20_CMD_CONVERTTEMP);

	return ds18b20_OK;
}
