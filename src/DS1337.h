/*
 * DS1337.h
 *
 *  Created on: Apr 30, 2016
 *      Author: krzys
 */

#ifndef DS1337_LOCKER
#define DS1337_LOCKER

#include "stm32f4xx_gpio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_rcc.h"
#include "SharedFunctions.h"

#define DS1337_BEGIN_OF_TIME_REGISTER 0x00
#define DS1337_BEGIN_OF_ALARM1_REGISTER 0x07
#define DS1337_BEGIN_OF_ALARM2_REGISTER 0x0B
#define DS1337_CONTROL_REGISTER 0x0E
#define DS1337_STATUS_REGISTER 0x0F

#define DS1337_ADRESS 0b1101000

struct DS1337_STATUS_STRUCT{
	int ERROR_CODE;
}DS1337_STATUS;
enum DS1337_ERRORS{
	DS1337_OK,
	DS1337_TIME_OUT
};

struct DS1337_TIME_STRUCT{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t decade;
}DS1337_TIME;

void ds1337_INIT();
void ds1337_writeRecordToRegister(uint8_t adress,uint8_t value);
void ds1337_writeRecordsToRegisters(uint8_t beginAdress,uint8_t * array,uint8_t size);
void ds1337_readRecordsFromRegister(uint8_t adress,uint8_t * ptrToBuffer);
void ds1337_readRecordsFromRegisters(uint8_t beginAdress,uint8_t * array,uint8_t size);
void ds1337_writeTimeStructure();

#endif
