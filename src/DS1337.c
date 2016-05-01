/*
 * DS1337.c
 *
 *  Created on: Apr 30, 2016
 *      Author: krzys
 */
#include "DS1337.h"

void ds1337_INIT(){
	if(I2C2_CONFIGURED==0){
		InitI2C2();
	}
}
void ds1337_writeRecordToRegister(uint8_t adress,uint8_t value){
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){}

	I2C_Send7bitAddress(I2C2,DS1337_ADRESS<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){}

	I2C_SendData(I2C2,adress);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}

	I2C_SendData(I2C2,value);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}
	I2C_GenerateSTOP(I2C2,ENABLE);

}

void ds1337_writeRecordsToRegisters(uint8_t beginAdress,uint8_t * array,uint8_t size){
	I2C_GenerateSTART(I2C2,ENABLE);

	I2C_AcknowledgeConfig(I2C2,ENABLE);
	I2C_GenerateSTOP(I2C2,DISABLE);

	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){}

	I2C_Send7bitAddress(I2C2,DS1337_ADRESS<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){}

	I2C_SendData(I2C2,beginAdress);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}

	uint8_t loopCounter=0;
	for(;loopCounter<size;loopCounter++){
		I2C_SendData(I2C2,array[loopCounter]);
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}
	}
	I2C_GenerateSTOP(I2C2,ENABLE);
}

void ds1337_readRecordsFromRegisters(uint8_t beginAdress,uint8_t * array,uint8_t size){

	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)){}

	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){}

	I2C_Send7bitAddress(I2C2,DS1337_ADRESS<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){}

	I2C_SendData(I2C2,beginAdress);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}

	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS){}

	I2C_AcknowledgeConfig(I2C2,ENABLE);
	I2C_GenerateSTOP(I2C2,DISABLE);

	I2C_Send7bitAddress(I2C2,DS1337_ADRESS<<1,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS){}

	uint8_t loopCounter=0;
	for(;loopCounter<size;loopCounter++){
		if(loopCounter==(size-1)){
			I2C_AcknowledgeConfig(I2C2,DISABLE);
			I2C_GenerateSTOP(I2C2,ENABLE);
		}

		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS){}
		array[loopCounter]=I2C_ReceiveData(I2C2);
	}
}

void ds1337_readRecordsFromRegister(uint8_t adress,uint8_t * ptrToBuffer){
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){}

	I2C_Send7bitAddress(I2C2,DS1337_ADRESS<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){}

	I2C_SendData(I2C2,adress);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS){}

	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS){}

	I2C_AcknowledgeConfig(I2C2,ENABLE);
	I2C_GenerateSTOP(I2C2,DISABLE);

	I2C_Send7bitAddress(I2C2,DS1337_ADRESS<<1,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS){}

	I2C_AcknowledgeConfig(I2C2,DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);

	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS){}
	*ptrToBuffer=I2C_ReceiveData(I2C2);
}
void ds1337_writeTimeStructure(){
	uint8_t buffer[7];

	buffer[0]=((DS1337_TIME.seconds/10)<<4)|(DS1337_TIME.seconds%10);
	buffer[1]=((DS1337_TIME.minutes/10)<<4)|(DS1337_TIME.minutes%10);
	buffer[2]=((DS1337_TIME.hour/10)<<4)|(DS1337_TIME.hour%10);
	buffer[3]=DS1337_TIME.day;
	buffer[4]=((DS1337_TIME.month/10)<<4)|(DS1337_TIME.month%10);
	buffer[5]=((DS1337_TIME.date/10)<<4)|(DS1337_TIME.date%10);
	buffer[6]=((DS1337_TIME.decade/10)<<4)|(DS1337_TIME.decade%10);

	ds1337_writeRecordsToRegisters(DS1337_BEGIN_OF_TIME_REGISTER,buffer,7);
}

void ds1337_readTimeStructure(){
	uint8_t buffer[7];
	ds1337_readRecordsFromRegisters(DS1337_BEGIN_OF_TIME_REGISTER,buffer,7);
	DS1337_TIME.seconds=(buffer[0]&(0b00001111))+((buffer[0]>>4)&0b00000111)*10;
	DS1337_TIME.minutes=(buffer[1]&(0b00001111))+((buffer[1]>>4)&0b00000111)*10;
	DS1337_TIME.hour=(buffer[2]&(0b00001111))+((buffer[2]>>4)&0b00000011)*10;
	DS1337_TIME.day=buffer[3]&0b00000111;
	DS1337_TIME.date=(buffer[4]&(0b00001111))+((buffer[4]>>4)&0b00000011)*10;
	DS1337_TIME.month=(buffer[5]&(0b00001111))+((buffer[5]>>4)&0b00000001)*10;
	DS1337_TIME.decade=(buffer[6]&(0b00001111))+((buffer[6]>>4)&0b00001111)*10;
}
