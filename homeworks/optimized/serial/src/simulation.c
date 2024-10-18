// Copyright [2024] <Sebastián Orozco>

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "plate.h"
#include "simulation.h"

char *format_time(const time_t seconds, char *text, const size_t capacity);

/// @brief This is the part of math of the simulation
/// @param matrix1 Matrix 1
/// @param matrix2 Matrix 2
/// @param formula formula to apply
/// @param R Rows
/// @param C Columns
/// @param point Thermal sensitivity
/// @return the number of states
uint64_t transfer(double *matrix1, double *matrix2, double formula, uint64_t R,
                   uint64_t C, double point);

/// @brief This function format the line
/// @param line line to format
/// @return the line formated
char *format_line(char *line);

/// @brief This function make the report
/// @param lineReport Line to report
/// @param time Time of simulation
/// @param output_path Path of report
/// @param states Number of states
void make_report(char *lineReport, time_t time, char *output_path,
                 uint64_t states);

void update_plate(double *matrix1, double *matrix2, size_t matrix_size);

uint8_t init_simulation(plate_t plate, char *output_path) {
  // this is the unique formula that have to aply
  double formula =
      (plate.time * plate.thermal_diffusivity) / (plate.alture * plate.alture);
  double *array1 = plate.arrayM1;
  double *array2 = plate.arrayM2;
  uint64_t R = plate.rows;
  uint64_t C = plate.columns;
  double point = plate.sensitivity;
  uint64_t states = transfer(array1, array2, formula, R, C, point);
  if (!states) {
    return 0;
  }
  int64_t totalTime = states * plate.time;
  time_t time_seconds = (time_t)totalTime;
  make_report(plate.lineReaded, time_seconds, output_path, states);
  return 1;
}

uint64_t transfer(double *matrix1, double *matrix2, double formula, uint64_t R,
                   uint64_t C, double point) {
  if (!matrix1 || !matrix2) {
    return 0;
  }
  uint64_t states = 0;
  bool limit = true;
  double up, down, left, right, center;
  while (limit) {
    limit = false;
    for (uint64_t i = 1; i < R - 1; i++) {
      for (uint64_t j = 1; j < C - 1; j++) {
        uint64_t idx = i * C + j;
        up = matrix2[idx - C];    // matrix2[i - 1][j]
        down = matrix2[idx + C];  // matrix2[i + 1][j]
        left = matrix2[idx - 1];  // matrix2[i][j - 1]
        right = matrix2[idx + 1]; // matrix2[i][j + 1]
        center = matrix2[idx];    // matrix2[i][j]
        matrix1[idx] = center + formula * (up + right + down + left - 4 * center);
        if (fabs(matrix1[idx] - center) > point) {
          limit = true;
        }
      }
    }
    if (limit) {
      double *temp = matrix2;
      matrix2 = matrix1;
      matrix1 = temp;
    }
    states++;
  }
  return states;
}

void make_report(char *lineReport, time_t time, char *output_path,
                 uint64_t states) {
  FILE *file = fopen(output_path, "a+");
  if (!file) {
    fprintf(stderr, "Error: can't find the path %s\n", output_path);
    return;
  }
  fputs(format_line(lineReport), file);
  fputc('\t', file);
  fputc('\t', file);
  char int_to_char[21];
  snprintf(int_to_char, sizeof(int_to_char), "%" SCNu64, (uint64_t)states);
  fputs(int_to_char, file);
  fputc('\t', file);
  fputc('\t', file);
  char *timeFormated = malloc(48 * sizeof(char));
  timeFormated = format_time(time, timeFormated, 48);
  printf("%s\n", timeFormated);
  fputs(timeFormated, file);
  fputc('\n', file);
  fclose(file);
  free(timeFormated);
}

// Return parameter text must have at least 48 chars (YYYY/MM/DD hh:mm:ss)
char *format_time(const time_t seconds, char *text, const size_t capacity) {
  const struct tm *gmt = gmtime(&seconds); // NOLINT
  snprintf(text, capacity, "%04d/%02d/%02d\t%02d:%02d:%02d", gmt->tm_year - 70,
           gmt->tm_mon, gmt->tm_mday - 1, gmt->tm_hour, gmt->tm_min,
           gmt->tm_sec);
  return text;
}

char *format_line(char *line) {
  for (uint32_t i = 0; line[i] != '\0'; i++) {
    if (line[i] == ' ' && line[i + 1] != ' ') {
      line[i] = '\t';
    }
  }
  return line;
}
