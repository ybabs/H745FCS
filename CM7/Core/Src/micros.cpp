/*
 * micros.cpp
 *
 *  Created on: 15 Sep 2022
 *      Author: Home
 */
#include <micros.hpp>
#include <stm32h745xx.h>



void DWT_Init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

 void delay_us(uint32_t us)
{
  uint32_t us_count_tic =  us * (SystemCoreClock / 1000000U);
  DWT->CYCCNT = 0U;
  while(DWT->CYCCNT < us_count_tic);
}

 uint32_t micros(void){
  return  DWT->CYCCNT / (SystemCoreClock / 1000000U);
}



