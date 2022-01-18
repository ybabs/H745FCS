/*
 * sbus.h
 *
 *  Created on: Nov 6, 2021
 *      Author: Daniel Babatunde
 */

#ifndef INC_SBUS_H_
#define INC_SBUS_H_

#include <stdint.h>

#define SBUS_RX_LEN 100
#define SBUS_RX_BUF_SIZE 1

uint8_t sbus_rx_buffer[SBUS_RX_LEN];
uint16_t sbus_rx_sta_flag;
uint8_t a_rx_buf[SBUS_RX_BUF_SIZE];




#endif /* INC_SBUS_H_ */
