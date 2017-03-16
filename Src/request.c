/**
  * @file  request.c
  * @brief Robot request functions
  */

#include "stdlib.h"
#include "string.h"
#include "usart.h"
#include "request.h"

static RequestTypeDef RequestParse(const char * request)
{
  RequestTypeDef r = (RequestTypeDef){.command=request[0]};
  if (r.command == 'M')
  {
    char * endptr;
    r.args[0] = strtol(request+1, &endptr, 10);
    r.args[1] = strtol(endptr+1,  &endptr, 10);
  }
  return r;
}

static StatusTypeDef RequestReceive(char * request)
{
  StatusTypeDef status = STATUS_OK;
  uint8_t data = 0;
  size_t counter = 0;
  while (data != '\n')
  {
    if (HAL_UART_Receive(&huart1, &data, 1, 10) == HAL_OK)
    {
      request[counter++] = (char)data;
    }
    if (counter >= REQUEST_BUF_SIZE) {
      counter = 0;
      status = STATUS_ERROR;
    }
    request[counter] = 0;
  }
  return status;
}

static StatusTypeDef RequestApply(const char * request, char * response)
{
  StatusTypeDef status = STATUS_OK;
  RequestTypeDef r = RequestParse(request);
  strcpy(response, request);
  switch (r.command)
  {
    case 'M':
      ROBOT_Move(r.args[0], r.args[1]);
      break;
    case 'F':
      ROBOT_Forward(100);
      break;
    case 'B':
      ROBOT_Backward(100);
      break;
    case 'S':
      ROBOT_Stop();
      break;
    case 'L':
      ROBOT_Left(80);
      break;
    case 'R':
      ROBOT_Right(80);
      break;
    case 'A':;
      float battery_V = 11 * adcResult[0] * 3.3 / 4095;
      int partInt  = (int)battery_V;
      int partFrac = (int)((battery_V - partInt) * 10.0);
      snprintf(response, REQUEST_BUF_SIZE, "%d.%d\n", partInt, partFrac);
      break;
    default:
      if ((r.command >= '1') && (r.command <= '6')) // IR1..6
      {
        size_t channel = r.command - '1' + 2;
        snprintf(response, REQUEST_BUF_SIZE, "%d\n", adcResult[channel]);
      }
      else
      {
        snprintf(response, REQUEST_BUF_SIZE, "wrong command\n");
        status = STATUS_ERROR;
      }
  }
  return status;
}

static HAL_StatusTypeDef RequestSend(const char * response)
{
  return HAL_UART_Transmit(&huart1, (uint8_t *)response, strlen(response), 10);
}

StatusTypeDef RequestNext()
{
  char  request[REQUEST_BUF_SIZE];
  char response[REQUEST_BUF_SIZE];
  StatusTypeDef status = STATUS_NONE;

  status = RequestReceive(request);
  if (status == STATUS_OK)
  {
    status = RequestApply(request, response);
    RequestSend(response);
  }
  else
  {
    RequestSend("wrong request\n");
  }
  return status;
}

void RequestStart()
{
  ROBOT_Init();
}
