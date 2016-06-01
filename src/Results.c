#include "Results.h"

void collectDataFromSensors(){
	results.temperatureAndHumidity=DHT11_read();
	results.pressure=LPS331_readPressure();
	results.externalTemperature=ds18b20_read();
	results.changed=1;
	ds18b20_beginConversion();
	results.sampleNumber++;

	pushRecord();
}


void pushRecord(){
	DMA_Cmd(DMA2_Stream0, ENABLE);
	DMA_Cmd(DMA2_Stream1, ENABLE);
	DMA_Cmd(DMA2_Stream2, ENABLE);
	DMA_Cmd(DMA2_Stream3, ENABLE);

	while (DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0) == RESET ||
			DMA_GetFlagStatus(DMA2_Stream1,DMA_FLAG_TCIF1) == RESET ||
			DMA_GetFlagStatus(DMA2_Stream2,DMA_FLAG_TCIF2) == RESET ||
			DMA_GetFlagStatus(DMA2_Stream3,DMA_FLAG_TCIF3) == RESET
	);
	DMA_ClearFlag(DMA2_Stream0,DMA_FLAG_TCIF0);
	DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);
	DMA_ClearFlag(DMA2_Stream2,DMA_FLAG_TCIF2);
	DMA_ClearFlag(DMA2_Stream3,DMA_FLAG_TCIF3);


	results.humidityHistory[BUFFER_SIZE-1]=results.temperatureAndHumidity.humidity;
	results.pressureHistory[BUFFER_SIZE-1]=results.pressure.pressure;
	results.temperaturesHistory[BUFFER_SIZE-1]=results.temperatureAndHumidity.temperature;
	results.extTemperaturesHistory[BUFFER_SIZE-1]=results.externalTemperature.temperature;
}

void DMArecordPushInit(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

	DMA_InitTypeDef dma;

	DMA_StructInit(&dma);

	dma.DMA_Channel = DMA_Channel_0;
	dma.DMA_Memory0BaseAddr = (uint32_t)(results.temperaturesHistory);
	dma.DMA_PeripheralBaseAddr = (uint32_t)(results.temperaturesHistory+1);
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	dma.DMA_PeripheralDataSize = DMA_MemoryDataSize_Word;
	dma.DMA_DIR = DMA_DIR_MemoryToMemory;
	dma.DMA_Mode = DMA_Mode_Normal;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	dma.DMA_BufferSize = (BUFFER_SIZE-1)*sizeof(float);
	dma.DMA_Priority = DMA_Priority_High;
	dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	dma.DMA_FIFOMode = DMA_FIFOMode_Disable;

	DMA_Init(DMA2_Stream0, &dma);

	dma.DMA_Channel = DMA_Channel_1;
	dma.DMA_Memory0BaseAddr = (uint32_t)(results.pressureHistory);
	dma.DMA_PeripheralBaseAddr = (uint32_t)(results.pressureHistory+1);

	DMA_Init(DMA2_Stream1, &dma);

	dma.DMA_Channel = DMA_Channel_2;
	dma.DMA_Memory0BaseAddr = (uint32_t)(results.humidityHistory);
	dma.DMA_PeripheralBaseAddr = (uint32_t)(results.humidityHistory+1);

	DMA_Init(DMA2_Stream2, &dma);

	dma.DMA_Channel = DMA_Channel_3;
	dma.DMA_Memory0BaseAddr = (uint32_t)(results.extTemperaturesHistory);
	dma.DMA_PeripheralBaseAddr = (uint32_t)(results.extTemperaturesHistory+1);

	DMA_Init(DMA2_Stream3, &dma);

}
