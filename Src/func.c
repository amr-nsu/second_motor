/**
  * @file  func.c
  * @brief Robot control functions
  */

#include "gpio.h"
#include "func.h"
#include "tim.h"

uint16_t adcResult[8];

/** @defgroup MECHANICS_CONTROL_FUNCTIONS Mechanics Control Functions
  * @{
  */

/**
 * @brief Set velociry of left motor
 * @param velocity
 */
static void MotorLeftSetVelocity(int velocity)
{
  if (velocity > 0)
  {
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, PWM(velocity, COR_L));
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
  }
  else
  {
    velocity = -velocity;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, PWM(velocity, COR_L));
  }
}

/**
 * @brief Set velociry of right motor
 * @param velocity
 */
static void MotorRightSetVelocity(int velocity)
{
  if (velocity > 0)
  {
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, PWM(velocity, COR_R));
  }
  else
  {
    velocity = -velocity;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, PWM(velocity, COR_R));
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
  }
}

/**
 * @brief Starts the PWM signal generation for left motor control channels
 */
static void MotorLeftStartPWM(void)
{
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

/**
 * @brief Starts the PWM signal generation for right motor control channels
 */
static void MotorRightStartPWM(void)
{
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}

/** @brief Get voltage from battery using 1/11 voltage divider befor adc
  */
float GetBatteryVoltage(void)
{
  return 11 * adcResult[0] * 3.3 / 4095;
}

/**
  * @}
  */

/** @defgroup ROBOT_CONTROL_FUNCTIONS Robot Control Functions
  * @{
  */

/** Init robot control
  */
void ROBOT_Init(void)
{
  MotorLeftStartPWM();
  MotorRightStartPWM();
}

/** Set wheels velocity
  */
void ROBOT_Move(int vel1, int vel2)
{
  MotorLeftSetVelocity(vel1);
  MotorRightSetVelocity(vel2);
}

/** Set forward velocity
  */
void ROBOT_Forward(uint16_t vel)
{
  MotorLeftSetVelocity(vel);
  MotorRightSetVelocity(vel);
}

/** Set backward velocity
  */
void ROBOT_Backward(uint16_t vel)
{
  MotorLeftSetVelocity(-vel);
  MotorRightSetVelocity(-vel);
}

/** Set left rotation velocity
  */
void ROBOT_Left(uint16_t vel)
{
  MotorLeftSetVelocity(-vel);
  MotorRightSetVelocity(vel);
}

/** Set right rotation velocity
  */
void ROBOT_Right(uint16_t vel)
{
  MotorLeftSetVelocity(vel);
  MotorRightSetVelocity(-vel);
}

/** Stop for all motor
  */
void ROBOT_Stop(void)
{
  MotorLeftSetVelocity(0);
  MotorRightSetVelocity(0);
}

/**
  * @}
  */
