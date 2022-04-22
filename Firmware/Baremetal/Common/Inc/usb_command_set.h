#ifndef USB_COMMAND_SET_H
#define USB_COMMAND_SET_H

#define UART_PREAMBLE_1 0x7F
#define UART_PREAMBLE_2 0x3A

#define GYRO_DATA_LEN   12
#define ACC_DATA_LEN    12
#define GPS_DATA_LEN    29
#define BARO_DATA_LEN   12
#define MAG_DATA_LEN    12

enum sensor_cmd_code_t {

  SENSOR_COMMAND_GYRO = 0x20,
  SENSOR_COMMAND_ACC  = 0x21,
  SENSOR_COMMAND_GPS  = 0x22,
  SENSOR_COMMAND_BARO = 0x23,
  SENSOR_COMMAND_MAG  = 0x24
};

enum response_code_t {
  RESPONSE_OK               = 0x00,
  RESPONSE_TIMEOUT          = 0x01,
  RESPONSE_INVALID_COMMAND  = 0x02,
  RESPONSE_INVALID_DATA     = 0x03,
  RESPONSE_CRC_ERROR        = 0x04
};

struct usb_packet_t {
  uint8_t preamble[2];
  uint8_t command_code;
  uint8_t data_length;
  uint8_t data[32];
  uint8_t crc;
}__attribute__((__packed__));


// USB RX State Machine
enum uart_state_machine_t {
  WAIT_PREAMBLE_1     = 0x00,
  WAIT_PREAMBLE_2     = 0x01,
  WAIT_COMMAND        = 0x02,
  WAIT_DATA_LENGTH    = 0x03,
  WAIT_DATA           = 0x04,
  WAIT_CRC            = 0x05
};
#endif
