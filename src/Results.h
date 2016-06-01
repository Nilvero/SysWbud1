#ifndef RESULT_MARK
#define RESULT_MARK

#include "DHT11.h"
#include "LPS331.h"
#include "ds18b20.h"
#include "stm32f4xx_dma.h"
#define BUFFER_SIZE 220

struct{
	uint32_t sampleNumber;
	uint8_t changed;
	uint8_t sensorReadingTime;
	DHT11Result temperatureAndHumidity;
	LPS331Result pressure;
	ds18b20Result externalTemperature;

	// 0-temperature 1-humidity 2-pressure 3-external

	float temperaturesHistory[BUFFER_SIZE];
	float pressureHistory[BUFFER_SIZE];
	float extTemperaturesHistory[BUFFER_SIZE];
	float humidityHistory[BUFFER_SIZE];

	//float pastValues[4][BUFFER_SIZE];
}results;

void collectDataFromSensors();

void DMArecordPushInit();
void pushRecord();








#endif
