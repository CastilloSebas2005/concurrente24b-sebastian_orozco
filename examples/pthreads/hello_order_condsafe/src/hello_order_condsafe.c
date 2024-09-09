// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_GREET_LEN 256
/// @note MAX_GREET_LEN 256 es simplemente para
/// definir los arreglos de tamaño 256

// thread_shared_data_t

/// @struct shared_data
/// @brief Estructura de datos compartidos vista en ejemplos anteriores

/** @var shared_data::greets
 *  Note que esta es una matriz de char**
 *  esta es la estructura de datos donde van a trabajar los hilos
 *  esta va a ser nuestra piscina con carriles, cada hilo
 *  trabajará en la posición asignada, pueden haber dos hilos
 *  en diferentes posiciones trabajando al mismo tiempo
 *  sin embargo no se cruzaran, cada uno tendrá su camino definido
 *  se puede decir que esto es un tipo de memoria semiprivada,
 *  cada hilo tendrá su espacio de memoria.
 *
 */
typedef struct shared_data {
  char **greets;
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t

/// @brief Estructura de datos compartidos vista en ejemplos anteriores
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t *shared_data;
} private_data_t;

/**
 * @brief ...
 */
void *greet(void *data);
int create_threads(shared_data_t *shared_data);

// procedure main(argc, argv[])

/// @brief La mayor parte es igual a los ejemplos anteriores
/// @param argc Número de argumentos
/// @param argv Argumentos
/// @return variable error
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

  shared_data_t *shared_data =
      (shared_data_t *)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    /// @note shared_data->greets = (char**) calloc(thread_count, sizeof(char*))
    /// simplemente es para inicializar la estructura de datos donde se van a
    /// escribir los saludos. Se inicializa con 0 para que haya mayor seguridad.
    /// nota: solo sería la primera parte de la matriz por decirlo de alguna
    /// manera los saludos falta inicializarlo aún, esto se hará en la función
    /// create_threads.
    shared_data->greets = (char **)calloc(thread_count, sizeof(char *));
    shared_data->thread_count = thread_count;

    if (shared_data->greets) {
      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
                            (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

      printf("Execution time: %.9lfs\n", elapsed_time);
      /// @note free(shared_data->greets) para liberar la memoria solicitada
      free(shared_data->greets);
    } else {
      fprintf(stderr, "Error: could not allocate greets\n");
      error = 13;
    }
    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }
  return error;
}  // end procedure

/// @brief Función comentada anteriormente
/// @param shared_data struct shared_data
/// @return variable error
int create_threads(shared_data_t *shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  pthread_t *threads =
      (pthread_t *)malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t *private_data = (private_data_t *)calloc(
      shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
         ++thread_number) {
      /** @note  shared_data->greets[thread_number] =
       *  (char *)malloc(MAX_GREET_LEN * sizeof(char));
       *  esto es para inicializar la parte de adentro de la matriz
       *  como se sabe que una matriz es un arreglo de arreglos
       *  acá se están inicializando los arreglos del arreglo
       */
      shared_data->greets[thread_number] =
          (char *)malloc(MAX_GREET_LEN * sizeof(char));
      /// @note if (shared_data->greets[thread_number])
      /// acá se pregunta si se tiene memoria
      if (shared_data->greets[thread_number]) {
        // *shared_data->greets[thread_number] = '\0';
        /// @note shared_data->greets[thread_number][0] = '\0'
        /// esto es simplemente para que la primera posición de esa matriz
        /// no tenga basura, no importan las demás posiciones, simplemente se
        /// necesita que esa esté vacía, se puede usar un calloc
        /// para que las demás
        /// estén limpias y sea más seguro.
        shared_data->greets[thread_number][0] = '\0';
        private_data[thread_number].thread_number = thread_number;
        private_data[thread_number].shared_data = shared_data;
        // create_thread(greet, thread_number)
        error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet,
                               /*arg*/ &private_data[thread_number]);
        if (error == EXIT_SUCCESS) {
        } else {
          fprintf(stderr, "Error: could not create secondary thread\n");
          error = 21;
          break;
        }
      } else {
        fprintf(stderr, "Error: could not init semaphore\n");
        error = 22;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
         ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    // for thread_number := 0 to thread_count do
    /** @note for (uint64_t thread_number = 0; thread_number <
    shared_data->thread_count;
         ++thread_number) {
      // print greets[thread_number]
      printf("%s\n", shared_data->greets[thread_number]);
      free(shared_data->greets[thread_number]);
    }  // end for
     * Acá basicamente lo que se está imprimiento son los saludos almacenados en
    la matriz
     * y luego se libera la memoria para evitar fugas
     */
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
         ++thread_number) {
      // print greets[thread_number]
      printf("%s\n", shared_data->greets[thread_number]);
      free(shared_data->greets[thread_number]);
    }  // end for

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n",
            shared_data->thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:

/// @brief Función que va a escribir los saludos en la estructura
/// de datos
/// @param data Memoria compartida, pero está ya está partida en pedazos
/// y estos pedazos ya están asignados a los hilos
/// @return NULL
void *greet(void *data) {
  assert(data);
  private_data_t *private_data = (private_data_t *)data;
  shared_data_t *shared_data = private_data->shared_data;

  // greets[thread_number] := format("Hello from secondary thread"
  // , thread_number, " of ", thread_count)

  /** @note se le puso "NOLINT", debido a que el linter
   * recomendaba lo siguiente: Never use sprintf. Use snprintf instead.
   */

  /** @note sprintf(shared_data->greets[private_data->thread_number]
          ,
          "Hello from secondary thread %" PRIu64 " of %" PRIu64,
          private_data->thread_number, shared_data->thread_count);
   * Esto es simplemente para escribir el saludo en la posición de memoria
   compartida
   * asignada para cada hilo, note que cada hilo tiene un identificador
   * este es thread_number y la memoria compartida tiene una estructura de datos
   * que es del mismo tamaño que la cantidad de hilos, por lo que siempre va a
   haber
   * una posición en esa para cada hilo, ya que el tamaño de la estructura
   * coincide con la cantidad de hilos, por ejemplo si escribe el hilo 1
   * en la estructura hay una posición 1 y así con todos los hilos creados.
   *
   */
  sprintf(shared_data->greets[private_data->thread_number] // NOLINT
          ,
          "Hello from secondary thread %" PRIu64 " of %" PRIu64,
          private_data->thread_number, shared_data->thread_count);

  return NULL;
}  // end procedure
