#ifndef L3GD20_H
#define L3GD20_H

#include <stdint.h>
#include<stm32h7xx_hal.h>
#include "i2c.h"

#define L3GD20_ADDRESS   ((uint8_t)0xD7)
#define L3GD20_HIGH_ADDRESS   ((uint8_t)0x6A)
#define I_AM_L3GD20      ((uint8_t)0xD4)
#define I_AM_L3GD20H   ((uint8_t)0xD7)
#define L3GD20_DPS_TO_RADS         (0.017453293F)  // degress/s to rad/s multiplier

//
#define GYRO_SENSITIVITY_250DPS (0.00875F) //!< Sensitivity at 250 dps
#define GYRO_SENSITIVITY_500DPS (0.0175F)  //!< Sensitivity at 500 dps
#define GYRO_SENSITIVITY_2000DPS (0.070F)  //!< Sensitivity at 2000 dps

typedef enum
{
	WHO_AM_I = 0x0F,
	CTRL1_REG =  0x20,
	CTRL2_REG  = 0x21,
	CTRL3_REG = 0x22,
	CTRL4_REG = 0x23,
	CTRL5_REG =  0x24,
	REEFERENCE_REG =  0x25,
	OUT_TEMP_REG = 0x26,
	STATUS_REG  = 0x27,
	OUT_X_L_REG  = 0x28,
	OUT_X_H_REG  = 0x29,
	OUT_Y_L_REG = 0x2A,
	OUT_Y_H_REG = 0x2B,
	OUT_Z_L_REG = 0x2C,
	OUT_Z_H_REG  = 0x2D,
	FIFO_CTRL_REG  = 0x2E,
	FIFO_SRC_REG = 0x2F,
	IG_CFG_REG = 0x30,
	IG_SRC_REG = 0x31,
	IG_THS_XH_REG = 0x32,
	IG_THS_XL_REG = 0x33,
	IG_THS_YH_REG = 0x34,
	IG_THS_YL_REG = 0x35,
	IG_THS_ZH_REG = 0x36,
	IG_THS_ZL_REG = 0x37,
	IG_DURATION_REG = 0x38,
	LOW_ODR_REG = 0x39

}LG320DRegisters;

typedef enum{
	L3GD20_NORMAL_MODE = ((uint8_t)0x08),
	L3GD20_POWERDOWN_MODE = ((uint8_t)0x00)
}GyroPowerModeConfig;

typedef enum{
	DATARATE_00 = ((uint8_t)0x00),
	DATARATE_01 = ((uint8_t)0x40),
	DATARATE_10 = ((uint8_t)0x80),
	DATARATE_11 = ((uint8_t)0xC0)

}DataRate;

typedef enum{
	BW_00 = ((uint8_t)0x00),
	BW_01 = ((uint8_t)0x10),
	BW_10 = ((uint8_t)0x20),
	BW_11 = ((uint8_t)0x30)

}Bandwidth;

typedef enum{
	L3GD20_X_EN = ((uint8_t)0x02),
	L3GD20_Y_EN = ((uint8_t)0x01),
	L3GD20_Z_EN = ((uint8_t)0x04),
	L3GD20_AXES_EN = ((uint8_t)0x07),
	L3GD20_AXES_DISABLE = ((uint8_t)0x00),

}GyroAxesConfig;

typedef enum{
	L3GD20_BDU_CONTINUOUS = ((uint8_t)0x00),
	L3GD20_BDU_SINGLE = ((uint8_t)0x80),
	L3GD20_BLE_LSB = ((uint8_t)0x00),
	L3GD20_BLE_MSB = ((uint8_t)0x40),
	FS_245DPS = ((uint8_t)0x00),
	FS_500DPS = ((uint8_t)0x10),
	FS_2000DPS = ((uint8_t)0x20),
	FS_SELECT = ((uint8_t)0x30)

}GyrpReg4Config;


typedef enum{
	L3GD20_HPF_DISABLE = ((uint8_t) 0x00),
	L3GD20_HPF_ENABLE = ((uint8_t) 0x10)
}GyroHPFConfig;

typedef enum{
	INT_1 = ((uint8_t) 0x00),
	INT_2 = ((uint8_t) 0x01)
}GyroIntSelectConfig;

typedef enum{
	INT1_ENABLE = ((uint8_t) 0x80),
	INT1_DISABLE = ((uint8_t) 0x00),
	INT2_ENABLE = ((uint8_t) 0x08),
	INT2_DISABLE = ((uint8_t) 0x00)
}GyroIntEnableConfig;


typedef enum{
	INT_1_LOW_EDGE = ((uint8_t) 0x20),
	INT_1_HIGH_EDGE = ((uint8_t) 0x00)
}GyroIntActiveEdgeConfig;

typedef enum{
	BOOT_NORMAL_RES = ((uint8_t)0x00),
	REF_SIGNAL = ((uint8_t)0x10),
	NORMAL_MODE_HPF = ((uint8_t)0x20),
	AUTORESET_INT = ((uint8_t)0x30)
}GyroHPFModeConfig;

typedef enum{
	L3GD20_BOOT_NORMAL = ((uint8_t)0x00),
	L3GD20_BOOT_REBOOTMEM = ((uint8_t)0x80)
}GyroBootMode;

typedef enum{
	L3GD20_HPCF_0 = 0x0,
	L3GD20_HPCF_1 = 0x01,
	L3GD20_HPCF_2 = 0x02,
	L3GD20_HPCF_3 = 0x03,
	L3GD20_HPCF_4 = 0x04,
	L3GD20_HPCF_5 = 0x05,
	L3GD20_HPCF_6 = 0x06,
	L3GD20_HPCF_7 = 0x07,
	L3GD20_HPCF_8 = 0x08,
	L3GD20_HPCF_9 = 0x09
}GyroHPFCutouffHz;

void L3GD20Startup(void);
void L3GD20Init(void);
uint8_t  GyroReadSensorID (uint16_t DeviceAddr, uint8_t reg);
void GyroFilterConfig(uint8_t FilterStruct);
void L3GD20Write(uint16_t DeviceAddr, uint8_t reg, uint8_t value);
uint8_t L3GD20Read(uint16_t DeviceAddr, uint8_t reg);
void    L3GD20ReadXYZAngRate(float *pfData);
uint8_t L3GD20GetDataStatus(void);
float_t L3GD20GetTemp();




#endif
