// Copyright [2024] <Sebastián Orozco>

#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "barrier.h"
#include "plate.h"
#include "simulation.h"

/// @brief This is the shared data of threads
/// @struct shared_data
/// @var shared_data::thread_count
/// This is the count of threads
/// @var shared_data::formula
/// This is the formula to make the simulation
/// @var shared_data::matrix1
/// This is the matrix1 of the plate
/// @var shared_data::matrix2
/// This is the matrix2 of the plate
/// @var shared_data::R
/// This is the size of rows
/// @var shared_data::C
/// This is the size of columns
/// @var shared_data::limit
/// This is the limit of the simulation
/// @var shared_data::point
/// This is the point of sensitivity
/// @var shared_data::barrier
/// This is the barrier to synchronize the threads
/// @var shared_data::terminate
/// This is the flag to terminate the threads
typedef struct shared_data {
  uint64_t thread_count;
  double formula;
  double *matrix1;
  double *matrix2;
  uint64_t R;
  uint64_t C;
  bool limit;
  double point;
  barrier_t barrier;
  bool terminate;
} shared_data_t;

/// @brief This is the private data of threads
/// @struct private_data
/// @var private_data::start_row
/// This is the start row of the thread
/// @var private_data::end_row
/// This is the end row of the thread
/// @var private_data::limit
/// This is the limit of the simulation
/// @var private_data::thread_id
/// This is the id of the thread
/// @var private_data::shared_data
/// This is the shared data of the threads
typedef struct {
  uint64_t start_row;
  uint64_t end_row;
  bool limit;
  uint64_t thread_id;
  shared_data_t *shared_data;
} private_data_t;

/// @brief This is to format the time in the format YYYY/MM/DD hh:mm:ss
/// @param seconds Time in seconds
/// @param text This is the buffer to save the time
/// @param capacity This is the capacity of the buffer
/// @return The buffer with the time
char *format_time(const time_t seconds, char *text, const size_t capacity);

/// @brief This is the function to make the simulation of the plate
/// @param shared_data This is the shared data of threads
/// @param private_data This is the private data of threads
/// @return The number of states
uint64_t transfer_parallel(shared_data_t *shared_data,
                           private_data_t *private_data);

/// @brief This is the function to make the simulation of the plate
/// @param matrix1 This is the matrix1 of the plate
/// @param matrix2 This is the matrix2 of the plate
/// @param formula This is the formula to make the simulation
/// @param R Size of rows
/// @param C Size of columns
/// @param point This is the point of sensitivity
/// @return The number of states
uint64_t transfer_serial(double *matrix1, double *matrix2, double formula,
                         uint64_t R, uint64_t C, double point);

/// @brief This is the function to calculate the transfer of the with the
/// threads
/// @param shared_data shared data of the threads
/// @param private_data private data of the threads
/// @param thread_count count of threads
void calculate_transfer(shared_data_t *shared_data);

/// @brief This is the function to make the transfer of the plate
/// @param private_data This is the private data of the threads
/// @return NULL
void *transfer_thread(void *private_data);

/// @brief This is to format the line
/// @param line This is the line to format
/// @return The line formatted
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
    init_barrier(&shared_data->barrier, Thread_count + 1);
    pthread_t *threads = (pthread_t *)malloc(Thread_count * sizeof(pthread_t));
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
      pthread_create(&threads[i], NULL, transfer_thread, &private_data[i]);
    }

    uint64_t states = transfer_parallel(shared_data, private_data);

    // To indicate that threads should terminate
    shared_data->terminate = true;
    wait_barrier(&shared_data->barrier);

    for (uint64_t i = 0; i < Thread_count; i++) {
      pthread_join(threads[i], NULL);
    }

    destruct_barrier(&shared_data->barrier);
    free(shared_data);
    free(threads);
    free(private_data);
    return states;
  }
  // this is the unique formula that have to aply
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

uint64_t transfer_parallel(shared_data_t *shared_data,
                           private_data_t *private_data) {
  if (!shared_data->matrix1 || !shared_data->matrix2) {
    return 0;
  }
  uint64_t states = 0;
  bool global_limit = true;

  while (global_limit) {
    global_limit = false;

    calculate_transfer(shared_data);

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

void calculate_transfer(shared_data_t *shared_data) {
  wait_barrier(&shared_data->barrier);

  wait_barrier(&shared_data->barrier);
}

void *transfer_thread(void *private_data) {
  private_data_t *data = (private_data_t *)private_data;
  shared_data_t *shared_data = data->shared_data;

  while (true) {
    wait_barrier(&shared_data->barrier);

    if (shared_data->terminate) {
      break;
    }

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
    // Wait for all threads to finish
    wait_barrier(&shared_data->barrier);
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
