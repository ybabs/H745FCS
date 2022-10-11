#include <rcfilter.hpp>
#include <helpers.hpp>
#include <micros.hpp>


float RCFilter::update(float input)
{
	uint32_t now = micros();
	double dt = (now - prev_time) / MICROS_PER_SEC;
	prev_time = now;

	// Compute equivalent RC constant from fc.
	float RC = 1.0/ (mathformulas::pi * 2 * cutoff_freq);

	 // Precompute filter coefficients for first order low-pass filter
	float alpha = dt / (dt + RC);
	float beta = RC / (dt + RC);

	float output = alpha * input + beta * prev_output;

	prev_output = output;

	return output;
}

void RCFilter::reset()
{
	prev_output = 0;
}
