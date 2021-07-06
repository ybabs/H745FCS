/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"


const uint32_t LED_DT_MS = 1000;

// HeartBeatTask
osThreadId_t heartbeatTaskHandle;
const osThreadAttr_t heartbeatTask_attributes = {
		.name = "heartbeatTask",
		.priority = (osPriority_t) osPriorityLow,
		.stack_size = 128 * 4
};

void StartLEDTask(void *argument);


void MX_FREERTOS_Init(void)
{

    heartbeatTaskHandle = osThreadNew(StartLEDTask, NULL, &heartbeatTask_attributes);

}


void StartLEDTask(void *argument)
{
	for(;;)
	{
		HAL_GPIO_TogglePin(LED_A_GPIO_Port, LED_A_Pin);
		osDelay(LED_DT_MS);
	}

}
