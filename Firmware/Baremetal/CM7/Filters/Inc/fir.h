/* FIR Filter implementation from Phil's Lab video https://www.youtube.com/watch?v=uNNNj9AZisM&t=40s
 *
 * To determine when designing filter
 * Sampling frequency
 * Length of Filter (impulse response)
 * Frequency Response
 * Filter Type
 * Windowing function
 *
 * Convolution y[n] = Sum(h[j] . x[n-j]) limits from j to M-1
 *
 * h[j] = impulse response
 * x[n-j] = shifted input signal
 *
 *
 * Author: Daniel Babatunde
 *
 *
 * */
#ifndef FIR_H
#define FIR_H

#include <stdint.h>
#include <ring_buffer.h>

#define RING_BUFFER_SIZE 16

static float ring_buffer_storage[RING_BUFFER_SIZE] = {0};
static buf_handle_t buffer_handle = NULL;


typedef struct {

  buf_handle_t buffer_handle;
  uint8_t buf_Index;
  float output;

}FIRFilter;

void FIRInit(FIRFilter *fir);
float FIRUpdate(FIRFilter* fir, float input);


#endif
