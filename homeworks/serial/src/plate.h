// Copyright [2024] <Sebastián Orozco>

#ifndef PLATE_H
#define PLATE_H

#include <stdlib.h>

/**
 @class plate
 @brief This class is going to work with the part of math and transmition, this
 class must read the archives and generate output file
 @details his atributtes are things that are needed to make calculus part
*/

typedef struct {
  /* data */
  // this is the rectangle matrix
  double* plate;
  //name of the binary archive
  char* binaryArchive;
  // time in seconds
  __int64_t time;
  // this is the alpha constant
  float thermal_diffusivity;
  // this is the h in the formula
  __int32_t alture;
  // this is the epsilon constant
  float sensitivity;
} plate_t;

void init_plate(plate_t *plate_object, char* jobFilePath);
 
#endif