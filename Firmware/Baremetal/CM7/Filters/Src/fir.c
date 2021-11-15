#include "fir.h"

static float FIR_IMPULSE_RESPONSE[RING_BUFFER_SIZE] = {};

static int setupBuffer(void)
{

  buffer_handle = bufferInit(ring_buffer_storage, RING_BUFFER_SIZE);
  return 0;
}
static int destroyBuffer(void)
{
  bufferFree(buffer_handle);
  return 0;
}


void FIRInit(FIRFilter* fir)
{

  // Setup the Buffer Array and initialise with zero
  setupBuffer();

  fir->buffer_handle = buffer_handle;

  // reset Buffer Index
  fir->buf_Index = 0;

  // clear filter output
  fir->output = 0.0;

}


void FIRUpdate(FIRFilter *fir, float input)
{
  bufferInsertOverwrite(fir->buffer_handle, input);

  fir->output = 0.0;

}
