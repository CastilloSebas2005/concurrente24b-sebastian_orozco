// Copyright [2024] <Sebastián Orozco>

#include <math.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "simulation.h"
#include "plate.h"

/// @brief This is to format the time in the format YYYY/MM/DD hh:mm:ss
/// @param seconds Time in seconds
/// @param text This is the buffer to save the time
/// @param capacity This is the capacity of the buffer
/// @return The buffer with the time
char *format_time(const time_t seconds, char *text, const size_t capacity);

/// @brief This is to make the simulation of the plate
/// @param matrix1 This is the matrix1 of the plate
/// @param matrix2 This is the matrix2 of the plate
/// @param formula This is the formula to make the simulation
/// @param R Size of rows
/// @param C Size of columns
/// @param point This is the point of sensitivity
/// @return The number of states
uint64_t transfer(double **matrix1, double **matrix2, double formula,
                  uint64_t R, uint64_t C, double point);

/// @brief This is to format the line
/// @param line This is the line to format
/// @return The line formatted
char *format_line(char *line);

uint64_t init_simulation(plate_t plate) {
  // this is the unique formula that have to aply
  double formula =
      (plate.time * plate.thermal_diffusivity) / (plate.alture * plate.alture);
  double **plate_matrix1 = plate.plateM1;
  double **plate_matrix2 = plate.plateM2;
  if (!plate_matrix1 && !plate_matrix2) {
    return 0;
  }
  uint64_t R = plate.rows;
  uint64_t C = plate.columns;
  double point = plate.sensitivity;
  uint64_t states =
      transfer(plate_matrix1, plate_matrix2, formula, R, C, point);
  if (!states) {
    fprintf(stderr, "Error: can't make the simulation\n");
    return 0;
  }
  return states;
}

uint64_t transfer(double **matrix1, double **matrix2, double formula,
                  uint64_t R, uint64_t C, double point) {
  if (!matrix1 && !matrix2) {
    fprintf(stderr, "Error: matrix1 and matrix2 are NULL\n");
    return 0;
  }
  uint64_t states = 0;
  bool limit = true;
  while (limit) {
    limit = false;
    // apply the formula to the matrix
    for (uint64_t i = 1; i < R - 1; i++) {
      for (uint64_t j = 1; j < C - 1; j++) {
        matrix1[i][j] =
            matrix2[i][j] + formula * (matrix2[i - 1][j] + matrix2[i][j + 1] +
                                       matrix2[i + 1][j] + matrix2[i][j - 1] -
                                       4 * (matrix2[i][j]));
        // if the difference is greater than the point 
        // then simulation continues
        if ((fabs(matrix1[i][j] - matrix2[i][j]) > point)) {
          limit = true;
        }
      }
    }
    // copy the matrix1 to matrix2
    for (uint64_t i = 0; i < R - 1; i++) {
      for (uint64_t j = 0; j < C - 1; j++) {
        matrix2[i][j] = matrix1[i][j];
      }
    }
    states++;
  }
  return states;
}

char *make_line_to_report(char *lineReport, time_t time, uint64_t states) {
  if (!lineReport) {
    return NULL;
  }
  char int_to_char[21];
  snprintf(int_to_char, sizeof(int_to_char), "%" SCNu64, (uint64_t)states);
  char timeFormated[48];
  format_time(time, timeFormated, sizeof(timeFormated));
  size_t buffer_size = 0;
  lineReport = format_line(lineReport);
  for (size_t i = 0; lineReport[i] != '\0'; i++) {
    buffer_size++;
  }
  buffer_size += 48 + 21 + 4;
  char *buffer = (char *)malloc(buffer_size * sizeof(char));
  if (!buffer) {
    fprintf(stderr, "Error: can't allocate memory for buffer\n");
    return NULL;
  }
  snprintf(buffer, buffer_size, "%s\t%" SCNu64 "\t%s\n", lineReport,
           (uint64_t)states, timeFormated);
  return buffer;
}

// Return parameter text must have at least 48 chars (YYYY/MM/DD hh:mm:ss)
char *format_time(const time_t seconds, char *text, const size_t capacity) {
  struct tm gmt;
  /// use gmtime_r because is thread safe
  gmtime_r(&seconds, &gmt);
  snprintf(text, capacity, "%04d/%02d/%02d\t%02d:%02d:%02d", gmt.tm_year - 70,
           gmt.tm_mon, gmt.tm_mday - 1, gmt.tm_hour, gmt.tm_min, gmt.tm_sec);
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
