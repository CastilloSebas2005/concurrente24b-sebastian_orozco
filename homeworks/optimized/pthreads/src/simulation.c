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
uint64_t transfer(double *matrix1, double *matrix2, double formula,
                  uint64_t R, uint64_t C, double point);

/// @brief This is to format the line
/// @param line This is the line to format
/// @return The line formatted
char *format_line(char *line);

uint64_t init_simulation(plate_t plate) {
  // this is the unique formula that have to aply
  double formula =
      (plate.time * plate.thermal_diffusivity) / (plate.alture * plate.alture);
  uint64_t R = plate.rows;
  uint64_t C = plate.columns;
  double point = plate.sensitivity;
  uint64_t states =
      transfer(plate.arrayM1, plate.arrayM2, formula, R, C, point);
  if (!states) {
    fprintf(stderr, "Error: can't make the simulation\n");
    return 0;
  }
  return states;
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
    // Iterate with loop unrolling
    for (uint64_t i = 1; i < R - 1; i++) {
      uint64_t base_idx = i * C;
      for (uint64_t j = 1; j < C - 1; j += 4) {
        uint64_t idx = base_idx + j;
        // It 1
        up = matrix2[idx - C];
        down = matrix2[idx + C];
        left = matrix2[idx - 1];
        right = matrix2[idx + 1];
        center = matrix2[idx];
        matrix1[idx] =
            center + formula * (up + right + down + left - 4 * center);
        if (fabs(matrix1[idx] - center) > point) {
          limit = true;
        }

        // It 2
        if (j + 1 < C - 1) {
          idx++;
          up = matrix2[idx - C];
          down = matrix2[idx + C];
          left = matrix2[idx - 1];
          right = matrix2[idx + 1];
          center = matrix2[idx];
          matrix1[idx] =
              center + formula * (up + right + down + left - 4 * center);
          if (fabs(matrix1[idx] - center) > point) {
            limit = true;
          }
        }

        // It 3
        if (j + 2 < C - 1) {
          idx++;
          up = matrix2[idx - C];
          down = matrix2[idx + C];
          left = matrix2[idx - 1];
          right = matrix2[idx + 1];
          center = matrix2[idx];
          matrix1[idx] =
              center + formula * (up + right + down + left - 4 * center);
          if (fabs(matrix1[idx] - center) > point) {
            limit = true;
          }
        }

        // It 4
        if (j + 3 < C - 1) {
          idx++;
          up = matrix2[idx - C];
          down = matrix2[idx + C];
          left = matrix2[idx - 1];
          right = matrix2[idx + 1];
          center = matrix2[idx];
          matrix1[idx] =
              center + formula * (up + right + down + left - 4 * center);
          if (fabs(matrix1[idx] - center) > point) {
            limit = true;
          }
        }
      }
    }
    // To update the matrix
    if (limit) {
      double *temp = matrix2;
      matrix2 = matrix1;
      matrix1 = temp;
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
