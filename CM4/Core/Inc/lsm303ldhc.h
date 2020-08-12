#ifndef LSM303DLHC_H
#define LSM303DLHC_H

#include <stdint.h>
#include<stm32h7xx_hal.h>
#include "i2c.h"
#include "usart.h"

/* I2C Addresses */
#define LSM303_ACC_ADDR    0x32   //  0011001x Last Bit R/W
#define LSM303_MAG_ADDR    0x3C   //  0011110x
#define LSM303_REG_WHO_AM_I 0x0F


// Linear Acceleration registers
typedef enum {
	CTRL_REG1_A  =  0x20,  // Control Register 1 Acceleration rw
	CTRL_REG2_A  =  0x21,  // Control Register 2 Acceleration rw
	CTRL_REG3_A  =  0x22,  // Control Register 3 Acceleration rw
	CTRL_REG4_A  =  0x23,  // Control Register 4 Acceleration rw
	CTRL_REG5_A  =  0x24,  // Control Register 5 Acceleration rw
	CTRL_REG6_A  =  0x25,  // Control Register 6 Acceleration rw
	REFERENCE_A  =  0x26, // Reference Register rw
	STATUS_REG_A =  0x27, // STatus Register r
	OUT_X_L_A    =   0x28, //
	OUT_X_H_A    =  0x29, //
	OUT_Y_L_A 	 =  0x2A, //
	OUT_Y_H_A 	 =  0x2B, //
	OUT_Z_L_A 	 =  0x2C, //
	OUT_Z_H_A    =  0x2D,
	FIFO_CTRL_REG_A = 0x2E,
	FIFO_SRC_REG_A = 0x2F,
	INT1_CFG_A    = 0x30,
	INT1_SOURCE_A = 0x31,
	INT1_THS_A  = 0x32,
	INT1_DURATION_A =  0x33,
	INT2_CFG_A    =0x34,
	INT2_SOURCE_A   =0x35,
	INT2_THS_A  = 0x36,
	INT2_DURATION_A  = 0x37,
	CLICK_CFG_A  = 0x38,
	CLICK_SRC_A  = 0x39,
	CLICK_THS_A  = 0x3A,
	TIME_LIMIT_A  = 0x3B,
	TIME_LATENCY_A =  0x3C,
	TIME_WINDOW_A  = 0x3D

} LSM303AccelRegisters;

// Options for CTRL_REG1_A Output Data Rate
typedef enum{
	ODR_POWER_DOWN = ((uint8_t)0x00),
	ODR_1HZ = ((uint8_t)0x10),
	ODR_10HZ = ((uint8_t)0x20),
	ODR_25HZ =  ((uint8_t)0x30),
	ODR_50HZ =  ((uint8_t)0x40),
	ODR_100HZ =  ((uint8_t)0x50),
	ODR_200HZ =  ((uint8_t)0x60),
	ODR_400HZ =  ((uint8_t)0x70),
	ODR_1620HZ_LP =  ((uint8_t)0x80),
	ODR_1344HZ =  ((uint8_t)0x90)

} DataRateAccConfig;

typedef enum{
	NORMAL_MODE = ((uint8_t)0x00),
	LP_MODE = ((uint8_t)0x08)
}PowerModeConfig;

typedef enum{
	X_EN = ((uint8_t)0x01),
	Y_EN = ((uint8_t)0x02),
	Z_EN = ((uint8_t)0x04),
	AXES_EN = ((uint8_t)0x07),
	AXES_DISABLE = ((uint8_t)0x00),

}AxesConfig;


// Option for CTRL_REG2_A High Pass FIlter Mode
typedef enum{
	HPM_NORMAL_RESET = ((uint8_t)0x00),
	HPM_REF_SIGNAL = ((uint8_t)0x40),
	HPM_NORMAL = ((uint8_t)0x80),
	HPM_AUTORESET = ((uint8_t)0xC0)
} HPMConfig;

typedef enum{
	HPF_AO1_DISABLE = ((uint8_t) 0x00),
	HPF_AO1_ENABLE = ((uint8_t) 0x01)
}HPFAOI1Config;

typedef enum{
	HPF_AO2_DISABLE = ((uint8_t) 0x00),
	HPF_AO2_ENABLE = ((uint8_t) 0x02)
}HPFAOI2Config;

// CTRL_REG4_A Options
typedef enum{
	BDU_CONTINUOUS = ((uint8_t)0x00),
	BDU_SINGLE = ((uint8_t)0x80),
	BLE_LSB = ((uint8_t)0x00),
	BLE_MSB = ((uint8_t)0x40),
	BLE = ((uint8_t)0x00),
	FS_2G = ((uint8_t)0x00),
	FS_4G = ((uint8_t)0x10),
	FS_8G = ((uint8_t)0x20),
	FS_16G = ((uint8_t)0x30),
	HR_EN = ((uint8_t)0x08),
	HR_DISABLE = ((uint8_t)0x00),
	SIM = ((uint8_t)0x00),

}AccReg4Config;

typedef enum{
	SENS_2G = ((uint8_t)1),
	SENS_4G = ((uint8_t)2),
	SENS_8G = ((uint8_t)4),
	SENS_16G = ((uint8_t)12)
}AccSensitivity;

// Reg 5 Contents
typedef enum{
	BOOT_NORMAL = ((uint8_t)0x00),
	BOOT_REBOOTMEM = ((uint8_t)0x80),
	FIFO_EN = ((uint8_t)0x40),
	FIFO_DISABLE = ((uint8_t)0x00)
}AccReg5Config;

typedef enum{
	HPCF_8 = ((uint8_t)0x00),
	HPCF_16 = ((uint8_t)0x10),
	HPCF_32 = ((uint8_t)0x20),
	HPCF_64= ((uint8_t)0x30)
}HPFCutOff;

typedef enum{
	HPF_ENABLE = ((uint8_t)0x00),
	HPF_DISABLE = ((uint8_t)0x08)
}HPFMode;


/***************************MAgnetometer Register Data */

typedef enum{
	CRA_REG_M  = 	0x00,
	CRB_REG_M  = 	0x01,
	MR_REG_M  = 	0x02,
	OUT_X_H_M  = 	0x03,
	OUT_X_L_M =  	0x04,
	OUT_Z_H_M  = 	0x05,
	OUT_Z_L_M  =  	0x06,
	OUT_Y_H_M  = 	0x07,
	OUT_Y_L_M  = 	0x08,
	SR_REG_MG  = 	0x09,
	IRA_REG_M  = 	0x0A,
	IRB_REG_M  = 	0x0B,
	IRC_REG_M  = 	0x0C,
	TEMP_OUT_H_M = 	0x31,
	TEMP_OUT_L_M = 	0x32

}LSM303MagRegisters;


typedef enum{
	ODR_0_75HZ = ((uint8_t)0x00),
	ODR_1_5HZ = ((uint8_t)0x04),
	ODR_3HZ =  ((uint8_t)0x08),
	ODR_7_5HZ =  ((uint8_t)0x0C),
	ODR_15HZ =  ((uint8_t)0x10),
	ODR_30HZ =  ((uint8_t)0x14),
	ODR_75HZ =  ((uint8_t)0x18),
	ODR_220HZ =  ((uint8_t)0x1C)
} DataRateMagConfig;

typedef enum
{
  LSM303_MAGGAIN_1_3                        = 0x20,  // +/- 1.3
  LSM303_MAGGAIN_1_9                        = 0x40,  // +/- 1.9
  LSM303_MAGGAIN_2_5                        = 0x60,  // +/- 2.5
  LSM303_MAGGAIN_4_0                        = 0x80,  // +/- 4.0
  LSM303_MAGGAIN_4_7                        = 0xA0,  // +/- 4.7
  LSM303_MAGGAIN_5_6                        = 0xC0,  // +/- 5.6
  LSM303_MAGGAIN_8_1                        = 0xE0   // +/- 8.1
} MagGain;

typedef enum{
 M_SENSITIVITY_XY_1_3Ga   =  1100,  /*!< magnetometer X Y axes sensitivity for 1.3 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_XY_1_9Ga   =  855 ,  /*!< magnetometer X Y axes sensitivity for 1.9 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_XY_2_5Ga   =  670 ,  /*!< magnetometer X Y axes sensitivity for 2.5 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_XY_4Ga     =  450,   /*!< magnetometer X Y axes sensitivity for 4 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_XY_4_7Ga   = 400 ,  /*!< magnetometer X Y axes sensitivity for 4.7 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_XY_5_6Ga   =  330 ,  /*!< magnetometer X Y axes sensitivity for 5.6 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_XY_8_1Ga   =  230 ,  /*!< magnetometer X Y axes sensitivity for 8.1 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_Z_1_3Ga    =  980 ,  /*!< magnetometer Z axis sensitivity for 1.3 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_Z_1_9Ga    =  760,   /*!< magnetometer Z axis sensitivity for 1.9 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_Z_2_5Ga    =  600 ,  /*!< magnetometer Z axis sensitivity for 2.5 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_Z_4Ga      =  400 ,  /*!< magnetometer Z axis sensitivity for 4 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_Z_4_7Ga    =  355 ,  /*!< magnetometer Z axis sensitivity for 4.7 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_Z_5_6Ga    =  295 ,  /*!< magnetometer Z axis sensitivity for 5.6 Ga full scale [LSB/Ga] */
 M_SENSITIVITY_Z_8_1Ga    =  205   /*!< magnetometer Z axis sensitivity for 8.1 Ga full scale [LSB/Ga] */
}MagSensitivity;

typedef enum{
	CONTINUOUS_CONVERSION = 0x00,
	SINGLE_CONVERSION = 0x01,
	SLEEP = 0x02
}MagOperatingMode;

typedef enum{
	TEMP_ENABLE = 0x80,
	TEMP_DISABLE = 0x00
}TempSensor;

typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
}LSM303AccData;

HAL_StatusTypeDef ret;

void  LSM303AccInit();
void  LSM303MagInit(void);
uint8_t  ReadSensorID (uint16_t DeviceAddr, uint8_t reg);

void LSM303Write(uint16_t DeviceAddr, uint8_t reg, uint8_t value);
uint8_t LSM303Read(uint16_t DeviceAddr, uint8_t reg);
void LSM303StartUp(void);
void FilterConfig(uint8_t FilterStruct);
void AccGetXYZ(int16_t* pData);
//void LSM303ReadAcc(LSM303AccData* data);
void LSM303ReadAcc(int16_t* pData);

#endif
