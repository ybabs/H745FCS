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


// Buffer Structure
typedef struct circular_buf_t circular_buf_t;

typedef circular_buf_t* buf_handle_t;


/* Passes in a storage buffer and size
 * @param buffer: array holding values
 * @param size: size of buffer
 * @returns: a circular buffer handle
 */
buf_handle_t bufferInit(float * buffer, uint8_t size);

/* Frees the buffer structure
 * Note this does not free data in the buffer
 * @param cbuf: Buffer Handle
 * @returns: nothing
 */
void bufferFree(buf_handle_t cbuf);

/*
 * Reset Circular Buffer to empty by making
 * head == tail
 * @param cbuf: Buffer Handle
 * @returns: nothing
 */
void bufferReset(buf_handle_t cbuf);

/*
 * Adds data to buffer
 * Overwrites existing data
 * if current buffer is full
 * @param cbuf: buffer handle
 * @param data: data to be added
 * @return: nothing
 */

void bufferInsertOverwrite(buf_handle_t cbuf, float data);

/*
 * Adds data to buffer
 * Overwrites existing data
 * if current buffer is full
 * @param cbuf: buffer handle
 * @param data: data to be added
 * @return: 0 if succesful, -1 if buffer is full
 */
int bufferInsertReject(buf_handle_t cbuf, float data);

/*
 * Retrieve a value from the buffer
 * @param cbuf: buffer Handle
 * @param data: value to be retrieved
 * @return: 0 if successful, -1 if buffer is empty
 */
int bufferGet(buf_handle_t cbuf, float * data);

/*
 * Checks if buffer is empty
 * @param cbuf: buffer Handle
 * @return: returns true if buffer is empty and false if buffer is full
 */

bool BufferIsEmpty(buf_handle_t cbuf);

/*
 * Checks if buffer is full
 * @param cbuf: buffer Handle
 * @return: returns true if buffer is full and false if buffer is empty
 */
bool BufferIsFull(buf_handle_t cbuf);

/*
 * Returns Maximum Capacity of Buffer
 * @param cbuf: buffer handle
 * @returns: max capacity of buffer
 */

size_t BufferCapacity(buf_handle_t cbuf);

/*
 * Returns current number of elements in buffer
 * @param cbuf: buffer handle
 * @returns: number of elements in buffer
 */
size_t BufferSize(buf_handle_t cbuf);



#endif
