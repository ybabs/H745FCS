#ifndef COMMON_H
#define COMMON_H

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"



// Semaphores







// LEDS //
#define LD1_Pin 		GPIO_PIN_0
#define LD1_GPIO_Port   GPIOB
#define LD3_Pin 		GPIO_PIN_14
#define LD3_GPIO_Port 	GPIOB
#define LD2_Pin 		GPIO_PIN_1
#define LD2_GPIO_Port 	GPIOE



#define LED3_ON()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)
#define LED3_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)


#define LED2_ON()  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET)
#define LED2_OFF() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET)


#define LED1_ON()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define LED1_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)









#endif
