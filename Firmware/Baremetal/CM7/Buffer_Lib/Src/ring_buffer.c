#include "ring_buffer.h"
#include  <assert.h>
#include <stdlib.h>

void RingBuf_ctor(RingBuf * const me,
                  RingBufElement sto[], RingBufCtr sto_len) {
    me->buf  = &sto[0];
    me->end  = sto_len;
    me->head = 0U;
    me->tail = 0U;
}
/*..........................................................................*/
bool RingBuf_put(RingBuf * const me, RingBufElement const el) {
    RingBufCtr head = me->head + 1U;
    if (head == me->end) {
        head = 0U;
    }
    if (head != me->tail) { /* buffer NOT full? */
        me->buf[me->head] = el;
        me->head = head; /* update the head to a *valid* index */
        return true;  /* element placed in the buffer */
    }
    else {
        return false; /* element NOT placed in the buffer */
    }
}
/*..........................................................................*/
bool RingBuf_get(RingBuf * const me, RingBufElement *pel) {
    RingBufCtr tail = me->tail;
    if (me->head != tail) { /* ring buffer NOT empty? */
        *pel = me->buf[tail];
        ++tail;
        if (tail == me->end) {
            tail = 0U;
        }
        me->tail = tail; /* update the tail to a *valid* index */
        return true;
    }
    else {
        return false;
    }
}
/*..........................................................................*/
void RingBuf_process_all(RingBuf * const me, RingBufHandler handler) {
    RingBufCtr tail = me->tail;
    while (me->head != tail) { /* ring buffer NOT empty? */
        (*handler)(me->buf[tail]);
        ++tail;
        if (tail == me->end) {
            tail = 0U;
        }
        me->tail = tail; /* update the tail to a *valid* index */
    }
}
/*..........................................................................*/
RingBufCtr RingBuf_num_free(RingBuf * const me) {
    RingBufCtr head = me->head;
    RingBufCtr tail = me->tail;
    if (head == tail) { /* buffer empty? */
        return (RingBufCtr)(me->end - 1U);
    }
    else if (head < tail) {
        return (RingBufCtr)(tail - head - 1U);
    }
    else {
        return (RingBufCtr)(me->end + tail - head - 1U);
    }
}
