/*
 * DHT11.h
 *
 *  Created on: Apr 10, 2016
 *      Author: krzys
 *
 *  DHT11 pin is PG5 (none AF functions)
 */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

#ifndef DHT11_H_
#define DHT11_H_

enum DHT11_ERRORS{
	DHT11_OK,
	DHT11_CRC_ERROR,
	DHT11_NO_RESPONSE,
	DHT11_TIME_OUT,
	DHT11_DECODE_ERROR
};

typedef struct DHT11Result{
	float temperature;
	float humidity;
	int ERROR_CODE;
}DHT11Result;

DHT11Result DHT11_read();

#endif /* DHT11_H_ */
