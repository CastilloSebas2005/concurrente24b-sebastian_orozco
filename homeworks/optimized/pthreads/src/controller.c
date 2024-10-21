// Copyright [2024] <Sebastián Orozco>

#include <assert.h>
#include <pthread.h>
#include <time.h>

#include "controller.h"
#include "manager_argument.h"
#include "plate.h"
#include "simulation.h"

/**
 * @brief This is the shared data of threads
 * @struct shared_data
 * @var shared_data::line_report
 * This is the array of lines to report, this contains the lines formatted
 * and this is saved in the output file
 */
typedef struct shared_data {
  char **line_report;
  uint64_t thread_count;
} shared_data_t;

/**
 * @brief this is the private data of threads
 * @struct private_data
 * @var private_data::plates
 * This is the array of plates, this contains the plates to make the simulation
 * @var private_data::linesToRead
 * This is the number of lines to read, this is unique in all threads
 * because all threads read the same file but plates are different
 * @var private_data::lines_to_write
 * This is the array of lines to write, this contains the number of line
 * that must write in the output file.
 * @var private_data::shared_data
 * This is the shared data of threads, this contains the lines to report
 *
 */
typedef struct private_data {
  plate_t *plates;
  uint64_t linesToRead;
  uint64_t *lines_to_write;
  shared_data_t *shared_data;
} private_data_t;

/// @brief This is to create the threads
/// @param thread_count Size of threads
/// @param plates Array of plates
/// @param linesToRead Number of lines to read
/// @param shared_data shared_data of threads
/// @return 0 if all is good 1 if occurs an error
void create_threads(uint64_t thread_count, plate_t *plates,
                    uint64_t linesToRead, shared_data_t *shared_data);

/// @brief This is the work of threads to make the simulation
/// @param data This is the private data of threads
/// @return NULL
void *work(void *data);
uint8_t make_report(char **report, char *output_path, uint64_t linesToRead);

void init_controller(char *argv[]) {
  manager_argument_t manager_argument;
  init_managerArgument(&manager_argument, argv);
  // get the path and verificate that path exist of job file and output file
  char *jobPath = get_jobPath(&manager_argument);
  char *output_Path = get_outputPath(&manager_argument);
  uint64_t thread_count = manager_argument.thread_count;
  if (!jobPath || !output_Path) {
    destruct_manager(&manager_argument);
    return;
  }
  uint64_t linesToRead = get_lines_to_read(jobPath);
  if (linesToRead == 0) {
    destruct_manager(&manager_argument);
    return;
  }
  if (thread_count == 0) {
    destruct_manager(&manager_argument);
    return;
  }
  shared_data_t *shared_data =
      (shared_data_t *)calloc(1, sizeof(shared_data_t));
  shared_data->line_report = (char **)calloc(linesToRead, sizeof(char *));
  shared_data->thread_count = thread_count;
  plate_t plates[linesToRead]; // NOLINT
  for (uint64_t i = 0; i < linesToRead; i++) {
    init_plate(&plates[i], jobPath, argv[2], i);
  }
  /// @note this is to manage the threads correctly
  /// because if the number of threads is 1, then the program
  /// must execute in a single thread
  if (thread_count == 1) {
    // Execute in a single thread
    for (uint64_t i = 0; i < linesToRead; i++) {
      if (plates[i].blankline == 0) {
        uint64_t states = init_simulation(plates[i], thread_count);
        char *line = make_line_to_report(
            plates[i].lineReaded, (time_t)(plates[i].time * states), states);
        shared_data->line_report[i] = line;
        printf("Plate number: %" SCNu64 " calculated correctly...\n", i);
      }
    }
  } else {
    // Execute in multiple threads
    create_threads(thread_count, plates, linesToRead, shared_data);
  }
  uint8_t error =
      make_report(shared_data->line_report, output_Path, linesToRead);
  if (!error) {
    printf("The report was created successfully\n");
  }
  for (uint64_t i = 0; i < linesToRead; i++) {
    if (plates[i].blankline == 0) {
      destruct_plate(&plates[i]);
    }
  }
  for (uint64_t i = 0; i < linesToRead; i++) {
    if (shared_data->line_report[i]) {
      free(shared_data->line_report[i]);
    }
  }
  free(shared_data->line_report);
  free(shared_data);
  destruct_manager(&manager_argument);
}

void create_threads(uint64_t thread_count, plate_t *plates,
                    uint64_t linesToRead, shared_data_t *shared_data) {
  pthread_t *threads = (pthread_t *)(malloc(thread_count * sizeof(pthread_t)));
  private_data_t *private_data =
      (private_data_t *)(calloc(thread_count, sizeof(private_data_t)));
  if (!threads || !private_data) {
    fprintf(stderr, "Error: can't create threads or allocate memory\n");
    free(threads);
    free(private_data);
    return;
  }

  /// @note this is to divide the work of threads
  uint64_t workDivision = linesToRead / thread_count;
  /// @note this is to know if the division is exact
  uint64_t remainder = linesToRead % thread_count;
  // Create the private data for each thread and assign the plates
  for (size_t i = 0; i < thread_count; i++) {
    uint64_t linesForThisThread = workDivision + (i < remainder ? 1 : 0);
    private_data[i].plates =
        (plate_t *)(malloc(linesForThisThread * sizeof(plate_t)));
    private_data[i].linesToRead = linesForThisThread;
    private_data[i].lines_to_write =
        (uint64_t *)(malloc(linesForThisThread * sizeof(uint64_t)));
    private_data[i].shared_data = shared_data;
    if (!private_data[i].plates && !private_data[i].lines_to_write) {
      fprintf(stderr, "Error: can't allocate memory for plates\n");
      for (size_t j = 0; j <= i; j++) {
        free(private_data[j].plates);
      }
      free(private_data);
      free(threads);
      return;
    }
  }

  size_t plateIndex = 0;
  /// @note this is to assign the plates to each thread
  for (size_t i = 0; i < thread_count; i++) {
    for (size_t j = 0; j < private_data[i].linesToRead; j++) {
      if (!plates[j].blankline) {
        /// @note this is to assign the line to write in the shared data
        /// this is to use the condition safe technique
        private_data[i].lines_to_write[j] = plateIndex;
        private_data[i].plates[j] = plates[plateIndex];
      }
      plateIndex++;
    }
  }

  for (size_t i = 0; i < thread_count; i++) {
    pthread_create(&threads[i], NULL, work, &private_data[i]);
  }

  for (size_t i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
  }

  for (size_t i = 0; i < thread_count; i++) {
    free(private_data[i].plates);
    free(private_data[i].lines_to_write);
  }
  free(private_data);
  free(threads);

  return;
}

void *work(void *data) {
  assert(data);
  private_data_t *private_data = (private_data_t *)data;
  for (uint64_t i = 0; i < private_data->linesToRead; i++) {
    if (private_data->plates[i].blankline == 0) {
      uint64_t states = init_simulation(private_data->plates[i], private_data->shared_data->thread_count);
      char *line = make_line_to_report(
          private_data->plates[i].lineReaded,
          (time_t)(private_data->plates[i].time * states), states);
      if (line || states != 0) {
        printf("Plate number: %" SCNu64 " calculate correctly...\n",
               private_data->lines_to_write[i]);
        /// @note this is to write the line in the shared data
        /// and not perturb the other threads
        private_data->shared_data
            ->line_report[private_data->lines_to_write[i]] = line;
      }
    }
  }
  return NULL;
}

uint8_t make_report(char **report, char *output_path, uint64_t linesToRead) {
  if (!report && !output_path) {
    fprintf(stderr, "Error: can't make the report\n");
    return 1;
  }
  FILE *file = fopen(output_path, "w");
  if (!file) {
    fprintf(stderr, "Error: can't create report, change your directory %s\n",
            output_path);
    return 1;
  }
  for (uint64_t i = 0; i < linesToRead; i++) {
    if (report[i]) {
      fputs(report[i], file);
    }
  }
  fclose(file);
  return 0;
}
