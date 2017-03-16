/**
  * @file  request.h
  * @brief Header file of robot request functions
  */

#ifndef __REQUEST_H
#define __REQUEST_H

#include "func.h"

#define REQUEST_BUF_SIZE 16

typedef struct
{
  char command;
  int args[2];
} RequestTypeDef;

StatusTypeDef RequestNext();
void RequestStart();

#endif /* __REQUEST_H */
