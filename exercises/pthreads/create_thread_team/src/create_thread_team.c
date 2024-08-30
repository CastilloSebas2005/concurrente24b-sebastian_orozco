// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// estructura de datos descrita en el enunciado
typedef struct private_data {
  uint64_t thread_number;
  uint64_t thread_count;
  void *shared_data;
  pthread_t thread_id;
} private_data_t;

// f unción de hello_w adaptada
void *greet(void *data);

// función nueva implementada
private_data_t *create_threads(size_t count, void *(*routine)(void *),
                               void *data);

// función nueva implementada
int join_threads(size_t count, private_data_t *private_data);

// procedure main(argc, argv[])
// función de hello_w adaptada para que pueda usar las funciones nuevas
int main(int argc, char *argv[]) {
#if 0
  for (int index = 0; index < argc; ++index) {
    printf("argv[%d] = '%s'\n", index, argv[index]);
  }
  return 0;
#endif

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
  // se debe de adaptar esta parte para que calce con nuestras funciones nuevas
  // y no den errores
  private_data_t *test = create_threads(thread_count, greet, NULL);
  // se llama a la función join_threads para "matar" a los hilos y liberar la
  // memoria inicializada en create_threads
  int error = join_threads(thread_count, test);
  return error;
}  // end procedure

private_data_t *create_threads(size_t count, void *(*routine)(void *),
                               void *data) {
  // to create the array of register
  // cambio descrito en el enunciado
  // se usa calloc para inicializar la memoria limpia
  private_data_t *private_data = calloc(count, sizeof(private_data_t));
  if (private_data) {
    for (size_t index = 0; index < count; ++index) {
      // asignación de datos decritos en el enunciado
      private_data[index].shared_data = data;
      private_data[index].thread_number = index;
      private_data[index].thread_count = count;

      // creation of threads
      // inicialización de hilos descritos en el enunciado
      if (pthread_create(&private_data[index].thread_id, NULL, routine,
                         &private_data[index]) != 0) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        join_threads(index, private_data);
        return NULL;
      }
    }
  } else {
    fprintf(stderr, "error: could not allocate memory\n");
    free(private_data);
    return NULL;
  }
  // cambio descrito en el enunciado
  return private_data;
}

// cambios en los parámetros descritos en el enunciado
int join_threads(size_t count, private_data_t *private_data) {
  int error_count = 0;
  for (size_t index = 0; index < count; ++index) {
    // cambio descrito en el enunciado
    const int error = pthread_join(private_data[index].thread_id, NULL);
    if (error) {
      fprintf(stderr, "error: could not join thread %zu\n", index);
      ++error_count;
    }
  }
  free(private_data);
  return error_count;
}

// procedure greet:
void *greet(void *data) {
  // assert(data);
  // adaptación de hello_w para que funcione con esta estructura y funciones
  // nuevas
  private_data_t *private_data = (private_data_t *)data;
  // se extraen datos de la estructura de datos que contiene el equipo de hilos
  uint64_t rank = private_data->thread_number;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 "\n", rank);
  return NULL;
}  // end procedure
