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

#define BUFFER_SIZE 64

//static float ring_buffer_storage[RING_BUFFER_SIZE] = {0};
//static buf_handle_t buffer_handle = NULL;

//RingBufElement buffer[RING_BUFFER_SIZE] = {0};
//RingBuf rb;


//typedef struct {
//
//  RingBuf rb;
//  RingBufCtr buf_Index;
//  float output;
//
//}FIRFilter;

typedef struct{
  float buf[BUFFER_SIZE];
  uint8_t buffer_index;

  float output;
}FIRFilter;

void FIRInit(FIRFilter *fir);
float FIRUpdate(FIRFilter* fir, float input);


#endif
