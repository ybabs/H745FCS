/*
 * sbus.hpp
 *
 *  Created on: 12 Oct 2022
 *      Author: Daniel
 */

#pragma once

#include <stdint.h>
#include <common.h>

static constexpr uint8_t SBUS_SIGNAL_OK = 0x00;
static constexpr uint8_t SBUS_SIGNAL_LOST 		= 0x01;
static constexpr uint8_t SBUS_SIGNAL_FAILSAFE 	= 0x01;
static constexpr uint8_t SBUS_HEADER	 		= 0x0F;


class SbusController
{
public:
	explicit SbusController();
	~SbusController();
	struct SBUS_CH
	{
		  uint16_t channel1;
		  uint16_t channel2;
		  uint16_t channel3;
		  uint16_t channel4;
		  uint16_t channel5;
		  uint16_t channel6;
		  uint16_t channel7;
		  uint16_t channel8;
		  uint16_t channel9;
		  uint16_t channel10;
		  uint16_t channel11;
		  uint16_t channel12;
		  uint16_t channel13;
		  uint16_t channel14;
		  uint16_t channel15;
		  uint16_t channel16;
	};
	void UpdateSbus(uint8_t *buf);

private:
	uint16_t channel_17;
	uint8_t failsafe_status;
	SBUS_CH sbus_channel;


};


