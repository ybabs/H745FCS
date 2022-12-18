/*
 * common.h
 *
 *  Created on: 12 Oct 2022
 *      Author: Daniel
 */

#ifndef INC_UARTCOMMON_H_
#define INC_UARTCOMMON_H_

#include "stdint.h"


#ifdef __cplusplus
extern "C" {
#endif

	#define SBUS_PACKET_LEN  25
	extern uint8_t sbus_buffer[SBUS_PACKET_LEN];    // Used by UART 4 to dump data from the FRSKy into it.

#ifdef __cplusplus
}
#endif


#endif /* INC_UARTCOMMON_H_ */
