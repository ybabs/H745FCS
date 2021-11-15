#include "ring_buffer.h"
#include  <assert.h>
#include <stdlib.h>

// Definition for buffer structure

struct circular_buf_t{
  float * buffer;
  size_t head;
  size_t tail;
  size_t max; // for the buffer
  uint8_t full;
};


static void advancePointer(buf_handle_t cbuf)
{
  assert(cbuf);

  if(cbuf->full)
  {

    if(++ (cbuf->tail) == cbuf->max)
    {
      cbuf->tail = 0;
    }

  }

  if(++ (cbuf->head) == cbuf->max)
  {
    cbuf->head = 0;
  }
  cbuf->full = (cbuf->head == cbuf->tail);
}

static void retreatPointer(buf_handle_t cbuf)
{
  assert(cbuf);

  cbuf->full = false;
  if(++(cbuf->tail)==cbuf->max)
  {
    cbuf->tail = 0;
  }

}



buf_handle_t bufferInit(float * buffer, uint8_t size)
{

  // Check that both buffer and size are valid numbers
  assert(buffer && size);

  buf_handle_t cbuf = malloc(sizeof(circular_buf_t));
  assert(cbuf);

  cbuf->buffer = buffer;
  cbuf->max = size;
  bufferReset(cbuf);

  assert(BufferIsEmpty(cbuf));

  return cbuf;

}


void bufferFree(buf_handle_t cbuf)
{
  assert(cbuf);
  free(cbuf);
}


void bufferReset(buf_handle_t cbuf)
{

  assert(cbuf);
  cbuf->head = 0;
  cbuf->tail = 0;
  cbuf->full = false;

}



void bufferInsertOverwrite(buf_handle_t cbuf, float data)
{
  assert(cbuf && cbuf->buffer);

  cbuf->buffer[cbuf->head] = data;
  advancePointer(cbuf);

}


int bufferInsertReject(buf_handle_t cbuf, float data)
{
  int r = -1;

  assert(cbuf && cbuf->buffer);

  if(!BufferIsFull(cbuf))
  {
    cbuf->buffer[cbuf->head] = data;
    advancePointer(cbuf);
    r = 0;
  }

  return r;

}


int bufferGet(buf_handle_t cbuf, float * data)
{

  assert(cbuf && data && cbuf->buffer);

  int r = -1;

  if(!BufferIsEmpty(cbuf) )
  {
    *data = cbuf->buffer[cbuf->tail];
    retreatPointer(cbuf);
    r = 0;
  }

  return r;

}



bool BufferIsEmpty(buf_handle_t cbuf)
{
  assert(cbuf);

  return(!cbuf->full && (cbuf->head == cbuf->tail));
}


bool BufferIsFull(buf_handle_t cbuf)
{
   assert(cbuf);

   return cbuf->full;
}

size_t BufferCapacity(buf_handle_t cbuf)
{
  assert(cbuf);

  return cbuf->max;
}

size_t BufferSize(buf_handle_t cbuf)
{
  assert(cbuf);

  size_t size = cbuf->max;

  if(!cbuf->full)
  {
    if(cbuf->head >= cbuf->tail)
    {
      size = (cbuf->head - cbuf->tail);
    }

    else
    {
      size = (cbuf->max + cbuf->head - cbuf->tail);
    }
  }

  return size;

}
