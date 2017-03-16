/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "gpio.h"
#include "request.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId IndicatorTaskHandle;
osThreadId UARTTaskHandle;
osThreadId StatusTaskHandle;
osMessageQId StatusHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartIndicatorTask(void const * argument);
void StartUARTTask(void const * argument);
void StartStatusTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of IndicatorTask */
  osThreadDef(IndicatorTask, StartIndicatorTask, osPriorityNormal, 0, 128);
  IndicatorTaskHandle = osThreadCreate(osThread(IndicatorTask), NULL);

  /* definition and creation of UARTTask */
  osThreadDef(UARTTask, StartUARTTask, osPriorityIdle, 0, 128);
  UARTTaskHandle = osThreadCreate(osThread(UARTTask), NULL);

  /* definition and creation of StatusTask */
  osThreadDef(StatusTask, StartStatusTask, osPriorityIdle, 0, 128);
  StatusTaskHandle = osThreadCreate(osThread(StatusTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of Status */
  osMessageQDef(Status, 16, uint16_t);
  StatusHandle = osMessageCreate(osMessageQ(Status), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartIndicatorTask function */
void StartIndicatorTask(void const * argument)
{

  /* USER CODE BEGIN StartIndicatorTask */
  StatusTypeDef status;
  uint16_t timeOn = 100;
  uint16_t timeOff = 1000;

  /* Infinite loop */
  for(;;)
  {
    /* Message receive */
    if (xQueueReceive(StatusHandle, &status, 0))
    {
      if (status == STATUS_OK)
      {
        timeOn = 100;
        timeOff = 1000;
      }
      if (status == STATUS_ERROR)
      {
        timeOn = 100;
        timeOff = 100;
      }
      if (status == STATUS_POWER_LOW)
      {
        HAL_GPIO_WritePin(Battery_GPIO_Port, Battery_Pin, GPIO_PIN_SET);
      }
      if (status == STATUS_POWER_NORMAL)
      {
        HAL_GPIO_WritePin(Battery_GPIO_Port, Battery_Pin, GPIO_PIN_RESET);
      }
    }

    /* Blink led indicator */
    HAL_GPIO_WritePin(Indicator_GPIO_Port, Indicator_Pin, GPIO_PIN_RESET);
    osDelay(timeOn);
    HAL_GPIO_WritePin(Indicator_GPIO_Port, Indicator_Pin, GPIO_PIN_SET);
    osDelay(timeOff);

  }
  /* USER CODE END StartIndicatorTask */
}

/* StartUARTTask function */
void StartUARTTask(void const * argument)
{
  /* USER CODE BEGIN StartUARTTask */
  StatusTypeDef status;
  StatusTypeDef status_last = STATUS_NONE;

  RequestStart();

  /* Infinite loop */
  for(;;)
  {
    status = RequestNext();

    /* check status */
    if (status_last != status)
    {
      status_last = status;
      xQueueSend(StatusHandle, &status, 0);
    }
  }
  /* USER CODE END StartUARTTask */
}

/* StartStatusTask function */
void StartStatusTask(void const * argument)
{
  /* USER CODE BEGIN StartStatusTask */
  StatusTypeDef status;
  StatusTypeDef status_last = STATUS_NONE;

  /* Infinite loop */
  for(;;)
  {
    float battery_V = GetBatteryVoltage();
    if (battery_V < 10.0)
    {
      status = STATUS_POWER_LOW;
    }
    else
    {
      status = STATUS_POWER_NORMAL;
    }

    /* check status */
    if (status_last != status)
    {
      status_last = status;
      xQueueSend(StatusHandle, &status, 0);
    }
    osDelay(1000);
  }
  /* USER CODE END StartStatusTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
