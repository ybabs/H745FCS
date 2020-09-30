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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "common.h"
#include "lsm303ldhc.h"

// Sensors //
Acceleration* accData;
Gauss* magData;

//#include "ublox.h"
//#include "usart.h"
//uint16_t duty_cycle = 1060;
//Motor motor_pwm;
//LSM303AccData* data;
//int16_t data[3];
//bmp180Sensor bmpSensor ;
////int16_t mag_data[3];
//uint8_t buf[30];
////float_t temp;
//float_t temp;
//float_t pressure;
//float_t altitude;
//float_t data[3];

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */
// Sample times
const uint32_t ACC_DT_MS =  3;
const uint32_t MAG_DT_MS = 5;

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

osThreadId_t magTaskHandle;
const osThreadAttr_t magTask_attributes = {
		.name = "magTask",
		.priority = (osPriority_t) osPriorityRealtime,
		.stack_size = 128 * 4
};

osThreadId_t accTaskHandle;
const osThreadAttr_t accTask_attributes = {
		.name = "accTask",
		.priority = (osPriority_t) osPriorityRealtime,
		.stack_size = 128 * 4
};


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartMagTask(void  *argument);
void StartAccTask(void  *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	//BMP180InitDevice(&bmpSensor);
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  accTaskHandle = osThreadNew(StartAccTask, NULL, &accTask_attributes);
  magTaskHandle = osThreadNew(StartMagTask, NULL, &magTask_attributes);


  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
//	  motor_pwm.motor1 = duty_cycle;
//	  SetMotorPWM(&motor_pwm);
//	  duty_cycle+=50;
//	 if(duty_cycle > 1900)
//	 {
//		 duty_cycle = 1000;
//	 }
	  //LSM303ReadAcc(data);
	//  L3GD20ReadXYZAngRate(data);
	  //LSM303ReadMag(mag_data);
 	 //temp =  LSM303GetTemp();
	  //temp = L3GD20GetTemp();
      //temp*=100;
//	  BMP180ReadTemperature(&bmpSensor);
//	  temp = bmpSensor.temperature;

	  //float temp = data[0];
	  //temp*=100;
      //temp = L3GD20Startup();
	  //snprintf((char*)buf, sizeof(buf), "X: %.2f, Y: %.2f Z: %.2f", data[0], data[1], data[2]);
     	// sprintf ((char*)buf, "X:%f Y:%f Z:%f \r\n", data[0], data[1], data[2]);
//	 // sprintf ((char*)buf, "X:% 06d Y:% 06d Z:% 06d \r\n", mag_data[0], mag_data[1], mag_data[2]);
      //sprintf ((char*)buf, ":%u.%02u \r\n", (unsigned int) temp/100, (unsigned int)temp % 100);
//
      //HAL_UART_Transmit(&huart3, buf, strlen((char*)buf), HAL_MAX_DELAY);
	  	//LED2_ON();

    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void StartMagTask(void *argument)
{
	for(;;)
	{
		LSM303GetGauss(magData);
		osDelay(MAG_DT_MS);
	}
}

void StartAccTask(void *argument)
{
	for(;;)
	{
		LSM303GetGs(accData);
		osDelay(ACC_DT_MS);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
