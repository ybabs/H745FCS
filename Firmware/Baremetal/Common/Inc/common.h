#ifndef COMMON_H
#define COMMON_H
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

#define GPS_DATA_TYPE 0x01
#define MAG_DATA_TYPE 0x02
#define ACC_DATA_TYPE 0x03
#define GYRO_DATA_TYPE 0x04
#define BARO_DATA_TYPE 0x05

 // Update Rates of sensors in milliseconds
const uint32_t GPS_SAMPLE_TIME =  100; // GPS has 10HZ update rate
const uint32_t MAG_SAMPLE_TIME =  13; // 80 Hz
const uint32_t IMU_SAMPLE_TIME =  1; // 952HZ
const uint32_t BARO_SAMPLE_TIME = 38; // 26.3 Hz

#define MEM_ALIGN(x)   (((x) + 0x00000003) & ~0x00000003)

#define SRAM4_ADDR 0x38000000
#define SRAM4_LEN  0x0000FFFF
#define IMU_PACKET_LEN (MEM_ALIGN(sizeof(LSM9DS1Handle)))
#define GPS_PACKET_LEN (MEM_ALIGN(sizeof(GPSHandle)))
#define BARO_PACKET_LEN (MEM_ALIGN(sizeof(BMP280Handle)))

// define Addresses
#define M4_TO_M7_ADDR MEM_ALIGN(SRAM4_ADDR)
#define IMU_BUFFER      MEM_ALIGN(M4_TO_M7_ADDR + IMU_PACKET_LEN)

#define US_IN_SECOND            ((uint32_t)1000000)
#define TimerCount_Start()  do{\
              SysTick->LOAD  =  0xFFFFFF  ; /* set reload register */\
              SysTick->VAL  =  0  ;     /* Clear Counter */    \
              SysTick->CTRL  =  0x5 ;     /* Enable Counting*/   \
              }while(0)

#define TimerCount_Stop(Value)  do {\
                SysTick->CTRL  =0;  /* Disable Counting */         \
                Value = SysTick->VAL;/* Load the SysTick Counter Value */\
                Value = 0xFFFFFF - Value;/* Capture Counts in CPU Cycles*/\
                }while(0)



#endif