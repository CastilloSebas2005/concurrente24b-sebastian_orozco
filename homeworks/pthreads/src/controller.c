// Copyright [2024] <Sebastián Orozco>

#include <pthread.h>

#include "controller.h"
#include "manager_argument.h"
#include "plate.h"
#include "simulation.h"

/// @brief this is the private data of threads
typedef struct private_data {
  plate_t *plates;
  uint64_t linesToRead;
} private_data_t;

uint8_t create_threads(uint64_t thread_count, plate_t* plates, uint64_t linesToRead);
void* work(void *data);

void init_controller(char *argv[]) {
  manager_argument_t manager_argument;
  init_managerArgument(&manager_argument, argv);
  char *jobPath = get_jobPath(&manager_argument);
  char *output_Path = get_outputPath(&manager_argument);
  uint64_t thread_count = manager_argument.thread_count;
  if (!jobPath && !output_Path) {
    fprintf(stderr, "Error: don't exist the subdirectory, plase try again\n");
    return;
  }
  uint64_t linesToRead = get_lines_to_read(jobPath);
  if (linesToRead == 0) {
    fprintf(stderr, "Error: something bad in your arguments or jobFile\n");
    destruct_manager(&manager_argument);
    return;
  }
  plate_t plates[linesToRead]; // NOLINT
  printf("%li", linesToRead);
  for (uint64_t i = 0; i < linesToRead; i++) {
    init_plate(&plates[i], jobPath, argv[2], i);
  }
  create_threads(thread_count, plates, linesToRead);
  for (uint64_t i = 0; i < linesToRead; i++) {
    destruct_plate(&plates[i]);
  }
  destruct_manager(&manager_argument);
}

uint8_t create_threads(uint64_t thread_count, plate_t* plates, uint64_t linesToRead) {
  pthread_t *threads = (pthread_t*)(malloc(thread_count * sizeof(pthread_t)));
  private_data_t *private_data = (private_data_t*)(calloc(thread_count, sizeof(private_data_t)));
  if (!threads || !private_data) {
    fprintf(stderr, "Error: can't create threads or allocate memory\n");
    free(threads);
    free(private_data);
    return 1;
  }

  // Calcular la división del trabajo
  uint64_t workDivision = linesToRead / thread_count;
  uint64_t remainder = linesToRead % thread_count;

  // Asignar memoria para los arrays plates dentro de private_data
  for (size_t i = 0; i < thread_count; i++) {
    uint64_t linesForThisThread = workDivision + (i < remainder ? 1 : 0);
    private_data[i].plates = (plate_t*)(malloc(linesForThisThread * sizeof(plate_t)));
    private_data[i].linesToRead = linesForThisThread;
    if (!private_data[i].plates) {
      fprintf(stderr, "Error: can't allocate memory for plates\n");
      for (size_t j = 0; j <= i; j++) {
        free(private_data[j].plates);
      }
      free(private_data);
      free(threads);
      return 1;
    }
  }

  size_t plateIndex = 0;
  for (size_t i = 0; i < thread_count; i++) {
    for (size_t j = 0; j < private_data[i].linesToRead; j++) {
      private_data[i].plates[j] = plates[plateIndex++];
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
    
  }
  free(private_data);
  free(threads);

  return 0;
}

void* work(void *data) {
  printf("Thread\n");
  private_data_t *private_data = (private_data_t*)data;
  for (uint64_t i = 0; i < private_data->linesToRead; i++) {
    printf("hola\n");
    init_simulation(private_data->plates[i], "output/job002.tsv");
  }
  return NULL;
}