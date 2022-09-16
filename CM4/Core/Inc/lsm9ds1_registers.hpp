#pragma once

#include <cstdint>

namespace gyroaccaddress
{

static constexpr uint8_t LSM9DS1_AG_ADDR        = 0x6B;
static constexpr uint8_t REG_ACT_THS            = 0x04;
static constexpr uint8_t REG_ACT_DUR            = 0x05;
static constexpr uint8_t REG_INT_GEN_CFG_XL     = 0x06;
static constexpr uint8_t REG_INT_GEN_THS_X_XL   = 0x07;
static constexpr uint8_t INT_GEN_THS_Y_XL       = 0x08;
static constexpr uint8_t INT_GEN_THS_Z_XL       = 0x09;
static constexpr uint8_t INT_GEN_DUR_XL         =0x0A;
static constexpr uint8_t INT1_CTRL              =0x0C;
static constexpr uint8_t INT2_CTRL              =0x0D;
static constexpr uint8_t WHO_AM_I_XG            =0x0F;
static constexpr uint8_t CTRL_REG1_G            =0x10;
static constexpr uint8_t CTRL_REG2_G            =0x11;
static constexpr uint8_t CTRL_REG3_G            =0x12;
static constexpr uint8_t ORIENT_CFG_G           =0x13;
static constexpr uint8_t INT_GEN_SRC_G          =0x14;
static constexpr uint8_t OUT_TEMP_L             =0x15;
static constexpr uint8_t OUT_TEMP_H             =0x16;
static constexpr uint8_t STATUS_REG_0           =0x17;
static constexpr uint8_t OUT_X_G_L              =0x18;
static constexpr uint8_t OUT_X_G_H              =0x19;
static constexpr uint8_t OUT_Y_G_L              =0x1A;
static constexpr uint8_t OUT_Y_G_H              =0x1B;
static constexpr uint8_t OUT_Z_G_L              =0x1C;
static constexpr uint8_t OUT_Z_G_H              =0x1D;
static constexpr uint8_t CTRL_REG4              =0x1E;
static constexpr uint8_t CTRL_REG5_XL           =0x1F;
static constexpr uint8_t CTRL_REG6_XL           =0x20;
static constexpr uint8_t CTRL_REG7_XL           =0x21;
static constexpr uint8_t CTRL_REG8              =0x22;
static constexpr uint8_t CTRL_REG9              =0x23;
static constexpr uint8_t CTRL_REG10             =0x24;
static constexpr uint8_t INT_GEN_SRC_XL         =0x26;
static constexpr uint8_t STATUS_REG_1           =0x27;
static constexpr uint8_t OUT_X_XL_L             =0x28;
static constexpr uint8_t OUT_X_XL_H             =0x29;
static constexpr uint8_t OUT_Y_XL_L             =0x2A;
static constexpr uint8_t OUT_Y_XL_H             =0x2B;
static constexpr uint8_t OUT_Z_XL_L             = 0x2C;
static constexpr uint8_t OUT_Z_XL_H             = 0x2D;
static constexpr uint8_t FIFO_CTRL              = 0x2E;
static constexpr uint8_t FIFO_SRC               = 0x2F;
static constexpr uint8_t INT_GEN_CFG_G          = 0x30;
static constexpr uint8_t INT_GEN_THS_XH_G       = 0x31;
static constexpr uint8_t INT_GEN_THS_XL_G       = 0x32;
static constexpr uint8_t INT_GEN_THS_YH_G       = 0x33;
static constexpr uint8_t INT_GEN_THS_YL_G       = 0x34;
static constexpr uint8_t INT_GEN_THS_ZH_G       = 0x35;
static constexpr uint8_t INT_GEN_THS_ZL_G       = 0x36;
static constexpr uint8_t INT_GEN_DUR_G          = 0x37;

}


//////////////////////////////////
// Magenetometer Registers     //
/////////////////////////////////

namespace magaddress
{
static constexpr uint8_t LSM9DS1_M_ADDR         = 0x1E;
static constexpr uint8_t  OFFSET_X_REG_M_L   =   0x05;
static constexpr uint8_t  OFFSET_X_REG_M_H   =   0x06;
static constexpr uint8_t  OFFSET_Y_REG_M_L    =  0x07;
static constexpr uint8_t  OFFSET_Y_REG_M_H   =   0x08;
static constexpr uint8_t  OFFSET_Z_REG_M_L    =  0x09;
static constexpr uint8_t  OFFSET_Z_REG_M_H    =  0x0A;
static constexpr uint8_t  WHO_AM_I_M          =  0x0F;
static constexpr uint8_t  CTRL_REG1_M         =  0x20;
static constexpr uint8_t  CTRL_REG2_M         =  0x21;
static constexpr uint8_t  CTRL_REG3_M         =  0x22;
static constexpr uint8_t  CTRL_REG4_M         =  0x23;
static constexpr uint8_t  CTRL_REG5_M         =  0x24;
static constexpr uint8_t  STATUS_REG_M        =  0x27;
static constexpr uint8_t  OUT_X_M_L           =  0x28;
static constexpr uint8_t  OUT_X_M_H           =  0x29;
static constexpr uint8_t  OUT_Y_M_L           =  0x2A;
static constexpr uint8_t  OUT_Y_M_H           =  0x2B;
static constexpr uint8_t  OUT_Z_M_L           =  0x2C;
static constexpr uint8_t  OUT_Z_M_H           =  0x2D;
static constexpr uint8_t  INT_CFG_M           =  0x30;
static constexpr uint8_t  INT_SRC_M           =  0x31;
static constexpr uint8_t  INT_THS_L_M         =  0x32;
static constexpr uint8_t  INT_THS_H_M         =  0x33;

}

namespace imuresponse
{
static constexpr uint8_t WHO_AM_I_AG_RSP    =   0x68;
static constexpr uint8_t WHO_AM_I_M_RSP     =   0x3D;
}

namespace resolution
{
static constexpr  float  ACC_FS_2G         =     0.000061;
static constexpr  float  ACC_FS_4G         =     0.000122;
static constexpr  float  ACC_FS_8G         =     0.000244;
static constexpr  float  ACC_FS_16G        =     0.000732;
static constexpr  float  MAG_FS_4G         =     0.00014;
static constexpr  float  MAG_FS_8G         =     0.00029;
static constexpr  float  MAG_FS_12G        =     0.00043;
static constexpr  float  MAG_FS_16G        =     0.00058;
static constexpr  float  GYRO_FS_245       =     0.00875;
static constexpr  float  GYRO_FS_500       =     0.0175;
static constexpr  float  GYRO_FS_2000      =     0.07;

}

namespace settings
{

enum class MagOpMode
{
  MAG_LP_MODE     = 0x00,
  MAG_MP_MODE     = 0x20,
  MAG_HP_MODE     = 0x40,
  MAG_UHP_MODE    = 0x60
};

enum class MagZOpMode
{
  MAG_LP_MODE     = 0x00,
  MAG_MP_MODE     = 0x04,
  MAG_HP_MODE     = 0x08,
  MAG_UHP_MODE    = 0x0C
};

enum class MagODR
{
    MAG_ODR_0_625HZ   = 0x00,
    MAG_ODR_1_25HZ    = 0x04,
    MAG_ODR_2_5HZ     = 0x08,
    MAG_ODR_5HZ       = 0x0C,
    MAG_ODR_10HZ      = 0x10,
    MAG_ODR_20HZ      = 0x14,
    MAG_ODR_40HZ      = 0x18,
    MAG_ODR_80HZ      = 0x1C
};

enum class MagFullScale
{
  MAG_4G          = 0x00,
  MAG_8G          = 0x20,
  MAG_12G         = 0x40,
  MAG_16G         = 0x60
};

enum class MagFIFOMode
{
  FIFO_OFF        = 0x00,
  FIFO_MODE       = 0x20,
  FIFO_CONT_TRG   = 0x60,
  FIFO_BYP_TRG    = 0x80,
  FIFO_CONT       = 0xC0
};

enum class GyroODR
{
  ODR_PD          = 0x00,
  ODR_14_9        = 0x20,
  ODR_59_5        = 0x40,
  ODR_119         = 0x70,
  ODR_238         = 0x80,
  ODR_476         = 0xA0,
  ODR_952         = 0xC0
};

enum class AccODR
{
  XL_ODR_PD       = 0x00,
  XL_ODR_10       = 0x20,
  XL_ODR_50       = 0x40,
  XL_ODR_119      = 0x60,
  XL_ODR_238      = 0x80,
  XL_ODR_476      = 0xA0,
  XL_ODR_952      = 0xC0
};

enum class AccFullScale
{
  XL_FS_2G        = 0x00,
  XL_FS_4G        = 0x10,
  XL_FS_8G        = 0x18,
  XL_FS_16G       = 0x08
};

enum class AccBandwidth
{
  XL_BW_50        = 0x03,
  XL_BW_105       = 0x02,
  XL_BW_211       = 0x01,
  XL_BW_408       = 0x00
};

enum class GyroFullScale
{
  FS_245          = 0x00,
  FS_500          = 0x08,
  FS_2000         = 0x18
};

enum class GyroBandwidth
{
  BW_00          = 0x00,
  BW_10          = 0x02,
  BW_01          = 0x01,
  BW_11          = 0x03
};

enum class GyroMode
{
  ODR_POWERDOWN  = ((uint8_t)0x00),
  ODR_14_9HZ     = ((uint8_t)0x01),
  ODR_59_5HZ     = ((uint8_t)0x02),
  ODR_119HZ      = ((uint8_t)0x03),
  ODR_238HZ      = ((uint8_t)0x04),
  ODR_476HZ      = ((uint8_t)0x05),
  ODR_952HZ      = ((uint8_t)0x06)
};

enum class MagMode
{
  MAG_ODR_0_625HZ = 0x00,
  MAG_ODR_1_25HZ  = 0x01,
  MAG_ODR_2_5HZ   = 0x02,
  MAG_ODR_5HZ     = 0x03,
  MAG_ODR_10HZ    = 0x04,
  MAG_ODR_20HZ    = 0x05,
  MAG_ODR_40HZ    = 0x06,
  MAG_ODR_80HZ    = 0x07
};

enum class AccIntGenerator
{
  XLIE_XL         = ((uint8_t)0x01),
  XHIE_XL         = ((uint8_t)0x02),
  YLIE_XL         = ((uint8_t)0x04),
  YHIE_XL         = ((uint8_t)0x08),
  ZLIE_XL         = ((uint8_t)0x10),
  ZHIE_XL         = ((uint8_t)0x20),
  GEN_6D          = ((uint8_t)0x40),
  AOI_XL          = ((uint8_t)0x80),
};

enum class INTRegGenerator
{
  INT_DRDY_XL    = ((uint8_t) 0x01), // Accelerometer data ready INT1 and INT2
  INT_DRDY_G     = ((uint8_t) 0x02), // Gyro data ready INT1 and INT2
  INT1_BOOT      = ((uint8_t) 0x04), // INt 1 Boot
  INT2_DRDY_TEMP = ((uint8_t) 0x04), // INt 2 temp data ready
  INT_FTH        = ((uint8_t) 0x08),      // FIFO threshold interrupt (INT1 & INT2)
  INT_OVR        = ((uint8_t)0x10),      // Overrun interrupt (INT1 & INT2)
  INT_FSS5       = ((uint8_t)0x20),     // FSS5 interrupt (INT1 & INT2)
  INT_IG_XL      = ((uint8_t)0x40),   // Accel interrupt generator (INT1)
  INT1_IG_G      = ((uint8_t)0x80),   // Gyro interrupt enable (INT1)
  INT2_INACT     = ((uint8_t)0x80),   // Inactivity interrupt output (INT2)
};

}
