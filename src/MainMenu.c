/*
 * MainMenu.c
 *
 *  Created on: May 6, 2016
 *      Author: krzys
 */
#include "MainMenu.h"

void LCDinit(){
	LCD_Init();
	LCD_LayerInit();
	LCD_SetLayer(LCD_BACKGROUND_LAYER);
	LCD_SetTransparency(0);
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LTDC_ReloadConfig(LTDC_IMReload);
	LTDC_Cmd(ENABLE);
	LCD_Clear(LCD_COLOR_BLACK);
}
void MainMenu(){
	Button configuration={5,220,"  Configure   "};
	Button chart={5,270,"    Chats     "};

	Label temperatureLabel={10,10,"Temperature[C]:"};
	Label humidityLabel={10,50,"Humidity[%]:"};
	Label pressureLabel={10,90,"Pressure[hPa]:"};
	Label externalTemperatureLabel={10,130,"Ext Temperature[C]:"};

	float data[]={1,10,15,1,4,3,4,5};

	ButtonDraw(&configuration);
	ButtonDraw(&chart);
	LabelDraw(&temperatureLabel);
	LabelDraw(&humidityLabel);
	LabelDraw(&pressureLabel);
	LabelDraw(&externalTemperatureLabel);

	IOE_TP_Config();
	TP_STATE state;

	while(1){
		state=*IOE_TP_GetState();
		if (state.TouchDetected == 128 ) {
			int s=state.TouchDetected;
			if(ButtonCheckIfPressed(state.X,state.Y,&chart)==CLICKED){
				state.TouchDetected=0;
				results.changed=1;
				ChartsMenu();
				results.changed=1;
			}
			state.TouchDetected=0;

		}
		if(results.changed){
			LCD_SetColors(0x0000,0xffff);
			LCD_DrawFullRect(0,0,240,320);
			ButtonDraw(&configuration);
			ButtonDraw(&chart);

			if(results.temperatureAndHumidity.ERROR_CODE==0){
				LabelDraw(&temperatureLabel);
				LabelDraw(&humidityLabel);
				DrawFloat(150,30,(float)results.temperatureAndHumidity.temperature,2);
				DrawFloat(150,70,(float)results.temperatureAndHumidity.humidity,2);
			}
			else{
				Label tmp=temperatureLabel;
				tmp.string="DHT_11 Error";
				LabelDraw(&tmp);
			}

			if(results.externalTemperature.ERROR_CODE==0){
				LabelDraw(&externalTemperatureLabel);
				DrawFloat(150,150,(float)results.externalTemperature.temperature,2);
			}
			else{
				Label tmp=externalTemperatureLabel;
				tmp.string="DS18B20 Error";
				LabelDraw(&tmp);
			}

			if(results.pressure.ERROR_CODE==0){
				LabelDraw(&pressureLabel);
				DrawFloat(150,110,(float)results.pressure.pressure,2);
			}
			else{
				Label tmp=pressureLabel;
				tmp.string="LPS331 Error";
				LabelDraw(&tmp);
			}

			results.changed=0;
		}
		delayMSC(100);
	}
}
