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
#include "DHT11.h"
#include "ds18b20.h"
#include "SharedFunctions.h"
int main(void)
{
  int i = 0;
  /* Infinite loop */
  while (1)
  {
	  ds18b20_beginConversion();
	  InitTimerTIM2ToCountInUc();
	  // start timer
	  TIM2->CR1 |= 0x01;
	  TIM2->CNT=0;
	  while(TIM2->CNT<1000000){
		  __NOP();
	  }
	  ds18b20_read();
  }
}

/*
 * Callback used by stm324xg_eval_i2c_ee.c.
 * Refer to stm324xg_eval_i2c_ee.h for more info.
 */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* TODO, implement your code here */
  while (1)
  {
  }
}
