// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// thread_private_data_t
typedef struct private_data {
  uint64_t thread_number;  // rank
  uint64_t thread_count;
  // puntero a estructura de datos privados
  struct private_data *next;
} private_data_t;

/**
 * @brief ...
 */
void *greet(void *data);
int create_threads(uint64_t thread_count);

// procedure main(argc, argv[])
int main(int argc, char *argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }
  error = create_threads(thread_count);
  return error;
}  // end procedure

int create_threads(uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  // se le pide memoria al sistema operativo
  // se crea un arreglo multiplicando la cantidad de memoria que ocupa un hilo
  // por la cantidad de hilos enviados
  pthread_t *threads = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
  // se crea con calloc porque es una estructura de datos propia, si se hace con
  // malloc puede haber memoria "basura"
  private_data_t *private_data =
      (private_data_t *)calloc(thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < thread_count;
         ++thread_number) {
      // a cada dato privado se le esta asignando los valores y cantidad de
      // hilos que siempre es la ingresado
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].thread_count = thread_count;
      // create_thread(greet, thread_number
      // se crean los hilos y se le asigna a cada hilo la estructura de datos
      // private_data correspondiente
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet,
                             /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    // aca se "matan" todos los hilos creados anteriormente
    for (uint64_t thread_number = 0; thread_number < thread_count;
         ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }
    // se debe liberar la memoria para una buena gestion de la misma
    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n",
            thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:
void *greet(void *data) {
  // assert(data);
  // se castea el parametro a private_data_t
  private_data_t *private_data = (private_data_t *)data;
  // print "Hello from secondary thread"

  // acá en este print se castea private data de las dos formas posibles
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n",
         (*private_data).thread_number, private_data->thread_count);
  return NULL;
}  // end procedure
