/*
 * rc.c
 *
 *  Created on: 16 Nov 2021
 *      Author: Daniel Babatunde
 */


#include "rc.h"

void RCInit(RCFilter *filt, float fc, float dt)
{
  // Compute equivalent RC constant from fc.
  float RC = 1.0f / (6.28318530718f  * fc);

  // Precompute filter coefficients for first order low-pass filter
  filt->coeff[0] =  dt / (dt + RC);
  filt->coeff[1] = RC / (dt + RC);

  filt->output[0] = 0.0f;
  filt->output[1] = 0.0f;
}
float RCUpdate(RCFilter *filt, float input)
{

  // SHift output samples  // set current output sample to prev sample
  filt->output[1] = filt->output[0];

  // Compute new output sample

filt->output[0] = filt->coeff[0] * input + filt->coeff[1] * filt->output[1];
  // Return filtered sample

return filt->output[0];

}
