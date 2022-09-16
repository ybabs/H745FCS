#include "lsm9ds1_registers.hpp"
#include "i2c.h"
#include <type_traits>


class IMU
{

  struct GyroSettings
  {
    bool enabled;
    uint16_t scale;
    uint8_t sampleRate;
    uint8_t bandwidth;
    bool lowPowerEnable;
    bool HPFEnable;
    uint8_t HPFCutoff;
    bool flipX;
    bool flipY;
    bool flipZ;
    uint8_t orientation;
    bool enableX;
    bool enableY;
    bool enableZ;
    uint8_t latchInterrupt;
  };

  struct MagSettings
  {
    bool enabled;
    uint8_t scale;
    uint8_t sampleRate;
    uint8_t tempCompensationEnable;
    uint8_t XYPerformance;
    uint8_t ZPerformance;
    bool lowPowerEnable;
    uint8_t operatingMode;
  };

  struct TempSettings
  {
    uint8_t enabled;
  };

  struct AccelSettings
  {
    bool enabled;
    uint8_t scale;
    uint8_t sampleRate;
    bool enableX; //
    bool enableY; //
    bool enableZ; //
    int8_t bandwidth;
    uint8_t highResEnable;
    uint8_t highResBandwidth;
  };

public:

  IMU();

  struct gyroData
  {
    float x;
    float y;
    float z;
  };

  struct accelData
  {
    float x;
    float y;
    float z;
  };

  struct magData
  {
    float x;
    float y;
    float z;
  };

  gyroData  GetGyroData();
  accelData GetAccelData();
  magData   GetMagData();

private:




  float gyro_res;
  float mag_res;
  float acc_res;
  float temperature;

  GyroSettings gyro_settings;
   AccelSettings accel_settings;
   MagSettings mag_settings;
   TempSettings temp_settings;
   magData mag_values;
   accelData accel_values;
   gyroData gyro_values;


  bool Setup();
  void Init();
  void InitGyro();
  void InitAccel();
  void InitMag();
  void GetGyroRes();
  void GetMagRes();
  void GetAccRes();
  void ClampScales();
  uint8_t CheckSensorID(const uint8_t &address, const uint8_t &reg);

  uint8_t ConfigIMU();

  uint8_t AccelAvailable();
  uint8_t GyroAvailable();
  uint8_t TempAvailable();
  uint8_t MagAvailable();

  void ReadGyro();
  void ReadMag();
  void ReadAccel();
  void ReadTemp();

  float CalcGyro(float gyro);
  float CalcMag(float mag);
  float CalcAccel(float accel);

  void CalcGyroResolution();
  void CalcAccResolution();
  void CalcMagResolution();

  void SetGyroScale(uint16_t& gyro_scale);
  void SetMagScale(uint8_t& mag_scale);
  void SetAccelScale(uint8_t& accel_scale);

  void SetGyroODR(uint8_t gyro_Rate);
  void SetAccelODR(uint8_t accel_Rate);
  void SetMagODR(uint8_t mag_Rate);
void SleepGyro(uint8_t enable);

  uint8_t I2CReadByte(const uint8_t &address, const uint8_t &reg);
  uint8_t I2CReadBytes(const uint8_t &address, const uint8_t &reg,  uint8_t* buffer, uint8_t numBytes);
  HAL_StatusTypeDef WriteByte(const uint8_t &address, const uint8_t &reg, uint8_t data);


  template <typename E>
  constexpr auto to_underlying(E e) noexcept
  {
      return static_cast<std::underlying_type_t<E>>(e);
  }



};
