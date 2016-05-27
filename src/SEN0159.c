/*
 * SEN0159.c
 *
 *  Created on: May 27, 2016
 *      Author: krzys
 */
#include "SEN0159.h"
#define         ZERO_POINT_X                 (2.602) //lg400=2.602, the start point_on X_axis of the curve
#define         ZERO_POINT_VOLTAGE           (0.324) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         MAX_POINT_VOLTAGE            (0.265) //define the output of the sensor in volts when the concentration of CO2 is 10,000PPM
#define         REACTION_VOLTGAE             (0.059) //define the voltage sensor when move the sensor from air into 1000ppm CO2
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier
float           CO2Curve[3]  =  {ZERO_POINT_X, ZERO_POINT_VOLTAGE, (REACTION_VOLTGAE / (2.602 - 4))};

void SEN0159init(){

	 //Clock configuration
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);

	 // pin configuration
	 GPIO_InitTypeDef GPIO_initStructre;
	 GPIO_initStructre.GPIO_Pin = GPIO_Pin_0;//channel 10 is connected to PC0
	 GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN;
	 GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_Init(GPIOC,&GPIO_initStructre);

	 // adc configuration
	 ADC_InitTypeDef ADC_init_structure;
	 ADC_DeInit();
	 ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;
	 ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;
	 ADC_init_structure.ADC_ContinuousConvMode = DISABLE;
	 //ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	 ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	 ADC_init_structure.ADC_NbrOfConversion = 1;
	 ADC_init_structure.ADC_ScanConvMode = DISABLE;
	 ADC_Init(ADC1,&ADC_init_structure);

	 //Enable ADC conversion
	 ADC_Cmd(ADC1,ENABLE);

	 //Select the channel to be read from
	 ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_480Cycles);
}

int SEN0159Result(){
    ADC_SoftwareStartConv(ADC1);//Start the conversion
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
    float StartVolts = ((float)ADC_GetConversionValue(ADC1)/4096.0)*6;
    float volts = StartVolts / DC_GAIN;
    if (volts > ZERO_POINT_VOLTAGE || volts < MAX_POINT_VOLTAGE ) {
      return -1;
    } else {
      return pow(10, (volts - CO2Curve[1]) / CO2Curve[2] + CO2Curve[0]);
      volts = 0;
    }
}
