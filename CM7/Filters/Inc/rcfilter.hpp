#pragma once


class  RCFilter
{
public:
	RCFilter(float fc): cutoff_freq(fc), prev_time(0), prev_output(0)
	{

	}

	float update(float input);
	void reset();

private:
	   void Tick();

	    float cutoff_freq = 0;
	    float prev_time;
	    float prev_output;




};
