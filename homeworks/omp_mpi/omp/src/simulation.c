// Copyright [2024] <Sebastián Orozco>

#include <inttypes.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "plate.h"
#include "simulation.h"

typedef struct shared_data {
  uint64_t thread_count;
  double formula;
  double *matrix1;
  double *matrix2;
  uint64_t R;
  uint64_t C;
  bool limit;
  double point;
  bool terminate;
} shared_data_t;

typedef struct {
  uint64_t start_row;
  uint64_t end_row;
  bool limit;
  uint64_t thread_id;
  shared_data_t *shared_data;
} private_data_t;

char *format_time(const time_t seconds, char *text, const size_t capacity);

uint64_t transfer_parallel(shared_data_t *shared_data,
                           private_data_t *private_data);

uint64_t transfer_serial(double *matrix1, double *matrix2, double formula,
                         uint64_t R, uint64_t C, double point);

void calculate_transfer(shared_data_t *shared_data);

void *transfer_thread(void *private_data);

char *format_line(char *line);

uint64_t init_simulation(plate_t plate, uint64_t Thread_count) {
  if (plate.columns > 50 && plate.rows > 50 && Thread_count > 1) {
    if (Thread_count > plate.rows) {
      Thread_count = plate.rows;
    }
    printf("Thread count simulation: %lu\n", Thread_count);
    shared_data_t *shared_data = (shared_data_t *)malloc(sizeof(shared_data_t));
    shared_data->formula = (plate.time * plate.thermal_diffusivity) /
                           (plate.alture * plate.alture);
    shared_data->matrix1 = plate.arrayM1;
    shared_data->matrix2 = plate.arrayM2;
    shared_data->R = plate.rows;
    shared_data->C = plate.columns;
    shared_data->point = plate.sensitivity;
    shared_data->thread_count = Thread_count;
    shared_data->limit = false;
    shared_data->terminate = false;

    private_data_t *private_data =
        (private_data_t *)calloc(Thread_count, sizeof(private_data_t));
    for (uint64_t i = 0; i < Thread_count; i++) {
      private_data[i].start_row = 1 + i * ((shared_data->R - 2) / Thread_count);
      private_data[i].end_row =
          (i == Thread_count - 1)
              ? shared_data->R - 1
              : 1 + (i + 1) * ((shared_data->R - 2) / Thread_count);
      private_data[i].thread_id = i;
      private_data[i].shared_data = shared_data;
      private_data[i].limit = false;
    }

    uint64_t states = transfer_parallel(shared_data, private_data);

    free(shared_data);
    free(private_data);
    return states;
  }
  double formula =
      (plate.time * plate.thermal_diffusivity) / (plate.alture * plate.alture);
  double *array1 = plate.arrayM1;
  double *array2 = plate.arrayM2;
  uint64_t R = plate.rows;
  uint64_t C = plate.columns;
  double point = plate.sensitivity;
  uint64_t states = transfer_serial(array1, array2, formula, R, C, point);
  if (!states) {
    return 0;
  }
  return states;
}

uint64_t transfer_serial(double *matrix1, double *matrix2, double formula,
                         uint64_t R, uint64_t C, double point) {
  if (!matrix1 || !matrix2) {
    return 0;
  }
  uint64_t states = 0;
  bool limit = true;
  double up, down, left, right, center;
  while (limit) {
    limit = false;
    for (uint64_t i = 1; i < R - 1; i++) {
      uint64_t base_idx = i * C;
      for (uint64_t j = 1; j < C - 1; j++) {
        uint64_t idx = base_idx + j;
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
    if (limit) {
      double *temp = matrix2;
      matrix2 = matrix1;
      matrix1 = temp;
    }
    states++;
  }
  return states;
}

uint64_t transfer_parallel(shared_data_t *shared_data,
                           private_data_t *private_data) {
  if (!shared_data->matrix1 || !shared_data->matrix2) {
    return 0;
  }
  uint64_t states = 0;
  bool global_limit = true;

  while (global_limit) {
    global_limit = false;

    #pragma omp parallel num_threads(shared_data->thread_count)
    {
      #pragma omp single
      {
        for (uint64_t i = 0; i < shared_data->thread_count; i++) {
          #pragma omp task firstprivate(i)
          {
            transfer_thread(&private_data[i]);
          }
        }
      }
      #pragma omp taskwait
    }

    for (uint64_t i = 0; i < shared_data->thread_count; i++) {
      if (private_data[i].limit) {
        global_limit = true;
        private_data[i].limit = false;
      }
    }

    if (global_limit) {
      double *temp = shared_data->matrix2;
      shared_data->matrix2 = shared_data->matrix1;
      shared_data->matrix1 = temp;
    }
    states++;
  }

  return states;
}

void *transfer_thread(void *private_data) {
  private_data_t *data = (private_data_t *)private_data;
  shared_data_t *shared_data = data->shared_data;

  for (uint64_t i = data->start_row; i < data->end_row; i++) {
    for (uint64_t j = 1; j < shared_data->C - 1; j++) {
      uint64_t idx = i * shared_data->C + j;
      double up = shared_data->matrix2[idx - shared_data->C];
      double down = shared_data->matrix2[idx + shared_data->C];
      double left = shared_data->matrix2[idx - 1];
      double right = shared_data->matrix2[idx + 1];
      double center = shared_data->matrix2[idx];
      shared_data->matrix1[idx] =
          center +
          shared_data->formula * (up + right + down + left - 4 * center);

      if (fabs(shared_data->matrix1[idx] - center) > shared_data->point) {
        data->limit = true;
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

char *format_time(const time_t seconds, char *text, const size_t capacity) {
  struct tm gmt;
  gmtime_r(&seconds, &gmt);
  snprintf(text, capacity, "%04d/%02d/%02d\t%02d:%02d:%02d", gmt.tm_year - 70, // NOLINT
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
