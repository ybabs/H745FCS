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

	return 0;
}

//__attribute__((unused)) int Serializer::SendResponse(const sensor_cmd_code_t& code)
//{
//	return 0;
//}

void Serializer::SendData(const gyro_data& data)
{
  if((HAL_GetTick() - gyro_timer) >= GYRO_UPDATE_RATE_MS)
	{
	 uint8_t frame[sizeof(data)];
	 std::memcpy(frame, (uint8_t*)&data, sizeof(data));
	 SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_GYRO, frame, sizeof(frame)/sizeof(frame[0]));
	 gyro_timer = HAL_GetTick();
	}
}

void Serializer::SendData(const mag_data& data)
{
  if((HAL_GetTick() - mag_timer) >= MAG_UPDATE_RATE_MS)
  {
      uint8_t frame[sizeof(data)];
      std::memcpy(frame, (uint8_t*)&data, sizeof(data));
      SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_MAG, frame, sizeof(frame)/sizeof(frame[0]));
      mag_timer = HAL_GetTick();
   }
}

void Serializer::SendData(const acc_data& data)
{
  if((HAL_GetTick() - acc_timer) >= ACC_UPDATE_RATE_MS)
  {
	  uint8_t frame[sizeof(data)];
	  std::memcpy(frame, (uint8_t*)&data, sizeof(data));
	  SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_ACC, frame, sizeof(frame)/sizeof(frame[0]));
	  acc_timer = HAL_GetTick();
  }
}

void Serializer::SendData(const baro_data& data)
{
  if((HAL_GetTick() - baro_timer) >= BARO_UPDATE_RATE_MS)
	{
	  uint8_t frame[sizeof(data)];
	  std::memcpy(frame, (uint8_t*)&data, sizeof(data));
	  SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_BARO, frame, sizeof(frame)/sizeof(frame[0]));
	  baro_timer = HAL_GetTick();
	}
}

void Serializer::SendData(const gps_data& data)
{
  if((HAL_GetTick() - gps_timer) >= GPS_UPDATE_RATE_MS)
	{
	 uint8_t frame[sizeof(data)];
	 std::memcpy(frame, (uint8_t*)&data, sizeof(data));
	 SendDataPacket(sensor_cmd_code_t::SENSOR_COMMAND_GPS, frame, sizeof(frame)/sizeof(frame[0]));
	  gps_timer = HAL_GetTick();
	}
}

