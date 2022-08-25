#ifndef UARTCONFIG_H
#define UARTCONFIG_H


enum class UartFSM
{
    WAIT_PREAMBLE_1,
    WAIT_PREAMBLE_2,
    WAIT_COMMAND,
    WAIT_DATA_LENGTH,
    WAIT_DATA,
    WAIT_CRC
};

enum class UartErrorCode
{
    INVALID_COMMAND  = 0x02,
    INVALID_DATA     = 0x03,
    INVALID_CRC      = 0x04,
    INVALID_LENGTH   = 0x05
};

enum class SensorCommandCode
{
    SENSOR_COMMAND_GYRO = 0x20,
    SENSOR_COMMAND_ACC  = 0x21,
    SENSOR_COMMAND_GPS  = 0x22,
    SENSOR_COMMAND_BARO = 0x23,
    SENSOR_COMMAND_MAG  = 0x24
};

enum class SensorDataLength
{
    GYRO_DATA = 12,
    ACC_DATA  = 12,
    GPS_DATA  = 29,
    BARO_DATA = 12,
    MAG_DATA  = 12
};

#endif // UARTCONFIG_H
