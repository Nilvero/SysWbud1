/*
 * LPS331.c
 *
 *  Created on: Apr 29, 2016
 *      Author: krzys
 */
#include "SharedFunctions.h"
#include "LPS331.h"



void LPS331_INIT(){
	if(I2C2_CONFIGURED==0){
		InitI2C2();
	}

	// send command to enable LPS331 in full precision mode

	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)){}

	I2C_GenerateSTART(I2C2,ENABLE);

	I2C_AcknowledgeConfig(I2C2,ENABLE);
	I2C_GenerateSTOP(I2C2,DISABLE);

	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){}

	I2C_Send7bitAddress(I2C2,LPS331_adress<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){}

	I2C_SendData(I2C2,LPS331_CTRL_REG1);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}

	I2C_SendData(I2C2,0b11100000);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}
	I2C_GenerateSTOP(I2C2,ENABLE);

}


LPS331Result LPS331_readPressure(){
	uint32_t lowPart;
	uint32_t basePart;
	uint32_t highPart;

	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)){}

	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){}

	I2C_Send7bitAddress(I2C2,LPS331_adress<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){}

	// set R/W register number to first register containing information about pressure
	I2C_SendData(I2C2,LPS331_PRESS_POUT_XL_REH|(1<<7));
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}


	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS){}

	I2C_AcknowledgeConfig(I2C2,ENABLE);
	I2C_GenerateSTOP(I2C2,DISABLE);

	I2C_Send7bitAddress(I2C2,LPS331_adress<<1,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS){}

	// read base register of pressure
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS){}
	basePart=I2C_ReceiveData(I2C2);

	// read low part of HIGH register of pressure
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS){}
	lowPart=I2C_ReceiveData(I2C2);

	// enable stopping transmission after next receive value
	I2C_AcknowledgeConfig(I2C2,DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);

	// read high part of HIGH register of pressure
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS){}
	highPart=I2C_ReceiveData(I2C2);

	// combine values
	uint32_t result=(highPart<<16)|(lowPart<<8)|basePart;

	// convert internal date format to hPa
	struct LPS331Result resultValue;
	resultValue.pressure=((float)result)/4096.0;

	return resultValue;
}

