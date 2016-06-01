/*
 * DHT11.c
 *
 *  Created on: Apr 10, 2016
 *      Author: krzys
 */

#include "DHT11.h"
#include "SharedFunctions.h"

void DHT11SentStartRequest(){
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

	// reset value
	TIM2->CNT = 0x00;

	// start timer
	TIM2->CR1 |= 0x01;

	// wait 20 ms
	while(TIM2->CNT<20000){
		__NOP();
	}

	// pull data line down && wait 20 ms
	GPIOG->ODR&=~((uint32_t)(1<<5));
	TIM2->CNT = 0x00;
	while(TIM2->CNT<20000){
		__NOP();
	}

	// pull data line up
	GPIOG->ODR |=((uint32_t)(1<<5));
}
uint32_t DHT11RecvData(uint32_t * intervalsArray){
	// enter to input mode (external pullup)
	//GPIOG->MODER &= ~((uint32_t)(1<<11));
	//GPIOG->MODER &= ~((uint32_t)(1<<10));

	uint16_t state=GPIOG->IDR & (1<<5);

	// listen for new data
	int dataNumber;
	for(dataNumber=0;dataNumber<42;dataNumber++){
		// reset timer value
		TIM2->CNT=0;
		//start timer
		TIM2->CR1|=0x01;

		// wait for logical level change or timeout
		while(TIM2->CNT<500 && (GPIOG->IDR & (1<<5))==state){
			__NOP();
		}

		// stop timer
		TIM2->CR1 &= ~0x00000001;

		// if timeout return error code
		if(TIM2->CNT>=500){
			if(dataNumber==0){
				return DHT11_NO_RESPONSE;
			}
			else{
				return DHT11_TIME_OUT;
			}
		}

		// read new logical state
		state=GPIOG->IDR & (1<<5);

		if(!state){
			// save active state time
			intervalsArray[dataNumber]=TIM2->CNT;
		}
		else
		{
			// ignore inactive states
			dataNumber--;
		}
	}
	return DHT11_OK;
}
uint32_t DHT11DecodeByte(uint32_t * intervals,uint8_t * result){
	int index;
	*result=0;
	for(index=0;index<8;index++){
		if(intervals[index]>10 && intervals[index]<40){
			*result=(*result)<<1;
		}
		else if(intervals[index]>50 && intervals[index]<90){
			*result=((*result)<<1) + 0x01;
		}
		else{
			return DHT11_DECODE_ERROR;
		}
	}
	return DHT11_OK;
}

void DHT11Decode(uint32_t * intervals,DHT11Result * result){

	// decode RH integral
	uint8_t RH_integral=0;
	result->ERROR_CODE=DHT11DecodeByte(&intervals[2],&RH_integral);
	if(result->ERROR_CODE!=DHT11_OK){ return; }

	// decode RH decimal
	uint8_t RH_decimal=0;
	result->ERROR_CODE=DHT11DecodeByte(&intervals[10],&RH_decimal);
	if(result->ERROR_CODE!=DHT11_OK){ return; }

	//decode T integral
	uint8_t T_integral=0;
	result->ERROR_CODE=DHT11DecodeByte(&intervals[18],&T_integral);
	if(result->ERROR_CODE!=DHT11_OK){ return; }

	//decode T decimal
	uint8_t T_decimal=0;
	result->ERROR_CODE=DHT11DecodeByte(&intervals[26],&T_decimal);
	if(result->ERROR_CODE!=DHT11_OK){ return; }

	//decode CRC
	uint8_t CRC_value=0;
	result->ERROR_CODE=DHT11DecodeByte(&intervals[34],&CRC_value);
	if(result->ERROR_CODE!=DHT11_OK){ return; }

	//check CRC
	uint8_t countedCRC=T_integral+T_decimal+RH_decimal+RH_integral;
	if(countedCRC!=CRC_value){
		result->ERROR_CODE=DHT11_CRC_ERROR;
		return;
	}

	//join decimal with integral (compability with dht-22)
	result->humidity=(float)(RH_decimal/10+RH_integral);
	result->temperature=(float)(T_decimal/10+T_integral);
}
DHT11Result DHT11_read(){
	uint32_t intervals[42];

	// check if port D enable if not enable it
	if((RCC->AHB1ENR & RCC_AHB1Periph_GPIOG)==0){
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOG;
	}

	DHT11Result result;


	// HOW TO DISABLE INTERRUPTS ?
	InitTimerTIM2ToCountInUc();
	DHT11SentStartRequest();
	result.ERROR_CODE=DHT11RecvData(intervals);

	if(result.ERROR_CODE!=DHT11_OK){
		return result;
	}
	// disable timer2
	TIM2->CR1 &= ~0x00000001;

	DHT11Decode(intervals,&result);

	return result;
}
