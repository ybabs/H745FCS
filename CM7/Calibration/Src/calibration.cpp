/*
 * calibration.cpp
 *
 *  Created on: 16 Sep 2022
 *      Author: Home
 */

#include <calibration.hpp>


Calibration::Calibration():
	upside_up_calibrated(false),
	upside_down_calibrated(false),
	left_side_calibrated(false),
	right_side_calibrated(false),
	nose_up_calibrated(false),
	nose_down_calibrated(false),
	upside_up_g(0),
	upside_down_g(0),
	left_side_g(0),
	right_side_g(0),
	nose_up_g(0),
	nose_down_g(0),
	filtered_output(0)
{

}


CalibrationPosition Calibration::CalibrateNextPosition()
{

}
void Calibration::Calibrate(CalibrationPosition drone_side)
{

}
void Calibration::GetPositionDatapoints(CalibrationPosition drone_side)
{

}
void Calibration::UpdateMeasuredGValue(CalibrationPosition drone_side, int num_samples)
{

}
void Calibration::ComputeOffsets()
{

}
bool Calibration::CalibrationComplete()
{
	return upside_down_calibrated && upside_up_calibrated &&
		   left_side_calibrated && right_side_calibrated &&
		   nose_up_calibrated && nose_down_calibrated;
}
