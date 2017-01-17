/**
  * @file  func.h
  * @brief Header file of robot control functions
  */
#ifndef __FUNC_H
#define __FUNC_H

/**
  * Corrects the PWM value depend on motor
  */
#define PWM(VALUE, CORRECTION) ((uint16_t)(VALUE * CORRECTION))
#define COR_L 1.00
#define COR_R 0.90

/* Function prototypes */
void ROBOT_Init();
void ROBOT_Forward(uint16_t);
void ROBOT_Backward(uint16_t);
void ROBOT_Right(uint16_t);
void ROBOT_Left(uint16_t);
void ROBOT_Stop();
typedef enum {
  STATUS_OK,
  STATUS_ERROR,
} StatusTypeDef;

#endif  // __FUNC_H
