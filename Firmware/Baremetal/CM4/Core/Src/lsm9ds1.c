#include "lsm9ds1.h"
#include <stm32h7xx_hal.h>


float gyro_res;
float mag_res;
float acc_res;

/*
 * @brief Used to read WHOAMI registers for the
 * sensors on the IC
 * @param address of the sensor
 * @param reg register to read
 * @returns value from the register
 */
uint8_t CheckIMUSensorID(uint8_t address, uint8_t reg)
{
    uint8_t reg_value = 0x00;
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Read(&hi2c1, address<<1, reg, I2C_MEMADD_SIZE_8BIT, &reg_value, 1,100);
    if(status != HAL_OK)
    {
       return HAL_ERROR;
    }
    return reg_value;
}

/*
 * @brief Initialises IMU
 * @param imu pointer to imu struct
 */
void init(LSM9DS1Handle* imu)
{
  // Setup Gyroscope
  imu->gyro_settings.enabled = 1;
  imu->gyro_settings.enableX = 1;
  imu->gyro_settings.enableY = 1;
  imu->gyro_settings.enableZ = 1;

  // Set GyroScope Scale 245, 500, 2000
  imu->gyro_settings.scale = 245;
  imu->gyro_settings.sampleRate = ODR_952HZ;

  // set cutoff frequency
  // set this all to zero
  // and do filtering in software
  imu->gyro_settings.bandwidth = 0;
  imu->gyro_settings.lowPowerEnable = 0;
  imu->gyro_settings.HPFEnable = 0;
  imu->gyro_settings.HPFCutoff = 0;
  imu->gyro_settings.flipX = 0;
  imu->gyro_settings.flipY = 0;
  imu->gyro_settings.flipZ = 0;
  imu->gyro_settings.orientation = 0;
  imu->gyro_settings.latchInterrupt = 1;

  //************Setup Accelerometer***************//
  imu->accel_settings.enabled = 1;
  imu->accel_settings.enableX = 1;
  imu->accel_settings.enableY = 1;
  imu->accel_settings.enableZ = 1;
  imu->accel_settings.scale = 2;
  imu->accel_settings.sampleRate = ODR_952HZ;
  imu->accel_settings.bandwidth = -1;  // bandwidth determined by sample rate
  imu->accel_settings.highResEnable = 0;
  imu->accel_settings.highResBandwidth = 0;

  //**********************Magnetometer******************//

  imu->mag_settings.enabled = 1;
  imu->mag_settings.scale = 4;
  imu->mag_settings.sampleRate = MAG_ODR_80HZ;
  imu->mag_settings.tempCompensationEnable = 0;
  imu->mag_settings.XYPerformance = MAG_UHP_MODE;  // ultra high performance
  imu->mag_settings.ZPerformance = MAG_UHP_MODE;
  imu->mag_settings.lowPowerEnable = 0;
  imu->mag_settings.operatingMode = 0 ; // continuous

  // enable temperature sensor
  imu->temp_settings.enabled = 1;
}

/*
 * @brief Calls initialisation functions
 * for various sensors
 * @param imu pointer to imu struct
 * @returns response from MAG and ACC/GYRO WHOAMI
 * register
 */
uint8_t setup(LSM9DS1Handle* imu)
{
  // initialise the sensors
  init(imu);

  // constrain the limits of each sensor
  clampScales(imu);

  // Calculate the resolution of each sensor
  calcGyroResolution(imu);
  calcAccResoltuion(imu);
  calcMagResolution(imu);

  // check that sensors are detected
  uint8_t ag_check =  CheckIMUSensorID(LSM9DS1_AG_ADDR, WHO_AM_I_XG);
  uint8_t mag_check = CheckIMUSensorID(LSM9DS1_M_ADDR, WHO_AM_I_M);

  uint16_t imu_response = (ag_check << 8) | mag_check;
  if(imu_response != ((WHO_AM_I_AG_RSP << 8 ) | WHO_AM_I_M_RSP))
  {
     return -1;
  }
  // initialise gyro
  initGyro(imu);
  // initialise accel
  initAccel(imu);
  // init Mag
  initMag(imu);
  return HAL_OK;;
}

/*
 * @brief Calls the setup IMU function
 * @param imu Pointer IMU struct
 * @retval none
 */
uint8_t ConfigIMU(LSM9DS1Handle* imu)
{
  if(setup(imu) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/*
 * @brief initialises gyroscope
 * @param imu ponter to imu struct
 * @returns nothing
 */
void initGyro(LSM9DS1Handle* imu)
{
  uint8_t tempRegValue = 0;
  // Only set sampleRate if gyro is enabled
  if(imu->gyro_settings.enabled == 1)
  {
    tempRegValue = (imu->gyro_settings.sampleRate & 0x07) << 5;
  }

  switch(imu->gyro_settings.scale)
  {
    case 500:
      tempRegValue |= (0x1 << 3);
      break;
    case 2000:
      tempRegValue |= (0x3 << 3);
      break;
      // defaults to 0x0 << 4 245 dps
  }
  tempRegValue |= (imu->gyro_settings.bandwidth & 0x3);
  // Write settings to the register
   WriteByte(LSM9DS1_AG_ADDR, CTRL_REG1_G, tempRegValue);
   // reset tempValue
   tempRegValue = 0;
   WriteByte(LSM9DS1_AG_ADDR, CTRL_REG2_G, tempRegValue);

   tempRegValue = 0;
   tempRegValue = imu->gyro_settings.lowPowerEnable ? (1<<7) : 0;
   if(imu->gyro_settings.HPFEnable == 0x01)
   {
     tempRegValue |= (1 << 6) | (imu->gyro_settings.HPFCutoff & 0x0F);
   }
   WriteByte(LSM9DS1_AG_ADDR, CTRL_REG3_G, tempRegValue);

   tempRegValue = 0;
   if(imu->gyro_settings.enableZ)
   {
     tempRegValue |= 1 << 5;
   }

   if(imu->gyro_settings.enableY)
   {
     tempRegValue |= 1 << 4;
   }

   if(imu->gyro_settings.enableX)
   {
     tempRegValue |= 1 << 3;
   }

   if(imu->gyro_settings.latchInterrupt)
   {
     tempRegValue |= 1 << 1;
   }
   WriteByte(LSM9DS1_AG_ADDR, CTRL_REG4, tempRegValue);

   tempRegValue = 0;
   if(imu->gyro_settings.flipX)
   {
      tempRegValue |= 1 << 5;
   }

   if(imu->gyro_settings.flipY)
   {
      tempRegValue |= 1 << 4;
   }

   if(imu->gyro_settings.flipZ)
   {
      tempRegValue |= 1 << 3;
   }

   WriteByte(LSM9DS1_AG_ADDR, ORIENT_CFG_G, tempRegValue);
}

/*
 * @brief initialises accelerometer
 * @param imu ponter to imu struct
 * @returns nothing
 */
void initAccel(LSM9DS1Handle* imu)
{
  uint8_t tempRegValue = 0;

  if(imu->accel_settings.enableZ)
  {
    tempRegValue |= 1 << 5;
  }

  if(imu->accel_settings.enableY)
  {
    tempRegValue |= 1 << 4;
  }

  if(imu->accel_settings.enableX)
  {
    tempRegValue |= 1 << 3;
  }

  // Write settings to the register
  WriteByte(LSM9DS1_AG_ADDR, CTRL_REG5_XL, tempRegValue);

  // reset tempValue
  tempRegValue = 0;
  // to disable accelerometer, set sampleRate bitfield to 0
  if(imu->accel_settings.enabled)
  {
    tempRegValue |= (imu->accel_settings.sampleRate & 0x07) << 5;
  }

  switch(imu->accel_settings.scale)
  {
    case 4:
      tempRegValue |= (0x2 << 3);
      break;
    case 8:
      tempRegValue |= (0x3 << 3);
      break;
    case 16:
      tempRegValue |= (0x1 << 3);
      break;
  }

  if(imu->accel_settings.bandwidth >= 0)
  {
     // Set BW Scale ODR
    tempRegValue |= (1 << 2);
    tempRegValue |= (imu->accel_settings.bandwidth & 0x03);
  }

  WriteByte(LSM9DS1_AG_ADDR, CTRL_REG6_XL, tempRegValue);
  // reset tempValue
  tempRegValue = 0;
  if(imu->accel_settings.highResEnable == 0x1)
  {
    tempRegValue |= (1 << 7);
    tempRegValue |= (imu->accel_settings.highResBandwidth & 0x3) << 5;
  }
  WriteByte(LSM9DS1_AG_ADDR, CTRL_REG7_XL, tempRegValue);
}


/*
 * @brief initialises magnetometer
 * @param imu ponter to imu struct
 * @returns nothing
 */
void initMag(LSM9DS1Handle* imu)
{
  uint8_t tempRegValue = 0;

  // check if temperature compensation is enabled
  if(imu->mag_settings.tempCompensationEnable == 0x01)
  {
    // Add 1 to the 7th bit of CTRL_REG_1_M
    tempRegValue |= 1 << 7;
  }
  // Make operating mode Ultra High Performance
  tempRegValue |= (imu->mag_settings.XYPerformance & 0x3) << 5;
  // Output Data rate set to 80Hz
  tempRegValue |= (imu->mag_settings.sampleRate & 0x7) << 2;
  // Write to Register
   WriteByte(LSM9DS1_M_ADDR, CTRL_REG1_M, tempRegValue);

   // reset regTempValue
   tempRegValue = 0;

   switch(imu->mag_settings.scale)
   {
     case 8:
      tempRegValue |= (0x1 << 5);
      break;
     case 12:
      tempRegValue |= (0x2 << 5);
      break;
     case 16:
      tempRegValue |= (0x3 << 5);
      break;
   }
   WriteByte(LSM9DS1_M_ADDR, CTRL_REG2_M, tempRegValue);

   tempRegValue = 0;
   if(imu->mag_settings.lowPowerEnable == 0x01)
   {
     // set LP bit in CTRL_REG3_M
     tempRegValue |= 0x1 << 5;
   }
   // Set Operating Mode to continuous conversion mode
   tempRegValue |= imu->mag_settings.operatingMode & 0x3;
   // Write value to register
   WriteByte(LSM9DS1_M_ADDR, CTRL_REG3_M, tempRegValue);

   // reset tempRegValue
   tempRegValue = 0;
   // set Z performance
   tempRegValue |= (imu->mag_settings.ZPerformance & 0x3) << 2;
   WriteByte(LSM9DS1_M_ADDR, CTRL_REG4_M, tempRegValue);

   // reset tempRegValue
   tempRegValue = 0;
   WriteByte(LSM9DS1_M_ADDR, CTRL_REG5_M, tempRegValue);

}

/*
 * @brief constrains scale of each
 * sensor to max values
 * @param pointer to imu struct
 * @returns nothing
 */
void clampScales(LSM9DS1Handle* imu)
{
  if((imu->gyro_settings.scale != 245) && (imu->gyro_settings.scale!= 500) && (imu->gyro_settings.scale!= 2000))
  {
     imu->gyro_settings.scale = 245;
  }

  if((imu->accel_settings.scale != 2) && (imu->accel_settings.scale!= 4) && (imu->accel_settings.scale!= 8)
      && (imu->accel_settings.scale != 16))
  {
     imu->accel_settings.scale = 245;
  }

  if((imu->mag_settings.scale != 4) && (imu->mag_settings.scale!= 8) && (imu->mag_settings.scale!= 12) &&
      (imu->mag_settings.scale != 16))
  {
     imu->mag_settings.scale = 4;
  }
}

/*
 * @brief calculates anhular rates in dps
 * @param gyro raw gyro value read from
 * register
 * @returns angular rate in dps
 */
float calcGyro(float gyro)
{
  return gyro * gyro_res;
}
/*
 * @brief calculates strength of magnetic field in gauss
 * @param mag raw mag value read from
 * register
 * @returns magnetic field strength in gauss
 */
float calcMag(float mag)
{
  return mag * mag_res;
}
/*
 * @brief calculates linear acceleration in gs
 * @param gyro raw acc value read from
 * register
 * @returns linear acceleration in gs
 */
float calcAccel(float accel)
{
  return accel * acc_res;
}

/*@brief returns the gyroscope
* data in dps
* @param imu struct handle
* @returns nothing
*/
void readGyro(LSM9DS1Handle* imu)
{
  int16_t gx, gy, gz;
  uint8_t buffer[6];

  I2CReadBytes(LSM9DS1_AG_ADDR, OUT_X_G_L, buffer, 6);

  gx = (buffer[1] << 8) | buffer[0]; // Store x-axis values into gx
  gy = (buffer[3] << 8) | buffer[2]; // Store y-axis values into gy
  gz = (buffer[5] << 8) | buffer[4]; // Store z-axis values into gz

  imu->gyro_values.x = calcGyro(gx);
  imu->gyro_values.y = calcGyro(gy);
  imu->gyro_values.z = calcGyro(gz);

}

///@brief returns the magnetometer
/// data in Gauss
///@param imu struct handle
///@returns nothing
void readMag(LSM9DS1Handle* imu)
{

  int16_t mx, my, mz;
  uint8_t buffer[6];

  I2CReadBytes(LSM9DS1_M_ADDR, OUT_X_M_L, buffer, 6);

  mx = (buffer[1] << 8) | buffer[0]; // Store x-axis values into mx
  my = (buffer[3] << 8) | buffer[2]; // Store y-axis values into my
  mz = (buffer[5] << 8) | buffer[4]; // Store z-axis values into mz

    imu->mag_values.x = calcMag(mx);
    imu->mag_values.y = calcMag(my);
    imu->mag_values.z = calcMag(mz);
}

///@brief returns the
/// accelerometer data in g's
///@param imu struct handle
///@returns nothing
void readAccel(LSM9DS1Handle* imu)
{
  int16_t ax, ay, az;
  uint8_t buffer[6];

  I2CReadBytes(LSM9DS1_AG_ADDR, OUT_X_XL_L, buffer, 6);

  ax = (buffer[1] << 8) | buffer[0]; // Store x-axis values into gx
  ay = (buffer[3] << 8) | buffer[2]; // Store y-axis values into gy
  az = (buffer[5] << 8) | buffer[4]; // Store z-axis values into gz

  imu->accel_values.x = calcAccel(ax);
  imu->accel_values.y = calcAccel(ay);
  imu->accel_values.z = calcAccel(az);

}

///@brief returns the
/// temprature data in celsius
///@param imu struct handle
///@returns nothing
void readTemp(LSM9DS1Handle* imu)
{
  float temp;
  uint8_t buffer[2];
  I2CReadBytes(LSM9DS1_AG_ADDR, OUT_TEMP_L, buffer, 2);
  // sensor reads 0 at 25 deg C so use as offset
  int16_t offset = 25;
  int16_t reg_values = (buffer[1] << 8) | buffer[0] >> 8;

  temp = ((float)reg_values/offset) + offset;
  //temp = offset + ((((int16_t)buffer[1] << 8) | buffer[0]) >> 8) ;
  imu->temperature = temp;
}

/*
 * @brief Sets Gyroscope scale
 * @param imu imu struct handle
 * @param gyro_scale gyroscope scale
 */

void setGyroScale(LSM9DS1Handle* imu,uint16_t gyro_scale)
{
  uint8_t temp = I2CReadByte(LSM9DS1_AG_ADDR, CTRL_REG1_G);
   // Mask out the scale bits
   temp &= 0xE7;
   switch(gyro_scale)
   {
     case 500:
       temp |= (0x1 << 3);
       imu->gyro_settings.scale = 500;
     break;

     case 2000:
       temp |= (0x3 << 3);
       imu->gyro_settings.scale = 2000;
     break;

     default:  // set default to 2
       imu->gyro_settings.scale = 245;
       break;
   }
   WriteByte(LSM9DS1_AG_ADDR, CTRL_REG1_G, temp);
   calcGyroResolution(imu);

}

/*
 * @brief Sets Magnetometer scale
 * @param imu imu struct handle
 * @param mag_scale magnetometer scale
 */
void setMagScale(LSM9DS1Handle* imu, uint8_t mag_scale)
{
  // read contents of the REG2_M register
  uint8_t temp = I2CReadByte(LSM9DS1_M_ADDR, CTRL_REG2_M);
  // Mask the Mag scale bit.
  temp &= 0xFF^(0x3 << 5);

    switch(mag_scale)
    {
      case 8:
        temp |= (0x1 << 5);
        imu->mag_settings.scale = 8;
        break;

      case 12:
        temp |= (0x2<<5);
        imu->mag_settings.scale = 12;
        break;

      case 16:
        temp |= (0x3 << 5);
        imu->mag_settings.scale = 12;
        break;
      default: // set to 4G
        imu->mag_settings.scale = 4;
        break;
    }
  // Write new value back into register.
  WriteByte(LSM9DS1_M_ADDR, CTRL_REG2_M, temp);
  // calculate new mag resolution
   calcMagResolution(imu);
}

/*
 * @brief Sets Accelerometer scale
 * @param imu imu struct handle
 * @param accel_scale accelerometer scale
 */
void setAccelScale(LSM9DS1Handle* imu, uint8_t accel_scale)
{
  // TODO Need to read the values of the initial register contents first so update
  // Register values to be local bits and not global hex values
  uint8_t temp = I2CReadByte(LSM9DS1_AG_ADDR, CTRL_REG6_XL);
  // Mask out the scale bits
  temp &= 0xE7;

  switch(accel_scale)
  {
    case 4:
      temp |= (0x2 << 3);
      imu->accel_settings.scale = 4;
    break;

    case 8:
      temp |= (0x3 << 3);
      imu->accel_settings.scale = 8;
    break;

    case 16:
      temp |= (0x1 << 3);
      imu->accel_settings.scale = 16;
    break;

    default:  // set default to 2
      imu->accel_settings.scale = 2;
      break;
  }
  WriteByte(LSM9DS1_AG_ADDR, CTRL_REG6_XL, temp);
  calcAccResoltuion(imu);
}

/*
 * @brief set Gyro data rate
 * @param imu imu struct handle
 * @param gyro_rate gyro data rate
 * @returns nothing
 */
void setGyroODR(LSM9DS1Handle* imu, uint8_t gyro_rate)
{
  // only do this if gyro_rate is not 0 (which would disable the gyro
  if((gyro_rate & 0x07) != 0)
  {
    // preserve the other bytes in CTRL_REG1_G so read first
    uint8_t temp = I2CReadByte(LSM9DS1_AG_ADDR, CTRL_REG1_G);
    // mask MAG ODR bits
    temp &= 0xFF^(0x7 << 5);
    // shift in the new ODR bits
    temp |= ((gyro_rate & 0x07) << 5);
    imu->accel_settings.sampleRate = gyro_rate & 0x07;

    // write new register valye back into CTRL_REG1_G
    WriteByte(LSM9DS1_AG_ADDR, CTRL_REG1_G, temp);
  }
}

/*
 * @brief set accel data rate
 * @param imu imu struct handle
 * @param gyro_rate accel data rate
 * @returns nothing
 */
void setAccelODR(LSM9DS1Handle* imu, uint8_t accel_Rate)
{
  // only do this if accel_rate is not 0 (which would disable the accel
  if((accel_Rate & 0x07) != 0)
  {

    // preserve the other bytes in CTRL_REG5_XM so read first
    uint8_t temp = I2CReadByte(LSM9DS1_AG_ADDR, CTRL_REG6_XL);
    // mask MAG ODR bits
    temp &= 0x1F;
    // shift in the new ODR bits
    temp |= ((accel_Rate & 0x07) << 2);
    imu->accel_settings.sampleRate = accel_Rate & 0x07;

    // write new register valye back into CTRL_REG5_XM
    WriteByte(LSM9DS1_AG_ADDR, CTRL_REG6_XL, temp);
  }

/*
 * @brief set Mag data rate
 * @param imu imu struct handle
 * @param mag_rate mag data rate
 * @returns nothing
 */
}
void setMagODR(LSM9DS1Handle* imu, uint8_t mag_rate)
{
  // preserve the other bytes in CTRL_REG5_XM so read first
  uint8_t temp = I2CReadByte(LSM9DS1_M_ADDR, CTRL_REG1_M);
  // mask MAG ODR bits
  temp &= 0xFF^(0x7 << 2);
  // shift in the new ODR bits
  temp |= ((mag_rate & 0x07) << 2);
  imu->mag_settings.sampleRate = mag_rate & 0x07;

  // write new register valye back into CTRL_REG5_XM
  WriteByte(LSM9DS1_M_ADDR, CTRL_REG1_M, temp);
}


/*
 * @brief puts gyroscope into sleep mode
 * @param enable enable flag
 * @returns nothing
 */
void sleepGyro(uint8_t enable)
{
  uint8_t temp = I2CReadByte(LSM9DS1_AG_ADDR, CTRL_REG9);
  if(enable == 1)
  {
    temp |= 1 << 6;
  }
  else
  {
    temp &= ~(1 << 6);
  }
  WriteByte(LSM9DS1_AG_ADDR, CTRL_REG9, temp);
}

/*
 * @brief Sets Gyroscope resolution
 * @param imu struct handle
 * @return nothing
 */
void calcGyroResolution(LSM9DS1Handle* imu)
{

  switch(imu->gyro_settings.scale)
  {
    case 245:
      gyro_res =  GYRO_FS_245;
      break;

    case 500:
      gyro_res = GYRO_FS_500;
      break;

    case 2000:
      gyro_res = GYRO_FS_2000;
      break;

    default:
      break;
  }
}

/*
 * @brief Sets Accelerometer resolution
 * @param imu struct handle
 * @return nothing
 */
void calcAccResoltuion(LSM9DS1Handle* imu)
{
  switch(imu->accel_settings.scale)
  {
    case 2:
      acc_res = ACC_FS_2G;
    break;

    case 4:
      acc_res = ACC_FS_4G;
     break;

    case 8:
      acc_res = ACC_FS_8G;
      break;

    case 16:
      acc_res = ACC_FS_16G;
      break;

    default:
      break;
  }
}

/*
 * @brief Sets Magnetometer resolution
 * @param imu struct handle
 * @return nothing
 */
void calcMagResolution(LSM9DS1Handle* imu)
{

  switch(imu->mag_settings.scale)
  {
    case 2:
      mag_res = MAG_FS_4G;
    break;

    case 4:
      mag_res = MAG_FS_8G;
     break;

    case 8:
      mag_res = MAG_FS_12G;
      break;

    case 16:
      mag_res = MAG_FS_16G;
      break;

    default:
      break;
  }
}


/*
 * @brief Reads a byte of data from a register
 * @param address address to read data from
 * @param reg register to read data from
 * @returns value read from register
 */
uint8_t I2CReadByte(uint8_t address, uint8_t reg)
{
   uint8_t value = 0x00;
   HAL_StatusTypeDef status;
   status = HAL_I2C_Mem_Read_DMA(&hi2c1, address<<1, reg, I2C_MEMADD_SIZE_8BIT, &value, 1);
   if(status != HAL_OK)
   {
      return HAL_ERROR;
   }
   return value;
}


/*
 * @brief Reads bytes of data from a register
 * @param address address to read data from
 * @param reg register to read data from
 * @param buffer stores data
 * @param number of bytes to read
 * @returns number of bytes read
 */
uint8_t I2CReadBytes(uint8_t address, uint8_t reg, uint8_t* buffer, uint8_t numBytes)
{
  HAL_StatusTypeDef status;
  status = HAL_I2C_Mem_Read(&hi2c1, address<<1, reg, I2C_MEMADD_SIZE_8BIT, buffer, numBytes,100);
  if(status !=HAL_OK)
  {
    return HAL_ERROR;
  }
  return numBytes;
}

/*
 * @brief Writes a byte of data to a register
 * @param address address to write data to
 * @param reg register to write data to
 * @param data data to write
 * @returns HAL_OK on success
 */
HAL_StatusTypeDef WriteByte(uint8_t address, uint8_t reg, uint8_t data)
{
  HAL_StatusTypeDef status;
  status = HAL_I2C_Mem_Write(&hi2c1, address<<1, (uint16_t) reg, I2C_MEMADD_SIZE_8BIT, &data, 1,100);
  if(status !=HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{


}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{


}


