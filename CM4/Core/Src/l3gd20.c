#include "l3gd20.h"
#include "common.h"


void L3GD20Startup(void)
{
	uint8_t temp = 0x00;
	temp = GyroReadSensorID(L3GD20_ADDRESS, WHO_AM_I);

//	if(GyroReadSensorID(L3GD20_ADDRESS, WHO_AM_I) == I_AM_L3GD20 || I_AM_L3GD20_TR)
//	{
//		LED1_ON();
//	}

	if(temp == I_AM_L3GD20 || I_AM_L3GD20H )
	{
		LED1_ON();
	}

	else
	{
		ErrorHandler();
	}

}


void L3GD20Init(void)
{
	L3GD20Startup();
	uint16_t InitStruct = 0x00;
	uint8_t ctrl = 0x00;

	// 200Hz ODR.
	// +/- 250 DPS
	InitStruct |= (L3GD20_NORMAL_MODE | DATARATE_01 | L3GD20_AXES_EN | BW_00);
	InitStruct |= ((L3GD20_BDU_CONTINUOUS | L3GD20_BLE_LSB | FS_500DPS) << 8);

	ctrl = (uint8_t)InitStruct;
	L3GD20Write(L3GD20_ADDRESS, CTRL1_REG, ctrl);

	ctrl = (uint8_t) (InitStruct << 8);
	L3GD20Write(L3GD20_ADDRESS, CTRL4_REG, ctrl);

	InitStruct = (uint8_t) (BOOT_NORMAL_RES | L3GD20_HPCF_0);
	GyroFilterConfig(InitStruct);

}

void GyroFilterConfig(uint8_t FilterStruct)
{
	uint8_t tmpreg;

	L3GD20Read(L3GD20_ADDRESS, CTRL2_REG);

	tmpreg &=0xC0;
	tmpreg |= FilterStruct;

	L3GD20Write(L3GD20_ADDRESS, CTRL2_REG, tmpreg);

}

void L3GD20Write(uint16_t DeviceAddr, uint8_t reg, uint8_t value)
{
	I2CWrite(DeviceAddr, reg, value);
}

uint8_t L3GD20Read(uint16_t DeviceAddr, uint8_t reg)
{
	return I2CRead(DeviceAddr, reg);
}

void  L3GD20ReadXYZAngRate(float *pfData)
{
	int16_t raw_Data [3];
	uint8_t ctrlx = 0;
	uint8_t buffer[6] ;
	uint8_t i = 0;
	float sensitivity = 0.0f;

	ctrlx = L3GD20Read(L3GD20_ADDRESS, CTRL4_REG);

	buffer[0] = LSM303Read(L3GD20_ADDRESS, OUT_X_L_REG);
	buffer[1] = LSM303Read(L3GD20_ADDRESS, OUT_X_H_REG);
	buffer[2] = LSM303Read(L3GD20_ADDRESS, OUT_Y_L_REG);
	buffer[3] = LSM303Read(L3GD20_ADDRESS, OUT_Y_H_REG);
	buffer[4] = LSM303Read(L3GD20_ADDRESS, OUT_Z_L_REG);
	buffer[5] = LSM303Read(L3GD20_ADDRESS, OUT_Z_H_REG);

	if(!(ctrlx & L3GD20_BLE_MSB))
	{
		for(i = 0; i<3; i++)
		{
			raw_Data[i] = ((int16_t) ((uint16_t)buffer[2*i+1] << 8) + buffer[2*i] );
		}
	}

	else // Big Endian Mode
	{
		for (i = 0; i < 3; i++)
		{
			raw_Data[i] = ((int16_t) ((uint16_t)buffer[2*i] << 8) + buffer[2*i+1] );
		}
	}

	switch(ctrlx & FS_SELECT)
	{
	case FS_245DPS:
		sensitivity = GYRO_SENSITIVITY_250DPS;
		break;
	case FS_500DPS:
		sensitivity = GYRO_SENSITIVITY_500DPS;
		break;
	case FS_2000DPS:
		sensitivity = GYRO_SENSITIVITY_2000DPS;
		break;
	}

	for(i = 0; i < 3; i++)
	{
		pfData[i] = (float)(raw_Data[i] * sensitivity);
	}


}


float_t L3GD20GetTemp()
{

	uint8_t val;
	float_t temp;

	val = L3GD20Read(L3GD20_ADDRESS, OUT_TEMP_REG);

	if(val > 0xF77)
	{
		val |= 0xF000;
	}

	temp = (val/256.0) + 20;


	return temp;

}


uint8_t GyroReadSensorID (uint16_t DeviceAddr, uint8_t reg)
{
	return I2CRead(DeviceAddr, reg);
}
