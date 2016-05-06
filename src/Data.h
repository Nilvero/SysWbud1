/*
 * Data.h
 *
 *  Created on: May 6, 2016
 *      Author: krzys
 */

#ifndef DATA_H_
#define DATA_H_

struct _Data{
	uint8_t changed;
	float temperature;
	float humidity;
	float pressure;
	float extTemperature;
	float dataSheet[0xff];
}Data;



#endif /* DATA_H_ */
