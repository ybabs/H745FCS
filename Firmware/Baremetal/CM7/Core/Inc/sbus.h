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

/* Easy method is to grab 25 bytes everytime and parse that data
 * using DMA CIRCULAR. A more thorough solution would involve
 * reading a single byte each time and using a state machine to
 * parse that data using a state machine similar to the bolderflight
 * implementation. This would require NORMAL DMA CALLS to read each byte
 * when the parseSbus function is called. This may or may not be implemented
 * in future improvements
 * */
#define USE_DMA_CIRCULAR 1
#define USE_DMA_NORMAL   0




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

//TODO Future implementation definitions
//uint8_t updateSbus();
//uint8_t ch17_data = 0;
//uint8_t ch18_data = 0;
//uint8_t new_data = 0;
//
//uint8_t readSbus();
//uint8_t parseSbus();
//#define SBUS_PACKET_LEN       25
//#define SBUS_NUM_CHANNEL      16
//#define SBUS_HEADER           0x0F
//#define SBUS_FOOTER           0x00
//#define SBUS_FOOTER2          0x04
//#define SBUS_CH17_MASK        0x01
//#define SBUS_CH18_MASK        0x02
//#define SBUS_LOST_FRAME_MASK  0x04
//#define SBUS_FAILSAFE_MASK    0x08
//
////if (huart->Instance == UART4)
//{
//  new_data = 0;
//  if (parseSbus() == 0)
//  {
//    new_data = 1;
//  }
//
//  if (new_data == 1)
//  {
//    updateSbus(sbus_buffer);
//  }
//
//}


#endif /* INC_SBUS_H_ */
