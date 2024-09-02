// Copyright [2024] <Sebastián Orozco>

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "manager_argument.h"
#include "plate.h"
#include <stdio.h>

/**
 @class controller
 @brief This class is going to controlate the calls other classes
 @details In this moment his implementation are null.
*/

typedef struct controller {
  /* data */
  manager_argument_t controllerArgument;
  plate_t plate;

}controller_t;

void init_controller(char *argv[]);

#endif
