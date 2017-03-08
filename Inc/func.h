/**
  * @file  func.h
  * @brief Header file of robot control functions
  */
#ifndef __FUNC_H
#define __FUNC_H

extern uint16_t adcResult[8];

/** Corrects the PWM value depend on motor
  */
#define PWM(VALUE, CORRECTION) ((uint16_t)(VALUE * CORRECTION))
#define COR_L 0.95
#define COR_R 1.00

/** Robot status enumeration
  */
typedef enum
{
  STATUS_NONE,
  STATUS_OK,
  STATUS_ERROR,
  STATUS_POWER_LOW,
  STATUS_POWER_NORMAL,
} StatusTypeDef;

/* Function prototypes */
void ROBOT_Init();
void ROBOT_Forward(uint16_t);
void ROBOT_Backward(uint16_t);
void ROBOT_Right(uint16_t);
void ROBOT_Left(uint16_t);
void ROBOT_Move (int16_t, int16_t);
void ROBOT_Stop();

#endif  // __FUNC_H
