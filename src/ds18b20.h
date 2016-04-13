/*
 * ds18b20.h
 *
 *  Created on: Apr 12, 2016
 *      Author: krzys
 *  DHT11 pin is PG4 (none AF functions)
 */

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

#ifndef DS18B20_H_
#define DS18B20_H_

//Command set
#define DS18B20_CMD_CONVERTTEMP 0x44
#define DS18B20_CMD_RSCRATCHPAD 0xbe
#define DS18B20_CMD_WSCRATCHPAD 0x4e
#define DS18B20_CMD_CPYSCRATCHPAD 0x48
#define DS18B20_CMD_RECEEPROM 0xb8
#define DS18B20_CMD_RPWRSUPPLY 0xb4
#define DS18B20_CMD_SEARCHROM 0xf0
#define DS18B20_CMD_READROM 0x33
#define DS18B20_CMD_MATCHROM 0x55
#define DS18B20_CMD_SKIPROM 0xcc
#define DS18B20_CMD_ALARMSEARCH 0xec

enum ds18b20_ERRORS{
	ds18b20_OK,
	ds18b20_NO_RESPONSE,
	ds18b20_STRANGE_VALUE
};

typedef struct ds18b20Result{
	float temperature;
	int ERROR_CODE;
}ds18b20Result;

ds18b20Result ds18b20_read();

/**
 * take maks. 750ms
 */
uint8_t ds18b20_beginConversion();
#endif /* DS18B20_H_ */
