// Adapted from https://github.com/JoeMerten/Stm32-Tools-Evaluation/tree/master/Stm32F4-SW-cubehal-source-freertos/Utilities/Components


#include "lsm303ldhc.h"
#include "common.h"

uint8_t I2CRead(uint16_t Address, uint8_t reg);
void I2CWrite(uint16_t Address, uint8_t reg, uint8_t value);
void ErrorHandler(void);


void LSM303StartUp(void)
{
	// Wait for sensor to initialise
	HAL_Delay(100);

	// check Sensor is present
	if(ReadSensorID(LSM303_ACC_ADDR, LSM303_REG_WHO_AM_I) == 0x33)
	{
		//LED1_ON();
	}

	else
	{
		ErrorHandler();
	}

}

void LSM303AccInit ()
{
	LSM303StartUp();
	uint16_t InitStruct = 0x00;
	uint8_t ctrl = 0x00;

	InitStruct |= (NORMAL_MODE | ODR_400HZ | X_EN | Y_EN | Z_EN) ;
	InitStruct |= ((BDU_CONTINUOUS | BLE_LSB | FS_2G | HR_EN) << 8);

	ctrl = (uint8_t)InitStruct;
    LSM303Write(LSM303_ACC_ADDR, CTRL_REG1_A, ctrl);
    ctrl = (uint8_t)(InitStruct << 8);
    LSM303Write(LSM303_ACC_ADDR, CTRL_REG4_A, ctrl);

    // Configure Filter structure for the accelerometer
    InitStruct = (uint8_t) (HPM_NORMAL | HPCF_16 | HPF_AO1_DISABLE | HPF_AO2_DISABLE);
    FilterConfig(InitStruct);


}

// configure filter for the Sensor
void FilterConfig(uint8_t FilterStruct)
{
	uint8_t reg_tmp;

	reg_tmp = LSM303Read(LSM303_ACC_ADDR, CTRL_REG2_A);

	reg_tmp &= 0x0C;
	reg_tmp |= FilterStruct;

	// Write value to the Register
	LSM303Write(LSM303_ACC_ADDR, CTRL_REG2_A, reg_tmp);
}

void LSM303MagInit()
{
	uint8_t ctrl = 0x00;
	uint32_t InitStruct = 0x00;

	InitStruct |= (TEMP_ENABLE | ODR_220HZ );
	InitStruct |= (LSM303_MAGGAIN_4_0) << 8;
	InitStruct |=  (CONTINUOUS_CONVERSION) << 16;

	ctrl = (uint8_t)InitStruct;
	LSM303Write(LSM303_MAG_ADDR, CRA_REG_M, ctrl);
	ctrl = (uint8_t) (InitStruct << 8);
	LSM303Write(LSM303_MAG_ADDR, CRB_REG_M, ctrl);
	ctrl = (uint8_t) (InitStruct << 16);
	LSM303Write(LSM303_MAG_ADDR, MR_REG_M, ctrl);

}

void LSM303Write(uint16_t DeviceAddr, uint8_t reg, uint8_t value)
{
	I2CWrite(DeviceAddr, reg, value);
}


uint8_t LSM303Read(uint16_t DeviceAddr, uint8_t reg)
{
	return I2CRead(DeviceAddr, reg);
}

void AccGetXYZ(int16_t* pData)
{
	int16_t raw_Data [3];
	uint8_t ctrlx[2] = {0,0};
	int8_t buffer [6];
	uint8_t i = 0;
	uint8_t sensitivity = SENS_2G;

	// read acceleration control content
	ctrlx[0] = LSM303Read(LSM303_ACC_ADDR, CTRL_REG4_A);
	ctrlx[1] = LSM303Read(LSM303_ACC_ADDR, CTRL_REG5_A);

	// Read Output Register
	buffer[0] = LSM303Read(LSM303_ACC_ADDR, OUT_X_L_A);
	buffer[1] = LSM303Read(LSM303_ACC_ADDR, OUT_X_H_A);
	buffer[2] = LSM303Read(LSM303_ACC_ADDR, OUT_Y_L_A);
	buffer[3] = LSM303Read(LSM303_ACC_ADDR, OUT_Y_H_A);
	buffer[4] = LSM303Read(LSM303_ACC_ADDR, OUT_Z_L_A);
	buffer[5] = LSM303Read(LSM303_ACC_ADDR, OUT_Z_H_A);

	// check REG4 alignment
	if(!(ctrlx[0] && BLE_MSB))
	{
		for (i = 0; i < 3; i++)
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

	// Switch sensitvity value in CTRL4
	switch(ctrlx[0] & FS_16G)
	{
		case FS_2G:
			sensitivity = SENS_2G;
		break;

		case FS_4G:
			sensitivity = SENS_4G;
		break;

		case FS_8G:
			sensitivity = SENS_8G;
		break;

		case FS_16G:
			sensitivity = SENS_16G;
		break;
	}

	// Obtain value in mg for each axis
	for(i = 0; i < 3; i++)
	{
		pData[i] = (raw_Data[i] * sensitivity);
	}

}



void LSM303ReadAcc(int16_t* pData)
{
	int16_t buffer[3] = {0};
	//LSM303AccData* sensor_acc = {0};

	AccGetXYZ(buffer);
	//data->x = buffer[0];
	//data->y = buffer[1];
	//data->z = buffer[2];
	pData[0] = buffer[0];
	pData[1] = buffer[1];
	pData[2] = buffer[2];

}

void LSM303ReadMag(int16_t* pData)
{
	int16_t buffer[3] = {0};

	static int16_t tmp16;

	MagGetXYZ(buffer);

	// correct for overflow
	tmp16 = buffer[0];
	if(tmp16 != -4096)
	{
		pData[0] = tmp16;
	}

	tmp16 = buffer[1];
	if(tmp16 != -4096)
	{
		pData[1] = tmp16;
	}

	tmp16 = buffer[2];
	if(tmp16 != -4096)
	{
		pData[2] = tmp16;
	}


}

float_t LSM303GetTemp()
{
	uint8_t TEMP_H;
	uint8_t TEMP_L;
	float_t temp;
	uint16_t pData;

	TEMP_H = LSM303Read(LSM303_MAG_ADDR, TEMP_OUT_H_M);
	TEMP_L = LSM303Read(LSM303_MAG_ADDR, TEMP_OUT_L_M);

    pData = (TEMP_H << 8);
    pData |= TEMP_L;

    // ALternatively
    // pData = (uint16_t)(TEMP_H << 4 | TEMP_L >> 4


	pData = ((uint16_t) pData >> 4);

	//2s complement
	if(pData > 0xF77 )
	{
		pData |= 0xF000;
	}
   // pData = ((uint16_t) ((TEMP_H << 8) | TEMP_L) >> 4);

	temp =  (pData / 256.0) + 20;
    // temp = pData / 256.0;

     return temp;
}


void MagGetXYZ(int16_t* pData)
{
	uint8_t buffer[6];
	uint8_t i = 0;

	buffer[0] = LSM303Read(LSM303_MAG_ADDR, OUT_X_H_M);
	buffer[1] = LSM303Read(LSM303_MAG_ADDR, OUT_X_L_M);
	buffer[2] = LSM303Read(LSM303_MAG_ADDR, OUT_Y_H_M);
	buffer[3] = LSM303Read(LSM303_MAG_ADDR, OUT_Y_L_M);
	buffer[4] = LSM303Read(LSM303_MAG_ADDR, OUT_Z_H_M);
	buffer[5] = LSM303Read(LSM303_MAG_ADDR, OUT_Z_L_M);

	for(i = 0; i < 3; i++)
	{
		if(pData[i] != -4096 )
		{
			pData[i] = ((uint16_t)((uint16_t)buffer[2*i] << 8) + buffer[2*i + 1]);
		}
	}
}


uint8_t I2CRead(uint16_t Address, uint8_t reg)
{
	HAL_StatusTypeDef status;
	uint8_t value = 0x00;

	status = HAL_I2C_Mem_Read(&hi2c1, Address, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 50);

	if(status !=HAL_OK)
	{
		ErrorHandler();
	}
//	else
//	{
//		LED1_ON();
//	}

	return value;
}

void I2CWrite(uint16_t Address, uint8_t reg, uint8_t value)
{
	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Write(&hi2c1, Address, (uint16_t) reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 50);
	if(status != HAL_OK)
	{
		ErrorHandler();
	}
	else
	{
		LED1_ON();
	}

}

void ErrorHandler(void)
{
	LED2_ON();
}

uint8_t ReadSensorID (uint16_t DeviceAddr, uint8_t reg)
{
	return I2CRead(DeviceAddr, reg);
}




