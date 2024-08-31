// Copyright [2024] <Sebastián Orozco>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "path_creator.h"
#include "plate.h"

double **makeMatrix(uint64_t R, uint64_t C);
double **read_binArchive(char *binName, char *subBin);
void freeMatrix(double **Matrix, uint64_t R);
char *lineToRead(FILE* file, uint64_t line);

void init_plate(plate_t *plate, char *jobFilePath, char *subBin,
                uint64_t line) {
  // we need to declare all attributes to save them
  char *BinaryArchive;
  __int64_t Time;
  float Thermal_diffusivity;
  __int32_t Alture;
  float Sensitivity;
  FILE *file = fopen(jobFilePath, "r");
  // if file is null, don't exist
  if (!file) {
    perror("Can't open the file");
    return;
  }
  char* line_readed = lineToRead(file, line);
  uint32_t index = 0;
  while(line_readed[index] != ' '){
    index++;
  }
  fclose(file);
}

/// @brief this is to read the binary archive and load the values in the matrix
/// @param binName name of the binary archive
/// @param subBin subdirectory of binary archive
/// @return a matrix with values
double **read_binArchive(char *binName, char *subBin) {
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
      // if we have an error, we have to free the memory
      for (uint64_t j = 0; j < i; j++) {
        free(matrix[j]);
      }
      free(matrix);
      perror("Error, value of rows if 0");
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

char* lineToRead(FILE* file, uint64_t line){
  // we need a buffer to save the line readed
  // but before we need to know the size of buffer
  uint64_t current_line = 0;
  size_t lengthLine = 0;
  char cursor = '\0';
  while (current_line != line && (cursor = fgetc(file)) != EOF) {
    if (cursor == '\n') {
      current_line++;
    }
  }
  // we need to obtain the size of buffer
  if (current_line == line) {
    while ((cursor = fgetc(file)) != EOF && cursor != '\n') {
      lengthLine++;
    }
  }
  // we need a one more because the last character must be '\0'
  lengthLine++;
  char *buffer = malloc(lengthLine * sizeof(char));

  rewind(file);
  current_line = 0;
  // if value is 0

  while (current_line != line && (cursor = fgetc(file)) != EOF) {
    if (cursor == '\n') {
      current_line++;
    }
  }
  // to read the specific line
  if (fgets(buffer, lengthLine, file)) {
    printf("%s\n", buffer);
  }
  return buffer;
}
