#include "i2c.h"

// ************************Gyroscope Register Settings********************************//

// CTRL_REG1_G (Default value: 0x00)
// [ODR_G2][ODR_G1][ODR_G0][FS_G1][FS_G0][0][BW_G1][BW_G0]
// ODR_G[2:0] - Output data rate selection
// FS_G[1:0] - Gyroscope full-scale selection
// BW_G[1:0] - Gyroscope bandwidth selection

// CTRL_REG2_G (Default value: 0x00)
// [0][0][0][0][INT_SEL1][INT_SEL0][OUT_SEL1][OUT_SEL0]
// INT_SEL[1:0] - INT selection configuration
// OUT_SEL[1:0] - Out selection configuration

// CTRL_REG3_G (Default value: 0x00)
// [LP_mode][HP_EN][0][0][HPCF3_G][HPCF2_G][HPCF1_G][HPCF0_G]
// LP_mode - Low-power mode enable (0: disabled, 1: enabled)
// HP_EN - HPF enable (0:disabled, 1: enabled)
// HPCF_G[3:0] - HPF cutoff frequency


// CTRL_REG4 (Default value: 0x38)
// [0][0][Zen_G][Yen_G][Xen_G][0][LIR_XL1][4D_XL1]
// Zen_G - Z-axis output enable (0:disable, 1:enable)
// Yen_G - Y-axis output enable (0:disable, 1:enable)
// Xen_G - X-axis output enable (0:disable, 1:enable)
// LIR_XL1 - Latched interrupt (0:not latched, 1:latched)
// 4D_XL1 - 4D option on interrupt (0:6D used, 1:4D used)


// ORIENT_CFG_G (Default value: 0x00)
// [0][0][SignX_G][SignY_G][SignZ_G][Orient_2][Orient_1][Orient_0]
// SignX_G - Pitch axis (X) angular rate sign (0: positive, 1: negative)
// Orient [2:0] - Directional user orientation selection

//************************Accelerometer Regsiter Settings******************************//
//  CTRL_REG5_XL (0x1F) (Default value: 0x38)
//  [DEC_1][DEC_0][Zen_XL][Yen_XL][Zen_XL][0][0][0]
//  DEC[0:1] - Decimation of accel data on OUT REG and FIFO.
//    00: None, 01: 2 samples, 10: 4 samples 11: 8 samples
//  Zen_XL - Z-axis output enabled
//  Yen_XL - Y-axis output enabled
//  Xen_XL - X-axis output enabled

// CTRL_REG6_XL (0x20) (Default value: 0x00)
// [ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
// ODR_XL[2:0] - Output data rate & power mode selection
// FS_XL[1:0] - Full-scale selection
// BW_SCAL_ODR - Bandwidth selection
// BW_XL[1:0] - Anti-aliasing filter bandwidth selection


// CTRL_REG7_XL (0x21) (Default value: 0x00)
// [HR][DCF1][DCF0][0][0][FDS][0][HPIS1]
// HR - High resolution mode (0: disable, 1: enable)
// DCF[1:0] - Digital filter cutoff frequency
// FDS - Filtered data selection
// HPIS1 - HPF enabled for interrupt function



//*********************Magnetometer Register Settings******************************//
// CTRL_REG1_M
// [TEMP_COMP][OM1][OM0][DO2][DO1][DO0][0][ST]
// TEMP_COMP - Temperature compensation
// OM[1:0] - X & Y axes op mode selection
//    00:low-power,
//    01:medium performance
//    10: high performance,
//    11:ultra-high performance
// DO[2:0] - Output data rate selection
// ST - Self-test enable


// CTRL_REG2_M (Default value 0x00)
// [0][FS1][FS0][0][REBOOT][SOFT_RST][0][0]
// FS[1:0] - Full-scale configuration
// REBOOT - Reboot memory content (0:normal, 1:reboot)
// SOFT_RST - Reset config and user registers (0:default, 1:reset)


// CTRL_REG3_M (Default value: 0x03)
// [I2C_DISABLE][0][LP][0][0][SIM][MD1][MD0]
// I2C_DISABLE - Disable I2C interace (0:enable, 1:disable)
// LP - Low-power mode cofiguration (1:enable)
// SIM - SPI mode selection (0:write-only, 1:read/write enable)
// MD[1:0] - Operating mode
//  00:continuous conversion, 01:single-conversion,
//  10,11: Power-down


// CTRL_REG4_M (Default value: 0x00)
// [0][0][0][0][OMZ1][OMZ0][BLE][0]
// OMZ[1:0] - Z-axis operative mode selection
//  00:low-power mode, 01:medium performance
//  10:high performance, 10:ultra-high performance
// BLE - Big/little endian data

// CTRL_REG5_M (Default value: 0x00)
// [0][BDU][0][0][0][0][0][0]
// BDU - Block data update for magnetic data
//  0:continuous, 1:not updated until MSB/LSB are read



#define LSM9DS1_AG_ADDR 0x6B
#define LSM9DS1_M_ADDR 0x1E
#define REG_ACT_THS           0x04
#define REG_ACT_DUR           0x05
#define REG_INT_GEN_CFG_XL    0x06
#define REG_INT_GEN_THS_X_XL  0x07
#define INT_GEN_THS_Y_XL      0x08
#define INT_GEN_THS_Z_XL      0x09
#define INT_GEN_DUR_XL        0x0A
#define INT1_CTRL             0x0C
#define INT2_CTRL             0x0D
#define WHO_AM_I_XG           0x0F
#define CTRL_REG1_G           0x10
#define CTRL_REG2_G           0x11
#define CTRL_REG3_G           0x12
#define ORIENT_CFG_G          0x13
#define INT_GEN_SRC_G         0x14
#define OUT_TEMP_L            0x15
#define OUT_TEMP_H            0x16
#define STATUS_REG_0          0x17
#define OUT_X_G_L             0x18
#define OUT_X_G_H             0x19
#define OUT_Y_G_L             0x1A
#define OUT_Y_G_H             0x1B
#define OUT_Z_G_L             0x1C
#define OUT_Z_G_H             0x1D
#define CTRL_REG4             0x1E
#define CTRL_REG5_XL          0x1F
#define CTRL_REG6_XL          0x20
#define CTRL_REG7_XL          0x21
#define CTRL_REG8             0x22
#define CTRL_REG9             0x23
#define CTRL_REG10            0x24
#define INT_GEN_SRC_XL        0x26
#define STATUS_REG_1          0x27
#define OUT_X_XL_L            0x28
#define OUT_X_XL_H            0x29
#define OUT_Y_XL_L            0x2A
#define OUT_Y_XL_H            0x2B
#define OUT_Z_XL_L            0x2C
#define OUT_Z_XL_H            0x2D
#define FIFO_CTRL             0x2E
#define FIFO_SRC              0x2F
#define INT_GEN_CFG_G         0x30
#define INT_GEN_THS_XH_G      0x31
#define INT_GEN_THS_XL_G      0x32
#define INT_GEN_THS_YH_G      0x33
#define INT_GEN_THS_YL_G      0x34
#define INT_GEN_THS_ZH_G      0x35
#define INT_GEN_THS_ZL_G      0x36
#define INT_GEN_DUR_G         0x37

//////////////////////////////////
// Magenetometer Registers     //
/////////////////////////////////

#define OFFSET_X_REG_M_L      0x05
#define OFFSET_X_REG_M_H      0x06
#define OFFSET_Y_REG_M_L      0x07
#define OFFSET_Y_REG_M_H      0x08
#define OFFSET_Z_REG_M_L      0x09
#define OFFSET_Z_REG_M_H      0x0A
#define WHO_AM_I_M            0x0F
#define CTRL_REG1_M           0x20
#define CTRL_REG2_M           0x21
#define CTRL_REG3_M           0x22
#define CTRL_REG4_M           0x23
#define CTRL_REG5_M           0x24
#define STATUS_REG_M          0x27
#define OUT_X_M_L             0x28
#define OUT_X_M_H             0x29
#define OUT_Y_M_L             0x2A
#define OUT_Y_M_H             0x2B
#define OUT_Z_M_L             0x2C
#define OUT_Z_M_H             0x2D
#define INT_CFG_M             0x30
#define INT_SRC_M             0x31
#define INT_THS_L_M           0x32
#define INT_THS_H_M           0x33

////////////////////////////////
// LSM9DS1 WHO_AM_I Responses //
////////////////////////////////
#define WHO_AM_I_AG_RSP   0x68
#define WHO_AM_I_M_RSP    0x3D


#define  ACC_FS_2G    0.000061
#define  ACC_FS_4G    0.000122
#define  ACC_FS_8G    0.000244
#define  ACC_FS_16G   0.000732
#define  MAG_FS_4G    0.00014
#define MAG_FS_8G     0.00029
#define MAG_FS_12G    0.00043
#define MAG_FS_16G    0.00058
#define GYRO_FS_245   0.00875
#define GYRO_FS_500   0.0175
#define GYRO_FS_2000  0.07


typedef enum{
  MAG_LP_MODE = 0x00,
  MAG_MP_MODE = 0x01,
  MAG_HP_MODE = 0x02,
  MAG_UHP_MODE = 0x03
}Mag_OpMode;

typedef enum{
  MAG_ODR_0_625 = 0x00,
  MAG_ODR_1_25 =  0x04,
  MAG_ODR_2_5 =   0x08,
  MAG_ODR_5 =     0x0C,
  MAG_ODR_10 =    0x10,
  MAG_ODR_20 =    0x14,
  MAG_ODR_40 =    0x18,
  MAG_ODR_80 =    0x1C

}Mag_ODR;

typedef enum{
  MAG_4G = 0x00,
  MAG_8G = 0x20,
  MAG_12G = 0x40,
  MAG_16G = 0x60
}Mag_FullScale;

typedef enum{
  FIFO_OFF = 0x00,
  FIFO_MODE = 0x20,
  FIFO_CONT_TRG = 0x60,
  FIFO_BYP_TRG = 0x80,
  FIFO_CONT = 0xC0
}FIFO_Mode;

// in g/LSB





typedef enum{
  ODR_PD = 0x00,
  ODR_14_9 = 0x20,
  ODR_59_5 = 0x40,
  ODR_119 =  0x70,
  ODR_238 =  0x80,
  ODR_476 =  0xA0,
  ODR_952 =  0xC0

}Gyro_ODR;

typedef enum{
  XL_ODR_PD = 0x00,
  XL_ODR_10 = 0x20,
  XL_ODR_50 = 0x40,
  XL_ODR_119 = 0x60,
  XL_ODR_238 = 0x80,
  XL_ODR_476 = 0xA0,
  XL_ODR_952 = 0xC0
}Acc_ODR;

typedef enum{

  XL_FS_2G = 0x00,
  XL_FS_4G = 0x10,
  XL_FS_8G =  0x18,
  XL_FS_16G = 0x08

}Acc_FullScale;

typedef enum{
  XL_BW_50 = 0x03,
  XL_BW_105 = 0x02,
  XL_BW_211 = 0x01,
  XL_BW_408 = 0x00
}Acc_BandWidth;

typedef enum{
  FS_245 = 0x00,
  FS_500 = 0x10,
  FS_2000 = 0x18
}Gyro_FullScale;

typedef enum{
  BW_00 = 0x00,
  BW_10 = 0x02,
  BW_01 = 0x01,
  BW_11 = 0x03
}Gyro_BW_G;



typedef enum{
  ODR_POWERDOWN = ((uint8_t)0x00),
  ODR_14_9HZ =  ((uint8_t)0x01),
  ODR_59_5HZ =  ((uint8_t)0x02),
  ODR_119HZ =  ((uint8_t)0x03),
  ODR_238HZ =  ((uint8_t)0x04),
  ODR_476HZ =  ((uint8_t)0x05),
  ODR_952HZ =  ((uint8_t)0x06)

}Gyro_Mode;


typedef enum{
  MAG_ODR_0_625HZ = 0x00,
  MAG_ODR_1_25HZ =  0x01,
  MAG_ODR_2_5HZ =   0x02,
  MAG_ODR_5HZ =     0x03,
  MAG_ODR_10HZ =    0x04,
  MAG_ODR_20HZ =    0x05,
  MAG_ODR_40HZ =    0x06,
  MAG_ODR_80HZ =    0x07

}Mag_Mode;

typedef enum{
  XLIE_XL = ((uint8_t)0x01),
  XHIE_XL = ((uint8_t)0x02),
  YLIE_XL = ((uint8_t)0x04),
  YHIE_XL = ((uint8_t)0x08),
  ZLIE_XL = ((uint8_t)0x10),
  ZHIE_XL = ((uint8_t)0x20),
  GEN_6D = ((uint8_t)0x40),
  AOI_XL = ((uint8_t)0x80),

}Acc_Int_Generator;


typedef enum{
  INT_DRDY_XL = ((uint8_t) 0x01), // Accelerometer data ready INT1 and INT2
  INT_DRDY_G = ((uint8_t) 0x02), // Gyro data ready INT1 and INT2
  INT1_BOOT = ((uint8_t) 0x04), // INt 1 Boot
  INT2_DRDY_TEMP = ((uint8_t) 0x04), // INt 2 temp data ready
  INT_FTH = ((uint8_t) 0x08),      // FIFO threshold interrupt (INT1 & INT2)
  INT_OVR = ((uint8_t)0x10),      // Overrun interrupt (INT1 & INT2)
  INT_FSS5 = ((uint8_t)0x20),     // FSS5 interrupt (INT1 & INT2)
  INT_IG_XL = ((uint8_t)0x40),   // Accel interrupt generator (INT1)
  INT1_IG_G = ((uint8_t)0x80),   // Gyro interrupt enable (INT1)
  INT2_INACT = ((uint8_t)0x80),   // Inactivity interrupt output (INT2)

}INT_Reg_Generator;



typedef struct gyroSettings
{

  uint8_t enabled;
  uint16_t scale;
  uint8_t sampleRate;
  uint8_t bandwidth;
  uint8_t lowPowerEnable;
  uint8_t HPFEnable;
  uint8_t HPFCutoff;
  uint8_t flipX;
  uint8_t flipY;
  uint8_t flipZ;
  uint8_t orientation;
  uint8_t enableX;
  uint8_t enableY;
  uint8_t enableZ;
  uint8_t latchInterrupt;
}gyroSettings;

typedef struct magSettings
{
  uint8_t enabled;
  uint8_t scale;
  uint8_t sampleRate;
  uint8_t tempCompensationEnable;
  uint8_t XYPerformance;
  uint8_t ZPerformance;
  uint8_t lowPowerEnable;
  uint8_t operatingMode;

}magSettings;


typedef struct tempSettings
{
  uint8_t enabled;

}tempSettings;

typedef struct accelSettings
{
  uint8_t enabled;
  uint8_t scale;
  uint8_t sampleRate;
  uint8_t enableX; //
  uint8_t enableY; //
  uint8_t enableZ; //
  int8_t bandwidth;
  uint8_t highResEnable;
  uint8_t highResBandwidth;

}accelSettings;

typedef struct gyroData
{

  float x;
  float y;
  float z;

}gyroData;

typedef struct accelData
{

  float x;
  float y;
  float z;

}accelData;

typedef struct magData
{
  float x;
  float y;
  float z;
}magData;

typedef struct{

  struct gyroSettings gyro_settings;
  struct accelSettings accel_settings;
  struct magSettings mag_settings;
  struct tempSettings temp_settings;

  struct magData mag_values;
  struct accelData accel_values;
  struct gyroData gyro_values;

}LSM9DS1Handle;




// Initialises the IMU using the address of the accelerometer
// gyro and magnetometer
uint16_t setup(LSM9DS1Handle* imu);


// sets up the sensor and default settings
void init(LSM9DS1Handle* imu);
void initGyro(LSM9DS1Handle* imu);
void initAccel(LSM9DS1Handle* imu);
void initMag(LSM9DS1Handle* imu);

// calibrates the sensors.
//void calibrate(uint8_t autoCalc);
//void calibrateMag(uint8_t loadIn);
//void magOffset(uint8_t axis, int16_t offset);


// poll sensors to check if data is available
uint8_t accelAvailable();
uint8_t gyroAvailable();
uint8_t tempAvailable();
uint8_t magAvailable();

void readGyro(LSM9DS1Handle* imu);
void readMag(LSM9DS1Handle* imu);
void readAccel(LSM9DS1Handle* imu);
float readTemp();

float calcGyro(float gyro);
float calcMag(float mag);
float calcAccel(float accel);


void setGyroScale(LSM9DS1Handle* imu,uint16_t gyro_scale);
void setMagScale(LSM9DS1Handle* imu, uint8_t mag_scale);
void setAccelScale(LSM9DS1Handle* imu, uint8_t accel_scale);

void setGyroODR(LSM9DS1Handle* imu, uint8_t gyro_Rate);
void setAccelODR(LSM9DS1Handle* imu, uint8_t accel_Rate);
void setMagODR(LSM9DS1Handle* imu, uint8_t mag_Rate);

void sleepGyro(uint8_t enable);


uint8_t I2CReadByte(uint8_t address, uint8_t reg);
uint8_t I2CReadBytes(uint8_t address, uint8_t reg, uint8_t* buffer, uint8_t numBytes);
void I2CWriteBytes(uint8_t dev_address, uint8_t reg, uint8_t address, uint8_t data);
HAL_StatusTypeDef WriteByte(uint8_t address, uint8_t reg, uint8_t data);





// get resolution of sensors
void getGyroRes();
void getMagRes();
void getAccRes();

void clampScales(LSM9DS1Handle* imu);


uint8_t CheckSensorID(uint8_t address, uint8_t reg);


uint8_t I2CreadByte(uint8_t address, uint8_t subAddress);
uint8_t I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count);

uint8_t ReadByteXG(uint8_t address);
uint8_t ReadBytesXG(uint8_t address, uint8_t* buffer, uint8_t numBytes);
void WriteByteXG(uint8_t address, uint8_t data);


void calcGyroResolution(LSM9DS1Handle* imu);
void calcAccResoltuion(LSM9DS1Handle* imu);
void calcMagResolution(LSM9DS1Handle* imu);




float gyro_res;
float mag_res;
float acc_res;























