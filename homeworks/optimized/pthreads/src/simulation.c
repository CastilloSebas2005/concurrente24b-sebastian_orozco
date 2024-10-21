// Copyright [2024] <Sebastián Orozco>

#include <math.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "simulation.h"
#include "plate.h"

/// @brief This is the data to make the simulation
typedef struct {
    double *matrix1;
    double *matrix2;
    double formula;
    uint64_t start_row;
    uint64_t end_row;
    uint64_t C;
    double point;
    bool *limit;
} thread_data_t;

/// @brief This is the function to make the simulation of the plate
/// @param data This is the data to make the simulation
/// @return NULL
void *thread_transfer(void* data);

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
                  uint64_t R, uint64_t C, double point, uint64_t thread_count);

/// @brief This is to format the line
/// @param line This is the line to format
/// @return The line formatted
char *format_line(char *line);

uint64_t init_simulation(plate_t plate, uint64_t thread_count) {
  // this is the unique formula that have to aply
  double formula =
      (plate.time * plate.thermal_diffusivity) / (plate.alture * plate.alture);
  uint64_t R = plate.rows;
  uint64_t C = plate.columns;
  double point = plate.sensitivity;
  uint64_t states =
      transfer(plate.arrayM1, plate.arrayM2, formula, R, C, point, thread_count);
  if (!states) {
    fprintf(stderr, "Error: can't make the simulation\n");
    return 0;
  }
  return states;
}

uint64_t transfer(double *matrix1, double *matrix2, double formula, uint64_t R, uint64_t C, double point, uint64_t thread_count) {
    if (!matrix1 || !matrix2) {
        return 0;
    }

    pthread_t *threads = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
    thread_data_t *thread_data = (thread_data_t *)malloc(thread_count * sizeof(thread_data_t));
    uint64_t rows_per_thread = (R - 2) / thread_count;  // Dividimos las filas entre hilos
    uint64_t states = 0;
    bool limit = true;

    while (limit) {
        limit = false;
        
        // create threads
        for (uint64_t t = 0; t < thread_count; t++) {
            thread_data[t].matrix1 = matrix1;
            thread_data[t].matrix2 = matrix2;
            thread_data[t].formula = formula;
            thread_data[t].start_row = 1 + t * rows_per_thread;
            thread_data[t].end_row = (t == thread_count - 1) ? R - 1 : (1 + (t + 1) * rows_per_thread);
            thread_data[t].C = C;
            thread_data[t].point = point;
            thread_data[t].limit = &limit;

            pthread_create(&threads[t], NULL, thread_transfer, (void *)&thread_data[t]);
        }

        // wait for threads
        for (uint64_t t = 0; t < thread_count; t++) {
            pthread_join(threads[t], NULL);
        }

        // update the matrix
        if (limit) {
            double *temp = matrix2;
            matrix2 = matrix1;
            matrix1 = temp;
        }

        states++;
    }
    free(threads);
    free(thread_data);
    return states;
}

void *thread_transfer(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;

    double *matrix1 = data->matrix1;
    double *matrix2 = data->matrix2;
    double formula = data->formula;
    uint64_t start_row = data->start_row;
    uint64_t end_row = data->end_row;
    uint64_t C = data->C;
    double point = data->point;
    bool *limit = data->limit;

    double up, down, left, right, center;
    for (uint64_t i = start_row; i < end_row; i++) {
        uint64_t base_idx = i * C;
        for (uint64_t j = 1; j < C - 1; j++) {
            uint64_t idx = base_idx + j;
            up = matrix2[idx - C];
            down = matrix2[idx + C];
            left = matrix2[idx - 1];
            right = matrix2[idx + 1];
            center = matrix2[idx];
            matrix1[idx] = center + formula * (up + right + down + left - 4 * center);
            if (fabs(matrix1[idx] - center) > point) {
                *limit = true;
            }
        }
    }

    return NULL;
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
