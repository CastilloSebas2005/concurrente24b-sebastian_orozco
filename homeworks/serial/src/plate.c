// Copyright [2024] <Sebastián Orozco>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "path_creator.h"
#include "plate.h"

double **makeMatrix(uint64_t R, uint64_t C);
double **read_binArchive(plate_t *plate, char *binName, char *subBin);
double **copy_matrix(uint64_t R, uint64_t C, double **matrixOriginal);
void freeMatrix(double **Matrix, uint64_t R);
char *lineToRead(FILE *file, uint64_t line);
uint8_t init_plate(plate_t *plate, char *jobFilePath, char *subBin,
                   uint64_t line) {
  // we need to declare all attributes to save them
  __int64_t Time;
  double Thermal_diffusivity;
  __int32_t Alture;
  double Sensitivity;
  FILE *file = fopen(jobFilePath, "r");
  // if file is null, don't exist
  if (!file) {
    fprintf(stderr, "Error: can't open the file\n");
    fclose(file);
    return 0;
  }
  char *line_readed = lineToRead(file, line);
  if (!line_readed) {
    fclose(file);
    return 0;
  }
  size_t index = 0;
  while (line_readed[index] != ' ') {
    index++;
  }
  // we need one more becase the '\0' character
  index++;
  printf("%li index\n", index);
  // we need to obtain de size of bin file
  char *BinaryFile = malloc(sizeof(char) * index);

  // read the line that contains the necesary information to the math part
  if (sscanf(line_readed, "%s %" SCNd64 " %lf %" SCNd32 " %lf", BinaryFile,
             &Time, &Thermal_diffusivity, &Alture, &Sensitivity) == 5) {
    plate->plateM1 = read_binArchive(plate, BinaryFile, subBin);
    plate->plateM2 = copy_matrix(plate->rows, plate->columns, plate->plateM1);
    plate->time = Time;
    plate->thermal_diffusivity = Thermal_diffusivity;
    plate->alture = Alture;
    plate->sensitivity = Sensitivity;
    plate->lineReaded = line_readed;
  } else {
    fprintf(stderr, "Error: the values of jobFile are incorrect\n");
    return 0;
  }
  free(BinaryFile);
  fclose(file);
  return 1;
}

/// @brief this is to read the binary archive and load the values in the matrix
/// @param plate struct plate
/// @param binName name of the binary archive
/// @param subBin subdirectory of binary archive
/// @return a matrix with values
double **read_binArchive(plate_t *plate, char *binName, char *subBin) {
  // to make binary archive path
  char *pathBin = make_path(subBin, binName);
  FILE *file = fopen(pathBin, "rb");
  // if file is null, don't exist
  if (!file) {
    free(pathBin);
    fprintf(stderr,"Error: can't open the bin file\n");
    return NULL;
  }
  uint64_t R = 0;
  uint64_t C = 0;
  // to read first 8 bytes, R = Rows
  if (fread(&R, sizeof(uint64_t), 1, file) != 1) {
    free(pathBin);
    fclose(file);
    fprintf(stderr,"Error: can't read the rows\n");
    return NULL;
  }
  // to read followin 8 bytes, C = columns
  if (fread(&C, sizeof(uint64_t), 1, file) != 1) {
    free(pathBin);
    fclose(file);
    fprintf(stderr,"Error: can't read the columns\n");
    return NULL;
  }
  plate->rows = R;
  plate->columns = C;
  double **matrix = makeMatrix(R, C);
  if (!matrix) {
    fprintf(stderr,"Error: can't make matrix\n");
    return NULL;
  }
  // to situate all valors in the matrix
  for (uint64_t i = 0; i < R; i++) {
    for (uint64_t j = 0; j < C; j++) {
      if (fread(&matrix[i][j], sizeof(double), 1, file) != 1) {
        // to free the memory if somthing fail
        for (uint64_t k = 0; k < R; k++) {
          free(matrix[k]);
        }
        free(pathBin);
        free(matrix);
        fclose(file);
        fprintf(stderr,"Error: can't read the matrix values\n");
        return NULL;
      }
    }
  }
  // to print matrix
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
  double **matrix = malloc(R * sizeof(double));
  if (!matrix) {
    fprintf(stderr,"Error: don't exist value of rows\n");
    return NULL;
  }
  // creation of columns
  for (uint64_t i = 0; i < R; i++) {
    matrix[i] = malloc(C * sizeof(double));
    if (!matrix[i]) {
      // if we have an error, we have to free the memory
      for (uint64_t j = 0; j < i; j++) {
        free(matrix[j]);
      }
      free(matrix);
      fprintf(stderr,"Error: don't exist value of columns\n");
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

/// @brief This subrutine is to get the line to read and the information
/// @param file job file
/// @param line number of line to read
/// @return the information of that line
char *lineToRead(FILE *file, uint64_t line) {
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
  if (lengthLine == 0) {
    return NULL;
  }
  // we need a one more because the last character must be '\0'
  lengthLine++;
  char *buffer = malloc(lengthLine * sizeof(char));
  if (!buffer) {
    fprintf(stderr,"Error: can't make the buffer\n");
    return NULL;
  }
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
  if (buffer[0] == '\n' || buffer[0] == '\0') {
    free(buffer);
    return NULL;
  }
  return buffer;
}

/// @brief This subrutine is to copy values of the original matrix in other
/// @param R Rows
/// @param C Cols
/// @param matrixOriginal Matrix created by Binary archive
/// @return a copy of original matrix
double **copy_matrix(uint64_t R, uint64_t C, double **matrixOriginal) {
  double **matrixCopy = makeMatrix(R, C);
  for (uint64_t i = 0; i < R; i++) {
    for (uint64_t j = 0; j < C; j++) {
      matrixCopy[i][j] = matrixOriginal[i][j];
    }
  }
  return matrixCopy;
}

uint8_t destruct_plate(plate_t *plate) {
  freeMatrix(plate->plateM1, plate->rows);
  freeMatrix(plate->plateM2, plate->rows);
  free(plate->lineReaded);
  return 0;
}
