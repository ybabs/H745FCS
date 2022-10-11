#pragma once

#include <helpers.hpp>
#include <sensors.hpp>
#include <rcfilter.hpp>

static constexpr float ACCEL_THRESHOLD = 0.9f; // in gs

static constexpr int16_t ACCEL_BANDWIDTH = 408;
static constexpr int16_t SAMPLE_THRESHOLD = 200;


enum class CalibrationPosition
{
	UPSIDE_UP = 0,
	UPSIDE_DOWN ,
	LEFT_SIDE_DOWN,
	RIGHT_SIDE_DOWN,
	NOSE_UP,
	NOSE_DOWN,
	INVALID = 255

};

class Calibration
{
private:

	SensorData sensor;

	// Acceleration Data
	acc_data acc_error;

	gyro_data gyro_error;

	// Acceleration flag
	bool upside_up_calibrated;
	bool upside_down_calibrated;

	bool left_side_calibrated;
	bool right_side_calibrated;

	bool nose_up_calibrated;
	bool nose_down_calibrated;

	// Gravity measured in each axis
	float upside_up_g;
	float upside_down_g;
	float left_side_g;
	float right_side_g;
	float nose_up_g;
	float nose_down_g;



	RCFilter filter{ACCEL_BANDWIDTH};

	double filtered_output;
	double sum_filter_output; //accumulated data before average


	double accumulate_gyro_x;
	double accumulate_gyro_y;
	double accumulate_gyro_z;

public:

	explicit Calibration();
	CalibrationPosition CalibrateNextPosition();
	void Calibrate(CalibrationPosition drone_side);
	void GetPositionDatapoints(CalibrationPosition drone_side);
	void UpdateMeasuredGValue(CalibrationPosition drone_side, int num_samples);
	void ComputeOffsets();
	bool CalibrationComplete();
	void CalibrateGyro();

	struct sensor_offset
	{
		float x_offset = 0;
		float y_offset = 0;
		float z_offset = 0;
	};

	sensor_offset accel_offset;
	sensor_offset gyro_offset;

};
