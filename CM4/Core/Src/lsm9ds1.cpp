/*
 * lsm9ds1.cpp
 *
 *  Created on: Aug 31, 2022
 *      Author: Daniel Babatunde
 */

#include "lsm9ds1.hpp"

IMU::IMU()
{
  bool is_imu = Setup();

}

gyroData IMU::GetGyroData()
{
  ReadGyro();

  return gyro_values;
}

accelData IMU::GetAccelData()
{
  ReadAccel();
  return accel_values;
}

magData IMU::GetMagData()
{
  ReadMag();

  return mag_values;
}

void IMU::Init()
{
  // Setup Gyroscope
  gyro_settings.enabled = true;
  gyro_settings.enableX = true;
  gyro_settings.enableY = true;
  gyro_settings.enableZ = true;

  // Set GyroScope Scale 245, 500, 2000
 gyro_settings.scale = to_underlying(settings::GyroFullScale::FS_245);
 gyro_settings.sampleRate = to_underlying(settings::GyroODR::ODR_952);

  // set cutoff frequency
  // set this all to zero
  // and do filtering in software
 gyro_settings.bandwidth = to_underlying(settings::GyroBandwidth::BW_00);
 gyro_settings.lowPowerEnable = false;
 gyro_settings.HPFEnable = false;
 gyro_settings.HPFCutoff = 0;
 gyro_settings.flipX = false;
 gyro_settings.flipY = false;
 gyro_settings.flipZ = false;
 gyro_settings.orientation = 0;
 gyro_settings.latchInterrupt = 0;

  //************Setup Accelerometer***************//
  accel_settings.enabled = true;
  accel_settings.enableX = true;
  accel_settings.enableY = true;
  accel_settings.enableZ = true;
  accel_settings.scale = to_underlying(settings::AccFullScale::XL_FS_4G);
  accel_settings.sampleRate = to_underlying(settings::AccODR::XL_ODR_952);
  accel_settings.bandwidth = -1;  // bandwidth determined by sample rate BW will be 408 Hz for 952 ODR
  accel_settings.highResEnable = 0;
  accel_settings.highResBandwidth = 0;

   //**********************Magnetometer******************//

  mag_settings.enabled = true;
  mag_settings.scale = to_underlying(settings::MagFullScale::MAG_4G);
  mag_settings.sampleRate = to_underlying(settings::MagODR::MAG_ODR_80HZ);
  mag_settings.tempCompensationEnable = 0;
  mag_settings.XYPerformance = to_underlying(settings::MagOpMode::MAG_UHP_MODE);  // ultra high performance
  mag_settings.ZPerformance = to_underlying(settings::MagZOpMode::MAG_UHP_MODE);
  mag_settings.lowPowerEnable = false;
  mag_settings.operatingMode = 0 ; // continuous

   // enable temperature sensor
  temp_settings.enabled = 1;

}

bool IMU::Setup()
{
  // initialise the sensors
    Init();

    // constrain the limits of each sensor
    ClampScales();

    // Calculate the resolution of each sensor
    CalcGyroResolution();
    CalcAccResolution();
    CalcMagResolution();

    // check that sensors are detected
     uint8_t ag_check =  CheckSensorID(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::WHO_AM_I_XG);
     uint8_t mag_check = CheckSensorID(magaddress::LSM9DS1_M_ADDR, magaddress::WHO_AM_I_M);


      uint16_t imu_response = (ag_check << 8) | mag_check;
        if(imu_response != ((imuresponse::WHO_AM_I_AG_RSP << 8 ) | imuresponse::WHO_AM_I_M_RSP))
        {
           return false;
        }
        // initialise gyro
        InitGyro();
        // initialise accel
        InitAccel();
        // init Mag
        InitMag();

        return true;

}

uint8_t IMU::CheckSensorID(const uint8_t &address, const uint8_t &reg)
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


void IMU::InitGyro()
{
   uint8_t temp_reg_val = 0;
   // CTRL_REG1_G (Default value: 0x00)
   // [ODR_G2][ODR_G1][ODR_G0][FS_G1][FS_G0][0][BW_G1][BW_G0]
   // ODR_G[2:0] - Output data rate selection
   // FS_G[1:0] - Gyroscope full-scale selection
   // BW_G[1:0] - Gyroscope bandwidth selection

   // Only set sampleRate if gyro is enabled
   if(gyro_settings.enabled == true)
   {
       temp_reg_val = gyro_settings.sampleRate;
   }

   // Set Scale
   temp_reg_val |= gyro_settings.scale;
   // Set Bandwidth
   temp_reg_val |= gyro_settings.bandwidth;
   // Write settings to the register
    WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG1_G, temp_reg_val);
    // reset tempValue
    temp_reg_val = 0;
    // CTRL_REG2_G (Default value: 0x00)
    // [0][0][0][0][INT_SEL1][INT_SEL0][OUT_SEL1][OUT_SEL0]
    // INT_SEL[1:0] - INT selection configuration
    // OUT_SEL[1:0] - Out selection configuration
    WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG2_G, temp_reg_val);
    temp_reg_val = 0;

    // CTRL_REG3_G (Default value: 0x00)
    // [LP_mode][HP_EN][0][0][HPCF3_G][HPCF2_G][HPCF1_G][HPCF0_G]
    // LP_mode - Low-power mode enable (0: disabled, 1: enabled)
    // HP_EN - HPF enable (0:disabled, 1: enabled)
    // HPCF_G[3:0] - HPF cutoff frequency
    temp_reg_val = gyro_settings.lowPowerEnable ? (1 << 7) : 0;
    if(gyro_settings.HPFEnable)
    {
        temp_reg_val |= (1 << 6) | (gyro_settings.HPFCutoff & 0x0F);
    }
    temp_reg_val = 0;
    // ORIENT_CFG_G (Default value: 0x00)
    // [0][0][SignX_G][SignY_G][SignZ_G][Orient_2][Orient_1][Orient_0]
    // SignX_G - Pitch axis (X) angular rate sign (0: positive, 1: negative)
    // Orient [2:0] - Directional user orientation selection
    if(gyro_settings.flipX)
    {
      temp_reg_val |= 1 << 5;
    }

    if(gyro_settings.flipY)
    {
      temp_reg_val |= 1 << 4;
    }

    if(gyro_settings.flipZ)
    {
      temp_reg_val |= 1 << 3;
    }
    WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::ORIENT_CFG_G, temp_reg_val);
    temp_reg_val = 0;

    // CTRL_REG4 (Default value: 0x38 | 00111000)
    // [0][0][Zen_G][Yen_G][Xen_G][0][LIR_XL1][4D_XL1]
    // Zen_G - Z-axis output enable (0:disable, 1:enable)
    // Yen_G - Y-axis output enable (0:disable, 1:enable)
    // Xen_G - X-axis output enable (0:disable, 1:enable)
    // LIR_XL1 - Latched interrupt (0:not latched, 1:latched)
    // 4D_XL1 - 4D option on interrupt (0:6D used, 1:4D used)

    if(gyro_settings.enableZ)
    {
      temp_reg_val |= 1 << 5;
    }

    if(gyro_settings.enableY)
    {
      temp_reg_val |= 1 << 4;
    }

    if(gyro_settings.enableX)
    {
      temp_reg_val |= 1 << 3;
    }

    if(gyro_settings.latchInterrupt)
    {
      temp_reg_val |= 1 << 1;
    }
    WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG4, temp_reg_val);
}

/*
 * @brief initialises accelerometer
 * @param none
 * @returns nothing
 */
void IMU::InitAccel()
{

  uint8_t temp_reg_val = 0;
  //    CTRL_REG5_XL (0x1F) (Default value: 0x38)
  //    [DEC_1][DEC_0][Zen_XL][Yen_XL][Zen_XL][0][0][0]
  //    DEC[0:1] - Decimation of accel data on OUT REG and FIFO.
  //        00: None, 01: 2 samples, 10: 4 samples 11: 8 samples
  //    Zen_XL - Z-axis output enabled
  //    Yen_XL - Y-axis output enabled
  //    Xen_XL - X-axis output enabled
  if(accel_settings.enableZ)
  {
    temp_reg_val |= 1 << 5;
  }

  if(accel_settings.enableY)
  {
    temp_reg_val |= 1 << 4;
  }

  if(accel_settings.enableX)
  {
    temp_reg_val |= 1 << 3;
  }

  // Write settings to the register
  WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG5_XL, temp_reg_val);

  // reset to zero
  temp_reg_val = 0;

  if(accel_settings.enabled == true)
  {
    temp_reg_val = accel_settings.sampleRate;
  }

  // Set Scale
  temp_reg_val |= accel_settings.scale;
  // Set Bandwidth
  if(accel_settings.bandwidth > 0)
  {
    temp_reg_val |= accel_settings.bandwidth;
  }

  WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG6_XL, temp_reg_val);

}

void IMU::InitMag()
{
  uint8_t temp_reg_val = 0;

  // CTRL_REG1_M (Default value: 0x10)
  // [TEMP_COMP][OM1][OM0][DO2][DO1][DO0][0][ST]
  // TEMP_COMP - Temperature compensation
  // OM[1:0] - X & Y axes op mode selection
  //    00:low-power, 01:medium performance
  //    10: high performance, 11:ultra-high performance
  // DO[2:0] - Output data rate selection
  // ST - Self-test enable

  // check if temperature compensation is enabled
  if(mag_settings.tempCompensationEnable)
  {
    // Add 1 to the 7th bit of CTRL_REG_1_M
    temp_reg_val |=  0x1 << 7;
  }

  // Make operating mode Ultra High Performance
  temp_reg_val |= mag_settings.XYPerformance;
  // Output Data rate set to 80Hz
  temp_reg_val |= mag_settings.sampleRate;
  // Write to Register
   WriteByte(magaddress::LSM9DS1_M_ADDR, magaddress::CTRL_REG1_M, temp_reg_val);

   // reset regTempValue
   temp_reg_val = 0;

   // CTRL_REG2_M (Default value 0x00)
   // [0][FS1][FS0][0][REBOOT][SOFT_RST][0][0]
   // FS[1:0] - Full-scale configuration
   // REBOOT - Reboot memory content (0:normal, 1:reboot)
   // SOFT_RST - Reset config and user registers (0:default, 1:reset)

   temp_reg_val |= mag_settings.scale;

   WriteByte(magaddress::LSM9DS1_M_ADDR, magaddress::CTRL_REG2_M, temp_reg_val);

   // reset regTempValue
   temp_reg_val = 0;

   // CTRL_REG3_M (Default value: 0x03)
   // [I2C_DISABLE][0][LP][0][0][SIM][MD1][MD0]
   // I2C_DISABLE - Disable I2C interace (0:enable, 1:disable)
   // LP - Low-power mode cofiguration (1:enable)
   // SIM - SPI mode selection (0:write-only, 1:read/write enable)
   // MD[1:0] - Operating mode
   //    00:continuous conversion, 01:single-conversion,
   //  10,11: Power-down
   if(mag_settings.lowPowerEnable)
   {
     // set LP bit in CTRL_REG3_M
     temp_reg_val |= 0x1 << 5;
   }

   // Set Operating Mode to continuous conversion mode
   temp_reg_val |= mag_settings.operatingMode & 0x3;

   // Write value to register
   WriteByte(magaddress::LSM9DS1_M_ADDR, magaddress::CTRL_REG3_M, temp_reg_val);

   // reset tempRegValue
   temp_reg_val = 0;
   // set Z performance
   temp_reg_val |= mag_settings.ZPerformance;
   WriteByte( magaddress::LSM9DS1_M_ADDR,  magaddress::CTRL_REG4_M, temp_reg_val);

   // reset tempRegValue
   temp_reg_val = 0;
   WriteByte( magaddress::LSM9DS1_M_ADDR,  magaddress::CTRL_REG5_M, temp_reg_val);

}


/*
 * @brief constrains scale of each
 * sensor to max values
 * @param pointer to imu struct
 * @returns nothing
 */
void IMU::ClampScales()
{
  if((gyro_settings.scale != to_underlying(settings::GyroFullScale::FS_245)) &&
     (gyro_settings.scale!= to_underlying(settings::GyroFullScale::FS_500)) &&
     (gyro_settings.scale!= to_underlying(settings::GyroFullScale::FS_2000)))
  {
     gyro_settings.scale = to_underlying(settings::GyroFullScale::FS_245);
  }

  if((accel_settings.scale != to_underlying(settings::AccFullScale::XL_FS_2G)) &&
     (accel_settings.scale!= to_underlying(settings::AccFullScale::XL_FS_4G)) &&
     (accel_settings.scale!= to_underlying(settings::AccFullScale::XL_FS_8G)) &&
     (accel_settings.scale != to_underlying(settings::AccFullScale::XL_FS_16G)))
  {
     accel_settings.scale = to_underlying(settings::AccFullScale::XL_FS_2G);
  }

  if((mag_settings.scale != to_underlying(settings::MagFullScale::MAG_4G)) &&
      (mag_settings.scale!= to_underlying(settings::MagFullScale::MAG_8G)) &&
      (mag_settings.scale!= to_underlying(settings::MagFullScale::MAG_12G)) &&
      (mag_settings.scale != to_underlying(settings::MagFullScale::MAG_16G)))
  {
     mag_settings.scale = to_underlying(settings::MagFullScale::MAG_4G);
  }
}


/*
* @brief calculates anhular rates in dps
* @param gyro raw gyro value read from
* register
* @returns angular rate in dps
*/
float IMU::CalcGyro(float gyro)
{
 return gyro * gyro_res;
}
/*
* @brief calculates strength of magnetic field in gauss
* @param mag raw mag value read from
* register
* @returns magnetic field strength in gauss
*/
float IMU::CalcMag(float mag)
{
 return mag * mag_res;
}
/*
* @brief calculates linear acceleration in gs
* @param gyro raw acc value read from
* register
* @returns linear acceleration in gs
*/
float IMU::CalcAccel(float accel)
{
 return accel * acc_res;
}


/*@brief returns the gyroscope
* data in dps
* @param imu struct handle
* @returns nothing
*/
void IMU::ReadGyro()
{
  int16_t gx, gy, gz;
  uint8_t buffer[6];

  I2CReadBytes(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::OUT_X_G_L, buffer, 6);

  gx = (buffer[1] << 8) | buffer[0]; // Store x-axis values into gx
  gy = (buffer[3] << 8) | buffer[2]; // Store y-axis values into gy
  gz = (buffer[5] << 8) | buffer[4]; // Store z-axis values into gz

  gyro_values.x = CalcGyro(gx);
  gyro_values.y = CalcGyro(gy);
  gyro_values.z = CalcGyro(gz);

}


///@brief returns the magnetometer
/// data in Gauss
///@param imu struct handle
///@returns nothing
void IMU::ReadMag()
{

  int16_t mx, my, mz;
  uint8_t buffer[6];

  I2CReadBytes(magaddress::LSM9DS1_M_ADDR, magaddress::OUT_X_M_L, buffer, 6);

  mx = (buffer[1] << 8) | buffer[0]; // Store x-axis values into mx
  my = (buffer[3] << 8) | buffer[2]; // Store y-axis values into my
  mz = (buffer[5] << 8) | buffer[4]; // Store z-axis values into mz

    mag_values.x = CalcMag(mx);
    mag_values.y = CalcMag(my);
    mag_values.z = CalcMag(mz);
}


///@brief returns the
/// accelerometer data in g's
///@param imu struct handle
///@returns nothing
void  IMU::ReadAccel()
{
  int16_t ax, ay, az;
  uint8_t buffer[6];

  I2CReadBytes(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::OUT_X_XL_L, buffer, 6);

  ax = (buffer[1] << 8) | buffer[0]; // Store x-axis values into ax
  ay = (buffer[3] << 8) | buffer[2]; // Store y-axis values into ay
  az = (buffer[5] << 8) | buffer[4]; // Store z-axis values into az

  accel_values.x = CalcAccel(ax);
  accel_values.y = CalcAccel(ay);
  accel_values.z = CalcAccel(az);

}


///@brief returns the
/// temprature data in celsius
///@param imu struct handle
///@returns nothing
void IMU::ReadTemp()
{
  float temp;
  uint8_t buffer[2];
  I2CReadBytes(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::OUT_TEMP_L, buffer, 2);
  // sensor reads 0 at 25 deg C so use as offset
  int16_t offset = 25;
  int16_t reg_values = (buffer[1] << 8) | buffer[0] >> 8;

  temp = ((float)reg_values/offset) + offset;
  //temp = offset + ((((int16_t)buffer[1] << 8) | buffer[0]) >> 8) ;
  temperature = temp;
}


/*
 * @brief Sets Gyroscope scale
 * @param gyro_scale gyroscope scale
 */

void IMU::SetGyroScale(uint16_t& gyro_scale)
{
  uint8_t temp = I2CReadByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG1_G);
   // Mask out the scale bits
   temp &= 0xE7;
   switch(gyro_scale)
   {
     case 500:
       temp |= (0x1 << 3);
       gyro_settings.scale = to_underlying(settings::GyroFullScale::FS_500);
     break;

     case 2000:
       temp |= (0x3 << 3);
       gyro_settings.scale = to_underlying(settings::GyroFullScale::FS_2000);
     break;

     default:  // set default to 2
       gyro_settings.scale = to_underlying(settings::GyroFullScale::FS_245);
       break;
   }
   WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG1_G, temp);
   CalcGyroResolution();

}

/*
 * @brief Sets Accelerometer scale
 * @param accel_scale accelerometer scale
 */
void IMU::SetAccelScale(uint8_t& accel_scale)
{
  // TODO Need to read the values of the initial register contents first so update
  // Register values to be local bits and not global hex values
  uint8_t temp = I2CReadByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG6_XL);
  // Mask out the scale bits
  temp &= 0xE7;

  switch(accel_scale)
  {
    case 4:
      temp |= (0x2 << 3);
      accel_settings.scale = to_underlying(settings::AccFullScale::XL_FS_4G);
    break;

    case 8:
      temp |= (0x3 << 3);
      accel_settings.scale = to_underlying(settings::AccFullScale::XL_FS_8G);
    break;

    case 16:
      temp |= (0x1 << 3);
      accel_settings.scale = to_underlying(settings::AccFullScale::XL_FS_16G);
    break;

    default:  // set default to 2
      accel_settings.scale = to_underlying(settings::AccFullScale::XL_FS_2G);
      break;
  }
  WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG6_XL, temp);
  CalcAccResolution();
}

/*
 * @brief set Gyro data rate
 * @param gyro_rate gyro data rate
 * @returns nothing
 */
void IMU::SetGyroODR(uint8_t gyro_rate)
{
  //TODO make sure gyro rate that is being set is a valid one
  // only do this if gyro_rate is not 0 (which would disable the gyro
  if((gyro_rate & 0x07) != 0)
  {
    // preserve the other bytes in CTRL_REG1_G so read first
    uint8_t temp = I2CReadByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG1_G);
    // mask MAG ODR bits
    temp &= 0xFF^(0x7 << 5);
    // shift in the new ODR bits
    temp |= ((gyro_rate & 0x07) << 5);
    gyro_settings.sampleRate = gyro_rate & 0x07;

    // write new register valye back into CTRL_REG1_G
    WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG1_G, temp);
  }
}

/*
 * @brief set accel data rate
 * @param gyro_rate accel data rate
 * @returns nothing
 */
void IMU::SetAccelODR(uint8_t accel_rate)
{
  //TODO check accel_rate is a valid value
  // only do this if accel_rate is not 0 (which would disable the accel
  if((accel_rate & 0x07) != 0)
  {

    // preserve the other bytes in CTRL_REG5_XM so read first
    uint8_t temp = I2CReadByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG6_XL);
    // mask MAG ODR bits
    temp &= 0x1F;
    // shift in the new ODR bits
    temp |= ((accel_rate & 0x07) << 2);
    accel_settings.sampleRate = accel_rate & 0x07;

    // write new register valye back into CTRL_REG5_XM
    WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG6_XL, temp);
  }

 }
/*
 * @brief set Mag data rate
 * @param mag_rate mag data rate
 * @returns nothing
 */
  void IMU::SetMagODR(uint8_t mag_rate)
  {
    //TODO check mag rate is a valid value
    // preserve the other bytes in CTRL_REG5_XM so read first
    uint8_t temp = I2CReadByte(magaddress::LSM9DS1_M_ADDR, magaddress::CTRL_REG1_M);
    // mask MAG ODR bits
    temp &= 0xFF^(0x7 << 2);
    // shift in the new ODR bits
    temp |= ((mag_rate & 0x07) << 2);
    mag_settings.sampleRate = mag_rate & 0x07;

    // write new register valye back into CTRL_REG5_XM
    WriteByte(magaddress::LSM9DS1_M_ADDR, magaddress::CTRL_REG1_M, temp);
  }



/*
 * @brief puts gyroscope into sleep mode
 * @param enable enable flag
 * @returns nothing
 */
void IMU::SleepGyro(uint8_t enable)
{
  uint8_t temp = I2CReadByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG9);
  if(enable == 1)
  {
    temp |= 1 << 6;
  }
  else
  {
    temp &= ~(1 << 6);
  }
  WriteByte(gyroaccaddress::LSM9DS1_AG_ADDR, gyroaccaddress::CTRL_REG9, temp);
}

/*
 * @brief Sets Gyroscope resolution
 * @return nothing
 */
void IMU::CalcGyroResolution()
{
  settings::GyroFullScale G;

  G = static_cast<settings::GyroFullScale>(gyro_settings.scale);

  switch(G)
  {
    case settings::GyroFullScale::FS_245:
      gyro_res =  resolution::GYRO_FS_245;
      break;

    case settings::GyroFullScale::FS_500:
      gyro_res = resolution::GYRO_FS_500;
      break;

    case settings::GyroFullScale::FS_2000:
      gyro_res =  resolution::GYRO_FS_2000;
      break;

    default:
      break;
  }
}


/*
 * @brief Sets Accelerometer resolution
 * @return nothing
 */
void IMU::CalcAccResolution()
{
  settings::AccFullScale A;
  A = static_cast<settings::AccFullScale>(accel_settings.scale);

  switch(A)
  {
    case settings::AccFullScale::XL_FS_2G:
      acc_res = resolution::ACC_FS_2G;
    break;

    case settings::AccFullScale::XL_FS_4G:
      acc_res =  resolution::ACC_FS_4G;
     break;

    case settings::AccFullScale::XL_FS_8G:
      acc_res =  resolution::ACC_FS_8G;
      break;

    case settings::AccFullScale::XL_FS_16G:
      acc_res =  resolution::ACC_FS_16G;
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
void IMU::CalcMagResolution()
{

  settings::MagFullScale M;

  M = static_cast<settings::MagFullScale>(mag_settings.scale);

  switch(M)
  {
    case settings::MagFullScale::MAG_4G:
      mag_res = resolution::MAG_FS_4G;
    break;

    case settings::MagFullScale::MAG_8G:
      mag_res = resolution::MAG_FS_8G;
     break;

    case settings::MagFullScale::MAG_12G:
      mag_res = resolution::MAG_FS_12G;
      break;

    case settings::MagFullScale::MAG_16G:
      mag_res = resolution::MAG_FS_16G;
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
uint8_t IMU::I2CReadByte(const uint8_t &address, const uint8_t &reg)
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
uint8_t IMU::I2CReadBytes(const uint8_t &address, const uint8_t &reg,  uint8_t* buffer, uint8_t numBytes)
{
  HAL_StatusTypeDef status;
  status = HAL_I2C_Mem_Read_DMA(&hi2c1, address<<1, reg, I2C_MEMADD_SIZE_8BIT, buffer, numBytes);
  HAL_Delay(1);
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
HAL_StatusTypeDef IMU::WriteByte(const uint8_t &address, const uint8_t &reg, uint8_t data)
{
  HAL_StatusTypeDef status;
//  status = HAL_I2C_Mem_Write(&hi2c1, address<<1, (uint16_t) reg, I2C_MEMADD_SIZE_8BIT, &data, 1,100);
  status = HAL_I2C_Mem_Write_DMA(&hi2c1, address<<1, (uint16_t) reg, I2C_MEMADD_SIZE_8BIT, &data, 1);
  HAL_Delay(1);
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



