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
	sum_filter_output(0),
	filtered_output(0)
{

}


CalibrationPosition Calibration::CalibrateNextPosition()
{
	while(1)
	{
		acc_error = sensor.GetAccData();
		// Acquire enough data past the threshold for each axis
		uint32_t num_samples = 0;

		// When nose up
		if(acc_error.imu_acc_x > ACCEL_THRESHOLD && !nose_up_calibrated)
		{
			while(acc_error.imu_acc_x > ACCEL_THRESHOLD)
			{
				num_samples++;
				if(num_samples > SAMPLE_THRESHOLD)
				{
					ledcontrol::LEDDOn();
					return CalibrationPosition::NOSE_UP;
				}

				HAL_Delay(10);
				acc_error = sensor.GetAccData();
			}
		}

		// Nose Down Scenario
		else if(acc_error.imu_acc_x < -ACCEL_THRESHOLD && !nose_down_calibrated)
		{
			while(acc_error.imu_acc_x < -ACCEL_THRESHOLD)
			{
				num_samples++;
				if(num_samples > SAMPLE_THRESHOLD)
				{
					ledcontrol::LEDDOff();
					return CalibrationPosition::NOSE_DOWN;

				}

				HAL_Delay(10);
				acc_error = sensor.GetAccData();
			}
		}
		// Right Side
		else if(acc_error.imu_acc_y  > ACCEL_THRESHOLD && !right_side_calibrated)
		{
			while(acc_error.imu_acc_y > ACCEL_THRESHOLD)
			{
				num_samples++;
				if(num_samples > SAMPLE_THRESHOLD)
				{
					ledcontrol::LEDCOn();
					return CalibrationPosition::RIGHT_SIDE_DOWN;
				}

				HAL_Delay(10);
				acc_error = sensor.GetAccData();
			}

		}

		// Left Side
		else if(acc_error.imu_acc_y  < -ACCEL_THRESHOLD && !left_side_calibrated)
		{
			while(acc_error.imu_acc_y < - ACCEL_THRESHOLD)
			{
				num_samples++;
				if(num_samples > SAMPLE_THRESHOLD)
				{
					ledcontrol::LEDCOff();
					return CalibrationPosition::LEFT_SIDE_DOWN;

				}

				HAL_Delay(10);
				acc_error = sensor.GetAccData();
			}

		}

		// Upside Up
		else if(acc_error.imu_acc_z > ACCEL_THRESHOLD && !upside_up_calibrated)
		{
			while(acc_error.imu_acc_z > ACCEL_THRESHOLD)
			{
				num_samples++;
				{
					if(num_samples > SAMPLE_THRESHOLD)
					{
						ledcontrol::LEDAOn();
						return CalibrationPosition::UPSIDE_UP;
					}
				}

				HAL_Delay(10);
				acc_error = sensor.GetAccData();
			}
		}


		// Upside Down
		else if(acc_error.imu_acc_z < - ACCEL_THRESHOLD && !upside_down_calibrated)
		{
			while(acc_error.imu_acc_z < - ACCEL_THRESHOLD)
			{
				num_samples++;
				{
					if(num_samples > SAMPLE_THRESHOLD)
					{
						ledcontrol::LEDAOff();
						return CalibrationPosition::UPSIDE_DOWN;
					}
				}

				HAL_Delay(10);
				acc_error = sensor.GetAccData();
			}
		}

		HAL_Delay(1000);
	}

}
void Calibration::Calibrate(CalibrationPosition drone_side)
{
	// Wait for a bit and collect data for some time
	auto ms_now = HAL_GetTick();
	auto start_ms = HAL_GetTick();
	uint32_t ms_elapsed = 0;
	uint32_t ms_record = 5000; // 5 seconds

	filter.reset();
	filtered_output = 0;

	uint32_t num_samples = 0;

	while (ms_elapsed < ms_record)
	{
		acc_error = sensor.GetAccData();
		GetPositionDatapoints(drone_side);
		HAL_Delay(100);
		ms_elapsed = HAL_GetTick() - start_ms;
	}

	// Update the Bias for the given side
	UpdateMeasuredGValue(drone_side, num_samples);


}
void Calibration::GetPositionDatapoints(CalibrationPosition drone_side)
{
	float value = 0;
	switch(drone_side)
	{
		case CalibrationPosition::UPSIDE_UP:
			value = acc_error.imu_acc_z;
			break;
		case CalibrationPosition::UPSIDE_DOWN:
			value = acc_error.imu_acc_z;
			break;
		case CalibrationPosition::LEFT_SIDE_DOWN:
			value = acc_error.imu_acc_y;
			break;
		case CalibrationPosition::RIGHT_SIDE_DOWN:
			value = acc_error.imu_acc_y;
			break;
		case CalibrationPosition::NOSE_UP:
			value = acc_error.imu_acc_x;
			break;
		case CalibrationPosition::NOSE_DOWN:
			value = acc_error.imu_acc_x;
			break;
		case CalibrationPosition::INVALID:
			//TODO Handle this case
			break;
		default:
			// LOGGER ("Unknwonw side);
			// TODO Catch this exception somehow;
			break;
	}

	filtered_output = filter.update(value);
	sum_filter_output += filtered_output;

}
void Calibration::UpdateMeasuredGValue(CalibrationPosition drone_side, int num_samples)
{
	double avg_filter_output = sum_filter_output / num_samples;

	switch(drone_side)
	{
		case CalibrationPosition::UPSIDE_UP:
			upside_up_g = filtered_output;
			upside_up_calibrated = true;
			break;
		case CalibrationPosition::UPSIDE_DOWN:
			upside_down_g = filtered_output;
			upside_down_calibrated = true;
			break;
		case CalibrationPosition::LEFT_SIDE_DOWN:
			left_side_g = filtered_output;
			left_side_calibrated = true;
			break;
		case CalibrationPosition::RIGHT_SIDE_DOWN:
			right_side_g = filtered_output;
			right_side_calibrated = true;
			break;
		case CalibrationPosition::NOSE_UP:
			nose_up_g = filtered_output;
			nose_up_calibrated = true;
			break;
		case CalibrationPosition::NOSE_DOWN:
			nose_down_g = filtered_output;
			nose_down_calibrated = true;
			break;
		case CalibrationPosition::INVALID:
			//TODO Handle this case
			break;
		default:
			// LOGGER ("Unknwonw side);
			// TODO Catch this exception somehow;
			break;
	}

}
void Calibration::ComputeOffsets()
{
	accel_offset.x_offset =  (nose_up_g + nose_down_g) / 2;
	accel_offset.y_offset =  (left_side_g + right_side_g)/2;
	accel_offset.z_offset =  (upside_up_g + upside_down_g)/2;

	// TODO Compute scaling factor

}
bool Calibration::CalibrationComplete()
{
	return upside_down_calibrated && upside_up_calibrated &&
		   left_side_calibrated && right_side_calibrated &&
		   nose_up_calibrated && nose_down_calibrated;
}

void Calibration::CalibrateGyro()
{
	// Wait for a bit and collect data for some time
	auto ms_now = HAL_GetTick();
	auto start_ms = HAL_GetTick();
	uint32_t ms_elapsed = 0;
	uint32_t ms_record = 5000; // 5 seconds


	uint32_t num_samples = 0;

	while (ms_elapsed < ms_record)
	{
		gyro_error = sensor.GetGyroData();
		accumulate_gyro_x += gyro_error.imu_gyro_x;
		accumulate_gyro_y += gyro_error.imu_gyro_y;
		accumulate_gyro_z += gyro_error.imu_gyro_z;

		num_samples++;

		HAL_Delay(100);
		ms_elapsed = HAL_GetTick() - start_ms;
	}

	gyro_offset.x_offset = accumulate_gyro_x / num_samples;
	gyro_offset.y_offset = accumulate_gyro_y / num_samples;
	gyro_offset.z_offset = accumulate_gyro_z / num_samples;

}
