#pragma once


#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"


static constexpr uint8_t GPS_DATA_TYPE = 0x01;
static constexpr uint8_t MAG_DATA_TYPE = 0x02;
static constexpr uint8_t ACC_DATA_TYPE = 0x03;
static constexpr uint8_t GYRO_DATA_TYPE = 0x04;
static constexpr uint8_t BARO_DATA_TYPE  = 0x05;

static constexpr uint8_t USB_UPDATE_RATE_MS     =       10;      // 100 Hz Update Rate
static constexpr uint16_t HEART_BEAT_RATE_MS     =       1000;    // 1 Hz
static constexpr uint8_t GPS_UPDATE_RATE_MS     =       100;     // 10 Hz
static constexpr uint8_t BARO_UPDATE_RATE_MS    =       38;      //~26 Hz
static constexpr uint8_t MAG_UPDATE_RATE_MS     =       12;      // ~80Hz
static constexpr uint8_t GYRO_UPDATE_RATE_MS    =       1;      // ~952 Hz
static constexpr uint8_t ACC_UPDATE_RATE_MS     =       1;       // ~952 Hz


static constexpr uint32_t HSEM_ID_1 = 0;

static constexpr double MICROS_PER_SEC = 1000000.0f;

namespace mathformulas {
 static constexpr float pi = 3.141592653589793;
 static constexpr float gravity = 9.80665f;
}


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
 struct acc_data
 {
     float imu_acc_x;
     float imu_acc_y;
     float imu_acc_z;
 };

 struct gyro_data
 {
     float imu_gyro_x;
     float imu_gyro_y;
     float imu_gyro_z;
 };


 struct mag_data
 {
     float imu_mag_x;
     float imu_mag_y;
     float imu_mag_z;
 };

 struct gps_data
 {
     float gps_latitude;
     float gps_longitude;
     float gps_altitude;
     float gps_velocity_x;
     float gps_velocity_y;
     float gps_velocity_z;
     float gnd_speed;
     int   gps_satellites;
 };

 struct baro_data
 {
     float baro_pressure;
     float baro_altitude;
 };

