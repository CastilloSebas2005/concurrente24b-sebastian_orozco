// Copyright [2024] <Sebastián Orozco>

#include "simulation.h"
#include "plate.h"
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char* format_time(const time_t seconds, char* text, const size_t capacity);
uint64_t transfer(double **matrix1, double **matrix2, double formula,
                  uint64_t R, uint64_t C, double point);

void make_report(char* lineReport, time_t time, char* output_path, uint64_t states);

void init_simulation(plate_t plate, char* output_path) {
  printf("entró\n");
  // this is the unique formula that have to aply
  double formula =
      (plate.time * plate.thermal_diffusivity) / (plate.alture * plate.alture);
  double **plate_matrix1 = plate.plateM1;
  double **plate_matrix2 = plate.plateM2;
  uint64_t R = plate.rows;
  uint64_t C = plate.colums;
  double point = plate.sensitivity;
  uint64_t states =
      transfer(plate_matrix1, plate_matrix2, formula, R, C, point);
  int64_t totalTime = states * plate.time;
  time_t time_seconds = (time_t)totalTime;
  make_report(plate.lineReaded, time_seconds, output_path, states);
  
}

uint64_t transfer(double **matrix1, double **matrix2, double formula,
                  uint64_t R, uint64_t C, double point) {
  uint64_t states = 1;
  uint64_t limit = 9223372036854775807;
  printf("entró2\n");
  while (states != limit) {
    for (uint64_t i = 1; i < R - 1; i++) {
      for (uint64_t j = 1; j < C - 1; j++) {
        matrix1[i][j] =
            matrix2[i][j] + formula * (matrix2[i - 1][j] + matrix2[i][j + 1] +
                                       matrix2[i + 1][j] + matrix2[i][j - 1] -
                                       4 * (matrix2[i][j]));
        if ((fabs(matrix1[i][j] - matrix2[i][j]) < point ) && states != 1) {
          printf("k = %li\n", states);
          for (uint64_t i = 0; i < R; i++) {
            for (uint64_t j = 0; j < C; j++) {
              printf("%f ", matrix1[i][j]);
            }
            printf("\n");
          }
          printf("%f - %f < %f\n", matrix1[i][j], matrix2[i][j], point);
          return states;
        }
        matrix2[i][j] = matrix1[i][j];
      }
    }
    states++;
  }
  printf("salió\n");
  return 22;
}

void make_report(char* lineReport, time_t time, char* output_path, uint64_t states){
  FILE* file = fopen(output_path, "w+");
  if(!file){
    perror("Can't find or create this file");
    return;
  }
  fputs(lineReport, file);
  fputc('\t', file);
  char int_to_char[21];
  snprintf(int_to_char, 21, "%"SCNu64, (uint64_t)states);
  fputs(int_to_char, file);
  fputc('\t', file);
  char timeFormated[48];
  timeFormated[48] = *format_time(time, timeFormated, sizeof(timeFormated));
  fputs(timeFormated, file); 
  fputc('\n', file);
  fclose(file);
}

// Return parameter text must have at least 48 chars (YYYY/MM/DD hh:mm:ss)
char* format_time(const time_t seconds, char* text, const size_t capacity) {
  const struct tm* gmt = gmtime(&seconds);
  snprintf(text, capacity, "%04d/%02d/%02d\t%02d:%02d:%02d", gmt->tm_year - 70, gmt->tm_mon, gmt->tm_mday - 1, gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
  return text;
}