// Copyright [2024] <Sebastián Orozco>

#include "simulation.h"
#include "plate.h"
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

char* format_time(const time_t seconds, char* text, const size_t capacity);
uint64_t transfer(double **matrix1, double **matrix2, double formula,
                  uint64_t R, uint64_t C, double point);
char *format_line(char* line);

void make_report(char* lineReport, time_t time, char* output_path, uint64_t states);

uint8_t init_simulation(plate_t plate, char* output_path) {
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
  if(!states){
    return 0;
  } 
  int64_t totalTime = states * plate.time;
  time_t time_seconds = (time_t)totalTime;
  make_report(plate.lineReaded, time_seconds, output_path, states);
  return 1;
}

uint64_t transfer(double **matrix1, double **matrix2, double formula,
                  uint64_t R, uint64_t C, double point) {
  if(!matrix1 && !matrix2){
    return 0;
  }
  uint64_t states = 0;
  bool limit = true;
  printf("entró2\n");
  while (limit) {
    limit = false;
    for (uint64_t i = 1; i < R - 1; i++) {
      for (uint64_t j = 1; j < C - 1; j++) {
        matrix1[i][j] =
            matrix2[i][j] + formula * (matrix2[i - 1][j] + matrix2[i][j + 1] +
                                       matrix2[i + 1][j] + matrix2[i][j - 1] -
                                       4 * (matrix2[i][j]));
        if ((fabs(matrix1[i][j] - matrix2[i][j]) > point )) {
          limit = true;
        }
      }
    }
    for (uint64_t i = 0; i < R-1; i++) {
      for (uint64_t j = 0; j < C-1; j++) {
        matrix2[i][j] = matrix1[i][j];
      }
    }
    states++;
  }
  printf("salió\n");
  return states;
}

void make_report(char* lineReport, time_t time, char* output_path, uint64_t states){
  FILE* file = fopen(output_path, "a+");
  if(!file){
    perror("Error: can't find or create this file");
    return;
  }
  fputs(format_line(lineReport), file);
  fputc('\t', file);
   fputc('\t', file);
  char int_to_char[21];
  snprintf(int_to_char, 21, "%"SCNu64, (uint64_t)states);
  fputs(int_to_char, file);
  fputc('\t', file);
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

char *format_line(char* line){
  for(uint32_t i = 0; line[i] != '\0'; i++){
    if(line[i] == ' ' && line[i+1] != ' '){
      line[i] = '\t';
    }
  }
  return line;
} 