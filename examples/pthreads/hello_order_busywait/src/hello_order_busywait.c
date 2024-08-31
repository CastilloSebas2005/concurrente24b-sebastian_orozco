// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/** @mainpage Note que no se va a documentar cosas de ejemplos anteriores solamente lo relevante de este ejemplo
 * también dentro de este ejemplo se va a mencionar a el ejemplo comentado de delayed_busy_wait, así que va dentro de este
 * igualmente lo importante de este ejemplo y el otro va a estar en la función greet, que es donde se producen las esperas activas
 */

// thread_shared_data_t
typedef struct shared_data {
  uint64_t next_thread;
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

void* greet(void* data);
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
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

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->next_thread = 0;
    shared_data->thread_count = thread_count;

    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    error = create_threads(shared_data);

    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    printf("Execution time: %.9lfs\n", elapsed_time);

    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    return 12;
  }
  return error;
}  // end procedure


int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    // se imprime cuando se empiezan a crear
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:

/// @brief En esta función vista en ejemplos pasados es la relevante para este ejemplo, ya que acá se produce la espera activa
/// @param data es una dirección de memoria que se pasa como parámetro, igual que los ejemplos anteriores
/// @return simplemente retorna NULL
void* greet(void* data) {
  /// @note <assert.h> es una biblioteca para pruebas unitarias, entonces esto de assert(data) lo que hace es verificar si data 
  /// es una posición no válida, se rompe la función
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  // Wait until it is my turn
  while (shared_data->next_thread < private_data->thread_number) {
    // busy-waiting
  }  // end while

  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count);

  // Allow subsequent thread to do the task
  ++shared_data->next_thread;

  return NULL;
}  // end procedure