// Copyright [2024] <Sebastián Orozco>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "path_creator.h"
#include "plate.h"

double **makeMatrix(uint64_t R, uint64_t C);
double **binaryArchive();
void freeMatrix(double **Matrix, uint64_t R);

void init_plate(plate_t *plate, char *jobFilePath, char *subBin) {
  // we need to declare all attributes to save them
  char *BinaryArchive;
  __int64_t Time;
  float Thermal_diffusivity;
  __int32_t Alture;
  float Sensitivity;
  FILE *file = fopen(jobFilePath, "r");
  // if file is null, don't exist
  if (file == NULL) {
    perror("Can't open the file");
    return;
  }
  char ch;
  size_t length = 0;
  // to calculate the size of the name file binary
  while ((ch = fgetc(file)) != ' ' && ch != EOF) {
    length++;
  }
  BinaryArchive = malloc(sizeof(char) * length + 1);
  if (ch == EOF) {
    perror("Can't read the name of this file");
    fclose(file);
    return;
  }
  // to read the file from the beginning
  rewind(file);
  if (fscanf(file, "%s %li %f %i %f", BinaryArchive, &Time,
             &Thermal_diffusivity, &Alture, &Sensitivity) == 5) {
    plate->binaryArchive = BinaryArchive;
    plate->time = Time;
    plate->thermal_diffusivity = Thermal_diffusivity;
    plate->alture = Alture;
    plate->sensitivity = Sensitivity;
    plate->plateM = binaryArchive(BinaryArchive, subBin);
  }

  free(BinaryArchive);
  fclose(file);
}

/// @brief this is to read the binary archive and load the values in the matrix
/// @param binName name of the binary archive
/// @param subBin subdirectory of binary archive
/// @return a matrix with values
double **binaryArchive(char *binName, char *subBin) {
  // to make binary archive path
  char *pathBin = make_path(subBin, binName);
  FILE *file = fopen(pathBin, "rb");
  // if file is null, don't exist
  if (!file) {
    perror("Can't open the bin file");
    return NULL;
  }
  uint64_t R = 0;
  uint64_t C = 0;
  // to read first 8 bytes, R = Rows
  if (fread(&R, sizeof(uint64_t), 1, file) != 1) {
    perror("Error, can't read the rows");
    fclose(file);
    return NULL;
  }
  // to read followin 8 bytes, C = columns
  if (fread(&C, sizeof(uint64_t), 1, file) != 1) {
    perror("Error: can't read the columns");
    fclose(file);
    return NULL;
  }

  double **matrix = makeMatrix(R, C);
  for (uint64_t i = 0; i < R; i++) {
    for (uint64_t j = 0; j < C; j++) {
      if (fread(&matrix[i][j], sizeof(double), 1, file) != 1) {
        perror("Error: can't read the matrix values");
        // to free the memory if somthing fail
        for (uint64_t k = 0; k < R; k++) {
          free(matrix[k]);
        }
        free(matrix);
        fclose(file);
        return NULL;
      }
    }
  }
  // Imprimir la matriz
  for (uint64_t i = 0; i < R; i++) {
    for (uint64_t j = 0; j < C; j++) {
      printf("%f ", matrix[i][j]);
    }
    printf("\n");
  }
  freeMatrix(matrix, R);
  free(pathBin);
  fclose(file);
  return matrix;
}

/// @brief to make matrix with information of the archive binary
/// @param R nums of rows
/// @param C nums of columns
/// @return return a matrix with trash, because his values don't inicialite yet
double **makeMatrix(uint64_t R, uint64_t C) {
  // creation of rows
  double **matrix = malloc(R * sizeof(uint64_t));
  if (!matrix) {
    perror("Error, value of rows if 0");
    return NULL;
  }
  // creation of columns
  for (uint64_t i = 0; i < R; i++) {
    matrix[i] = malloc(C * sizeof(uint64_t));
    if (!matrix[i]) {
      perror("Error, value of rows if 0");
      // if we have an error, we have to free the memory
      for (uint64_t j = 0; j < i; j++) {
        free(matrix[j]);
      }
      free(matrix);
      return NULL;
    }
  }
  return matrix;
}

/// @brief to liberate memory from a matrix of dinamic memory
/// @param Matrix matrix of doubles
/// @param R rows
void freeMatrix(double **Matrix, uint64_t R) {
  for (uint64_t i = 0; i < R; i++) {
    free(Matrix[i]);
  }
  free(Matrix);
}
