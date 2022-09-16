/*
 * pwm.c
 *
 *  Created on: Nov 6, 2021
 *      Author: Daniel Babatunde
 */


#include "pwm.h"

void SetMotorPWM(Motor *motor)
{
  uint32_t pwm_value;

  if(motor->motor1 >= MAX_MOTOR_PWM)
  {
    pwm_value = MicrosToTicks(MAX_MOTOR_PWM);
    htim1.Instance->CCR1 = pwm_value;
  }
  else if(motor->motor1 <= MIN_MOTOR_PWM)
  {
    pwm_value = MicrosToTicks(MIN_MOTOR_PWM);
    htim1.Instance->CCR1 = pwm_value;
  }
  else
  {
    pwm_value = MicrosToTicks(motor->motor1);
    htim1.Instance->CCR1 = pwm_value;
  }

  if(motor->motor2 >= MAX_MOTOR_PWM)
  {
    pwm_value = MicrosToTicks(MAX_MOTOR_PWM);
    htim1.Instance->CCR2 = pwm_value;
  }
  else if(motor->motor2 <= MIN_MOTOR_PWM)
  {
    pwm_value = MicrosToTicks(MIN_MOTOR_PWM);
    htim1.Instance->CCR2 = pwm_value;
  }
  else
  {
    pwm_value = MicrosToTicks(motor->motor2);
    htim1.Instance->CCR2 = pwm_value;
  }

  if(motor->motor3 >= MAX_MOTOR_PWM)
  {
    pwm_value = MicrosToTicks(MAX_MOTOR_PWM);
    htim1.Instance->CCR3 = pwm_value;
  }
  else if(motor->motor3 <= MIN_MOTOR_PWM)
  {
    pwm_value = MicrosToTicks(MIN_MOTOR_PWM);
    htim1.Instance->CCR3 = pwm_value;
  }
  else
  {
    pwm_value = MicrosToTicks(motor->motor3);
    htim1.Instance->CCR3 = pwm_value;
  }

  if(motor->motor4 >= MAX_MOTOR_PWM)
  {
    pwm_value = MicrosToTicks(MAX_MOTOR_PWM);
    htim1.Instance->CCR4 = pwm_value;
  }
  else if(motor->motor4 <= MIN_MOTOR_PWM)
  {
    pwm_value = MicrosToTicks(MIN_MOTOR_PWM);
    htim1.Instance->CCR4 = pwm_value;
  }
  else
  {
    pwm_value = MicrosToTicks(motor->motor4);
    htim1.Instance->CCR4 = pwm_value;
  }


}
void SetMotorMinPWM(Motor *motor)
{
  uint32_t pwm_value = MicrosToTicks(MIN_MOTOR_PWM);

  motor->motor1 = pwm_value;
  motor->motor2 = pwm_value;
  motor->motor3 = pwm_value;
  motor->motor4 = pwm_value;



}
void SetMotorMaxPWM(Motor *motor)
{
  uint32_t pwm_value = MicrosToTicks(MAX_MOTOR_PWM);

  motor->motor1 = pwm_value;
  motor->motor2 = pwm_value;
  motor->motor3 = pwm_value;
  motor->motor4 = pwm_value;

}
uint32_t MicrosToTicks(uint32_t pulse_width)
{

   uint32_t ticks = pulse_width/ TICKS_PER_US;

   return ticks;

}
