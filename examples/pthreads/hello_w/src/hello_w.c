// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief ...
 */
void *greet(void *data);
int create_threads(uint64_t thread_count);

// procedure main(argc, argv[])
int main(int argc, char *argv[]) {
#if 0
  for (int index = 0; index < argc; ++index) {
    printf("argv[%d] = '%s'\n", index, argv[index]);
  }
  return 0;
#endif

  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  // se valida que el número de argumentos sean 2
  if (argc == 2) {
    // se valida que los argumentos sean efectivamente un entero sin signo de 64
    // bits
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      // si no es así se imprime en la salida del error estándar
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
  // se crea un arreglo de hilos con la cantidad de hilos que se pasó por el
  // argumento
  pthread_t *threads = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
  if (threads) {
    for (uint64_t thread_number = 0; thread_number < thread_count;
         ++thread_number) {
      // create_thread(greet, thread_number)
      // con cada iteración se crea un hilo nuevo
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet,
                             /*arg*/ (void *)thread_number);
      if (error == EXIT_SUCCESS) {
      } else {  // si no se puede crear el hilo se imprime en la salida estándar
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < thread_count;
         ++thread_number) {
    // se espera para "matar" a los hilos creados
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }
    // se libera el arreglo de hilos para evitar fugas
    free(threads);
  } else {  // si el valor del arreglo de hilos es 0, se imprime este error
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n",
            thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:
void *greet(void *data) {
  // assert(data);
  const uint64_t rank = (uint64_t)data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 "\n", rank);
  return NULL;
}  // end procedure
