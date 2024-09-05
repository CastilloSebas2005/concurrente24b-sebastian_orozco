// Copyright [2024] <Sebastián Orozco>

#ifndef PLATE_H
#define PLATE_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

/**
 @class plate
 @brief This class is going to save the necessary things to do the calculus
 part, this class must read the archives and generate output file
 @details his atributtes are things that are needed to make calculus part
*/

/// @struct plate
typedef struct plate {
  // this is the rectangle matrix
  double **plateM1;
  double **plateM2;
  // time in seconds
  __int64_t time;
  // this is the alpha constant
  double thermal_diffusivity;
  // this is the h in the formula
  __int32_t alture;
  // this is the epsilon constant
  double sensitivity;
  // We need the count of rows and columns of the matrix
  uint64_t rows;
  uint64_t colums;
  char* lineReaded;
} plate_t;

/// @brief this is to inicializate plate
/// @param plate struct
/// @param jobFilePath path of file that contains job.txt
/// @param subBin subdirectory where binary path is saved
/// @param line this is the number of line that must read.
/// @return return 0 if occurs an error and 1 if all is correct
uint8_t init_plate(plate_t *plate, char *jobFilePath, char* subBin, uint64_t line);

/// @brief This is to liberate the memory
/// @param plate struct plate
/// @return 0 if liberate correctly the memory 1 if something bad
__uint8_t destruct_plate(plate_t *plate);
#endif
