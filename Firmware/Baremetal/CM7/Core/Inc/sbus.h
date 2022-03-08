/*
 * sbus.h
 *
 *  Created on: Nov 6, 2021
 *      Author: Daniel Babatunde
 */

#ifndef INC_SBUS_H_
#define INC_SBUS_H_

#include <stdint.h>

// MAX 1811 MIN 172 CENTER 1018 (THROTTLE)  CENTER RS (989)

#define SBUS_SIGNAL_OK        0x00
#define SBUS_SIGNAL_LOST      0x01
#define SBUS_SIGNAL_FAILSAFE  0x03
#define SBUS_HEADER           0x0F
#define SBUS_PACKET_LEN       25


typedef struct
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
} SBUS_CH;

uint16_t failsafe_status;
uint16_t channel_17;

extern SBUS_CH sbus_channel;
void updateSbus(uint8_t *buf);
extern uint8_t sbus_buffer[SBUS_PACKET_LEN];


#endif /* INC_SBUS_H_ */
