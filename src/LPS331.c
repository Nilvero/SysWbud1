/*
 * LPS331.c
 *
 *  Created on: Apr 29, 2016
 *      Author: krzys
 */
#include "SharedFunctions.h"
#include "LPS331.h"



void LPS331_INIT(){
	InitI2C3();

	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY)){}

	// generate start conditions
	I2C_GenerateSTART(I2C3,ENABLE);

	// enable auto-ack
	I2C_AcknowledgeConfig(I2C3,ENABLE);
	I2C_GenerateSTOP(I2C3,DISABLE);

	while(!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_MODE_SELECT)){}

	// write device adress
	I2C_Send7bitAddress(I2C3,LPS331_adress<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){}

	// send control register ID
	I2C_SendData(I2C3,LPS331_CTRL_REG1);
	while(I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}

	// send initial value to enable LPS331 in full precision mode
	I2C_SendData(I2C3,0b11100000);
	while(I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}

	// generate stop condition
	I2C_GenerateSTOP(I2C3,ENABLE);

}


LPS331Result LPS331_readPressure(){
	uint32_t lowPart;
	uint32_t basePart;
	uint32_t highPart;

	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY)){}

	//generate start condition
	I2C_GenerateSTART(I2C3,ENABLE);
	while(!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_MODE_SELECT)){}

	// send device adress
	I2C_Send7bitAddress(I2C3,LPS331_adress<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){}

	// set R/W register number to first register containing information about pressure
	I2C_SendData(I2C3,LPS331_PRESS_POUT_XL_REH|(1<<7));
	while(I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}

	// generate start condition
	I2C_GenerateSTART(I2C3,ENABLE);
	while(I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS){}

	// enable auto-ack
	I2C_AcknowledgeConfig(I2C3,ENABLE);
	I2C_GenerateSTOP(I2C3,DISABLE);

	// send device adress
	I2C_Send7bitAddress(I2C3,LPS331_adress<<1,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS){}

	// read base register of pressure
	while(I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS){}
	basePart=I2C_ReceiveData(I2C3);

	// read low part of HIGH register of pressure
	while(I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS){}
	lowPart=I2C_ReceiveData(I2C3);

	// enable stopping transmission after next receive value
	I2C_AcknowledgeConfig(I2C3,DISABLE);
	I2C_GenerateSTOP(I2C3,ENABLE);

	// read high part of HIGH register of pressure
	while(I2C_CheckEvent(I2C3,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS){}
	highPart=I2C_ReceiveData(I2C3);

	// combine values
	uint32_t result=(highPart<<16)|(lowPart<<8)|basePart;

	// convert internal date format to hPa
	struct LPS331Result resultValue;
	resultValue.pressure=((float)result)/4096.0;
	resultValue.ERROR_CODE=0;
	return resultValue;
}

