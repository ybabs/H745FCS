/*
 * A ring buffer API implementation as described by
 * Embedded Artistry in https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/
 *
 * Author:
 *
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef float RingBufElement;
typedef uint16_t RingBufCtr;

typedef struct {
    RingBufElement *buf; /*!< pointer to the start of the ring buffer */
    RingBufCtr     end;  /*!< offset of the end of the ring buffer */
    RingBufCtr     head; /*!< offset to where next byte will be inserted */
    RingBufCtr     tail; /*!< offset of where next byte will be extracted */
} RingBuf;

typedef void (*RingBufHandler)(RingBufElement const el);

void RingBuf_ctor(RingBuf * const me,
                  RingBufElement sto[], RingBufCtr sto_len);
bool RingBuf_put(RingBuf * const me, RingBufElement const el);
bool RingBuf_get(RingBuf * const me, RingBufElement *pel);
void RingBuf_process_all(RingBuf * const me, RingBufHandler handler);
RingBufCtr RingBuf_num_free(RingBuf * const me);

#endif
