/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed "as is", without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_ioe.h"
#include "DHT11.h"
#include "ds18b20.h"
#include "LPS331.h"
#include "SharedFunctions.h"
#include "MainMenu.h"
#include "Results.h"

#define MEASURE_INTERVAL_MS 2000
static unsigned counter;

void delayDec(void)
{
    if(counter>0){
        counter--;
    }
    else{
		collectDataFromSensors();
    	counter=MEASURE_INTERVAL_MS;
    }
}

void SysTick_Handler(void)
{
   delayDec();
}

int main(void)
{
   LPS331_INIT();
   LCDinit();
   DMArecordPushInit();
   results.sampleNumber=0;

   counter=MEASURE_INTERVAL_MS;

   SysTick_Config(SystemCoreClock / 1000);
  /* Infinite loop */
   while (1)
   {
	  MainMenu();
   }
}




/*
 * Callback used by stm324xg_eval_i2c_ee.c.
 * Refer to stm324xg_eval_i2c_ee.h for more info.
 */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  while (1)
  {
  }
}
