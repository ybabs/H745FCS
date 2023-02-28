/*
 * serializer.cpp
 *
 *  Created on: 19 Oct 2022
 *      Author: Daniel
 */

#include "serializer.h"

Serializer::Serializer():
		usb_timer(0),
		acc_timer(0),
		gyro_timer(0),
		gps_timer(0),
		mag_timer(0),
		baro_timer(0)
{

}

uint8_t Serializer::ComputeCRC8(uint8_t* data, size_t len)
{
    uint32_t i = 0;
    uint8_t crc8 = 0;
    for (i = 0; i < len; i++)
    {
        crc8 = crc_table[crc8 ^ data[i]];
    }
    return crc8;
}


int Serializer::SendDataPacket(const sensor_cmd_code_t& code, const uint8_t* data, size_t len)
{

	m_data_packet.preamble[0] = UART_PREAMBLE_1;
	m_data_packet.preamble[1] = UART_PREAMBLE_2;
	m_data_packet.command_code = to_underlying(code);
	m_data_packet.data_length = len;
	std::memcpy(m_data_packet.data, &data[0], len);
	uint8_t crc_val = ComputeCRC8((uint8_t *) &m_data_packet, 4U + len);
	m_data_packet.crc = crc_val;
	CDC_Transmit_FS((uint8_t*)&m_data_packet, sizeof(m_data_packet));

	// Wait for Transmission to complete
//	while (CDC_Transmit_FS((uint8_t*)&m_data_packet, sizeof(m_data_packet)) != USBD_OK) {
//	}

	return 0;
}

//__attribute__((unused)) int Serializer::SendResponse(const sensor_cmd_code_t& code)
//{
//	return 0;
//}

void Serializer::SendData(const gyroData& data)
{
	 uint8_t frame[sizeof(data)];
	 std::memcpy(frame, (uint8_t*)&data, sizeof(data));
	 SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_GYRO, frame, sizeof(frame)/sizeof(frame[0]));
}

void Serializer::SendData(const magData& data)
{
      uint8_t frame[sizeof(data)];
      std::memcpy(frame, (uint8_t*)&data, sizeof(data));
      SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_MAG, frame, sizeof(frame)/sizeof(frame[0]));
}

void Serializer::SendData(const accelData& data)
{
	  uint8_t frame[sizeof(data)];
	  std::memcpy(frame, (uint8_t*)&data, sizeof(data));
	  SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_ACC, frame, sizeof(frame)/sizeof(frame[0]));

}

void Serializer::SendData(const baroData& data)
{
	  uint8_t frame[sizeof(data)];
	  std::memcpy(frame, (uint8_t*)&data, sizeof(data));
	  SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_BARO, frame, sizeof(frame)/sizeof(frame[0]));
	  baro_timer = HAL_GetTick();

}

void Serializer::SendData(const gpsData& data)
{
	 uint8_t frame[sizeof(data)];
	 std::memcpy(frame, (uint8_t*)&data, sizeof(data));
	 SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_GPS, frame, sizeof(frame)/sizeof(frame[0]));
	  gps_timer = HAL_GetTick();
}

