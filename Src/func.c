/**
  * @file  func.c
  * @brief Robot control functions
  */

#include "gpio.h"
#include "func.h"
#include "tim.h"

uint16_t adcResult[8];

/** @defgroup ROBOT_CONTROL_FUNCTIONS Robot Control Functions
  * @{
  */

/**
  * Starts the PWM signal generation for all control channels
  */
void ROBOT_Init()
{
  // Right motor
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  // Left motor
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

/**
  * Set forward velocity
  */
void ROBOT_Forward(uint16_t vel)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  // Right motor
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, PWM(vel, COR_R));
  // Left motor
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, PWM(vel, COR_L));
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
}

/**
  * Set backward velocity
  */
void ROBOT_Backward(uint16_t vel)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  // Right motor
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, PWM(vel, COR_R));
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
  // Left motor
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, PWM(vel, COR_L));
}

/**
  * Set left rotation velocity
  */
void ROBOT_Left(uint16_t vel)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  // Right motor
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, PWM(vel, COR_R));
  // Left motor
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, PWM(vel, COR_L));
}

/**
  * Set right rotation velocity
  */
void ROBOT_Right(uint16_t vel)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  // Right motor
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, PWM(vel, COR_R));
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
  // Left motor
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, PWM(vel, COR_L));
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
}

/**
  * Stop for all motor
  */
void ROBOT_Stop()
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  // Right motor
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
  // Left motor
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
}

/**
  * @}
  */
