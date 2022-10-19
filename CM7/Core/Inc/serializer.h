/*
 * serializer.h
 *
 *  Created on: 19 Oct 2022
 *      Author: Daniel
 */

#pragma once

#include "usbd_cdc_if.h"
#include "usb_command_set.h"
#include "crclib.h"
#include <helpers.hpp>
#include <cstring>
#include <type_traits>


class Serializer
{
public:
	explicit Serializer();
	~Serializer();
	int SendDataPacket(const sensor_cmd_code_t& code, const uint8_t* data, size_t len );
	//__attribute__((unused)) int SendResponse(const sensor_cmd_code_t& code);
	void SendData(const gyro_data& data);
	void SendData(const mag_data& data);
	void SendData(const acc_data& data);
	void SendData(const baro_data& data) ;
	void SendData(const gps_data& data);
private:
	uint32_t usb_timer = 0;
	uint32_t acc_timer = 0;
	uint32_t gyro_timer = 0;
	uint32_t gps_timer = 0;
	uint32_t mag_timer = 0;
	uint32_t baro_timer = 0;
	struct usb_packet_t m_data_packet;

   template <typename E>
   constexpr auto to_underlying(E e) noexcept
   {
	   return static_cast<std::underlying_type_t<E>>(e);
   }

};
