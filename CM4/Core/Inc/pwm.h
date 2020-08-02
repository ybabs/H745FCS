#ifndef PWM_H
#define PWM_H

#include "tim.h"

/**
 * Motor Configuration
 * 0   1
 *  \/
 *  /\
 * 2  3
 *
 * MOTOR_0 -- TIM1_CH1
 * MOTOR_1 -- TIM1_CH2
 * MOTOR_2 -- TIM1_CH3
 * MOTOR_3 -- TIM1_CH4
 */

// Max Update Rate of 400Hz (2.5 ms) from Timer 1.
// Number of ticks = 11000
// microseconds per tick = 2500 us / 11000
#define TICKS_PER_US 0.22727272727f
#define MAX_MOTOR_PWM 1860.0f  // Afro ESC Configuration
#define MIN_MOTOR_PWM 1060.0f  //

typedef struct
{
	float motor1, motor2, motor3, motor4;
}Motor;

void SetMotorPWM(Motor *motor_pwm);
void SetMotorMinPWM(Motor *motor_pwm);
void SetMotorMaxPWM(Motor *motor_pwm);
uint32_t MicrosToTicks(uint32_t pulse_width);



#endif
