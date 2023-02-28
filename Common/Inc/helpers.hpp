#pragma once


#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

//uint32_t x, y;
//uint32_t Cycles;
//
//volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004;
//volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000;
//volatile unsigned int *DWT_LAR      = (volatile unsigned int *)0xE0001FB0;
//volatile unsigned int *SCB_DHCSR    = (volatile unsigned int *)0xE000EDF0;
//volatile unsigned int *SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC;
//volatile unsigned int *ITM_TER      = (volatile unsigned int *)0xE0000E00;
//volatile unsigned int *ITM_TCR      = (volatile unsigned int *)0xE0000E80;
//
//static int Debug_ITMDebug = 0;


#define ACCEL_D3_ADDRESS 0x38000000
#define GYRO_D3_ADDRESS  0x38000010  //0x38000040
#define MAG_D3_ADDRESS 	 0x38000020 //0x38000080
#define BARO_D3_ADDRESS	 0x38000030          // 0x380000C0
#define GPS_D3_ADDRESS   0x38000040         //0x38000100

static constexpr uint8_t USB_UPDATE_RATE_MS     =       7;      // 100 Hz Update Rate
static constexpr uint16_t HEART_BEAT_RATE_MS     =       1000;    // 1 Hz
static constexpr uint8_t GPS_UPDATE_RATE_MS     =       100;     // 10 Hz
static constexpr uint8_t BARO_UPDATE_RATE_MS    =       38;      //~26 Hz
static constexpr uint8_t MAG_UPDATE_RATE_MS     =       10;      // ~80Hz
static constexpr uint8_t GYRO_UPDATE_RATE_MS    =       10;      // ~952 Hz
static constexpr uint8_t ACC_UPDATE_RATE_MS     =       10;       // ~952 Hz
static constexpr uint8_t MAX_USB_RETRY_COUNT    = 1;

static constexpr uint8_t SEM_MASK_ACC = 2;
static constexpr uint8_t SEM_MASK_GYRO = 4;
static constexpr uint8_t SEM_MASK_MAG = 8;
static constexpr uint8_t SEM_MASK_BARO = 16;
static constexpr uint8_t SEM_MASK_GPS = 32;

#define US_IN_SECOND  					((uint32_t)1000000)

static constexpr double MICROS_PER_SEC = 1000000.0f;

namespace mathformulas {
 static constexpr float pi = 3.141592653589793;
 static constexpr float gravity = 9.80665f;
}

//inline void EnableTiming(void)
//{
//  if ((*SCB_DHCSR & 1) && (*ITM_TER & 1)) // Enabled?
//    Debug_ITMDebug = 1;
//
//  *SCB_DEMCR |= 0x01000000;
//  *DWT_LAR = 0xC5ACCE55; // enable access
//  *DWT_CYCCNT = 0; // reset the counter
//  *DWT_CONTROL |= 1 ; // enable the counter
//}


namespace mathconversions{

static constexpr float rad_to_deg = 57.295779513;
static constexpr float deg_to_rad = 0.01745329252;
static constexpr uint8_t gauss_to_microtesla = 100;
static constexpr uint32_t us_in_second = 1000000;

}

namespace ledcontrol{

inline void LEDAOn ()
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
}

inline void LEDAOff ()
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
}

inline void LEDBOn ()
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
}

inline void LEDBOff ()
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
}

inline void LEDCOn ()
{
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);
}

inline void LEDCOff ()
{
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET);
}

inline void LEDDOn ()
{
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
}

inline void LEDDOff ()
{
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
}

}


namespace sampletime
{
  static constexpr uint8_t barometer = 38; // 26.3Hz
}


static constexpr void TimerCountStart()
{
   do
   {
     SysTick->LOAD  =  0xFFFFFF  ; /* set reload register */
     SysTick->VAL  =  0  ;     /* Clear Counter */
     SysTick->CTRL  =  0x5 ;     /* Enable Counting*/
   }

   while(0);
}

__attribute__((unused)) static uint32_t TimerCountStop()
{
	uint32_t value;
   do
   {
     SysTick->CTRL  =0;  /* Disable Counting */
     value = SysTick->VAL;/* Load the SysTick Counter Value */
     value = 0xFFFFFF - value;/* Capture Counts in CPU Cycles*/

   }

   while(0);

   return value;
}

/*
 * Accelerometer data
 */
struct gyroData
{
  float x;
  float y;
  float z;
};

struct magData
{
  float x;
  float y;
  float z;
};

 struct accelData
 {
   float x;
   float y;
   float z;
 };

 struct gpsData
 {
     float gps_latitude;
     float gps_longitude;
     float gps_altitude;
     float gps_velocity_x;
     float gps_velocity_y;
     float gps_velocity_z;
     float gnd_speed;
     unsigned long iTOW;
     short magDec;
     uint8_t  gps_satellites;
 }__attribute__((__packed__));;

 struct baroData
 {
     float baro_pressure;
     float baro_altitude;
 };

