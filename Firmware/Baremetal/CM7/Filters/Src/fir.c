#include "fir.h"

static float FIR_IMPULSE_RESPONSE[BUFFER_SIZE] = {0.0004935f,0.0007081f,-0.0007039f,-0.0007769f,0.0011041f,0.0008928f,-0.0017719f,-0.0009655f,0.0027688f,0.0008719f,-0.0041321f,-0.0004591f,0.0058687f,-0.0004541f,-0.0079513f,0.0020780f,0.0103177f,-0.0046625f,-0.0128740f,0.0085270f,0.0155003f,-0.0141365f,-0.0180593f,0.0222910f,0.0204070f,-0.0346554f,-0.0224045f,0.0555691f,0.0239290f,-0.1010953f,-0.0248846f,0.3166146f,0.5252101f,0.3166146f,-0.0248846f,-0.1010953f,0.0239290f,0.0555691f,-0.0224045f,-0.0346554f,0.0204070f,0.0222910f,-0.0180593f,-0.0141365f,0.0155003f,0.0085270f,-0.0128740f,-0.0046625f,0.0103177f,0.0020780f,-0.0079513f,-0.0004541f,0.0058687f,-0.0004591f,-0.0041321f,0.0008719f,0.0027688f,-0.0009655f,-0.0017719f,0.0008928f,0.0011041f,-0.0007769f,-0.0007039f,0.0007081f};
void FIRInit(FIRFilter* fir)
{

  // Setup the Buffer Array and initialise with zero
  for(uint8_t n = 0; n < BUFFER_SIZE; n++)
  {
    fir->buf[n] = 0.0f;
  }

  // reset Buffer Index
  fir->buffer_index = 0;

  // clear filter output
  fir->output = 0.0;

}


float FIRUpdate(FIRFilter *fir, float input)
{
 // Store latest sample in buffer   // buffer.put(&rb, input);
  fir->buf[fir->buffer_index] = input;

  // Increment buffer
  fir->buffer_index++;

  if(fir->buffer_index == BUFFER_SIZE)
  {
    fir->buffer_index = 0;
  }

  // CMOpute new output via convolution
  fir->output = 0.0;

  uint8_t sum_index = fir->buffer_index;

  for(uint8_t n = 0; n < BUFFER_SIZE; n++)
  {
     // check index and decrement index
    if(sum_index > 0)
    {
      sum_index--;
    }

    else
    {
      sum_index =BUFFER_SIZE - 1;
    }

    // Multiply impulse response with shifted impulse sample and add to output.
    fir->output += FIR_IMPULSE_RESPONSE[n] * fir->buf[sum_index];

  }

  return fir->output;

}
