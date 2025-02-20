// Copyright [2024] <Sebastián Orozco>

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/** falta la documentación, implementar golpes, implementar la creación de
 * hilos, pasar por valgrind, etc... hacer lint
 *
 */

// thread_shared_data_t
typedef struct shared_data {
  // cantidad de hilos totales
  uint64_t thread_count;
  // cantidad de golpes que aguanta la piñata
  uint64_t cantidad_golpes;
  // identificador para saber si la piñata está rota
  uint8_t rota;
  // este es el mutex para saber si se puede golpear en ese momento o no
  pthread_mutex_t puede_golpear;
} shared_data_t;

// thread_private_data
typedef struct private_data {
  // identificador del hilo
  uint64_t numero_hilo;
  // todos tienen acceso a la memoria compartida
  shared_data_t *shared_data;
} private_data_t;

// función para hacer la creación de los hilos
int create_threads(shared_data_t *shared_data);
// función que manda a los hilos a trabajar, en este caso golpear la piñata
void *golpear_pinata(void *data);

int main(int argc, char *argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  uint64_t cantidad_golpes = 0;
  if (argc == 3) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: thread_count inválido\n");
      return 11;
    }
    // Es para capturar los datos de la dureza de la piñata
    if (sscanf(argv[2], "%" SCNu64, &cantidad_golpes) == 1) {
    } else {
      fprintf(stderr, "Error: cantidad de fuerza inválida\n");
      return 11;
    }
  }
  shared_data_t *shared_data =
      (shared_data_t *)calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    shared_data->cantidad_golpes = cantidad_golpes;
    shared_data->rota = 1;
    // se inicia el mutex para evitar que los hilos le peguen todos a la vez
    error = pthread_mutex_init(&shared_data->puede_golpear, NULL);
    if (error == EXIT_SUCCESS) {
      shared_data->thread_count = thread_count;
      error = create_threads(shared_data);
    }
  }

  free(shared_data);
  return error;
}

/**
 * Esta función es la misma que se ve en los ejemplos comentados, no hay nada
 * relevante que documentar acá
 */
int create_threads(shared_data_t *shared_data) {
  int error = EXIT_SUCCESS;
  // continuar con la creación de hilos
  pthread_t *thread =
      (pthread_t *)malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t *private_data = (private_data_t *)(calloc(
      shared_data->thread_count, sizeof(private_data_t)));
  // si son válidos
  if (thread && private_data) {
    for (uint64_t numero_hilo = 0; numero_hilo < shared_data->thread_count;
         ++numero_hilo) {
      private_data[numero_hilo].numero_hilo = numero_hilo;
      private_data[numero_hilo].shared_data = shared_data;
      // se crean los hilos y se les asigna su función
      error = pthread_create(&thread[numero_hilo], NULL, golpear_pinata,
                             &private_data[numero_hilo]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
         ++thread_number) {
      pthread_join(thread[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);
    free(thread);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n",
            shared_data->thread_count);
    error = 22;
  }

  return error;
}

void *golpear_pinata(void *data) {
  assert(data);
  private_data_t *private_data = (private_data_t *)data;
  shared_data_t *shared_data = private_data->shared_data;
  // se bloque el mutex para evitar que varios hilos le peguen a la piñata a la
  // vez
  pthread_mutex_lock(&shared_data->puede_golpear);
  // Calcular la cantidad de golpes que puede dar cada hilo
  uint64_t base_golpes =
      shared_data->cantidad_golpes / shared_data->thread_count;
  uint64_t golpes_restantes =
      shared_data->cantidad_golpes % shared_data->thread_count;

  // Se crea la semilla
  unsigned int seed = (unsigned int)time(NULL) + private_data->numero_hilo;
  uint64_t cantidad_golpes = base_golpes;
  // si los golpes restantes son mayores que cero se asigna una cantidad
  // aleatoria de golpes
  if (golpes_restantes > 0) {
    cantidad_golpes += rand_r(&seed) % (golpes_restantes + 1);
    golpes_restantes -= cantidad_golpes - base_golpes;
  }

  // el último hilo es el encargado de reventar la piñata
  if (shared_data->thread_count - 1 == private_data->numero_hilo) {
    cantidad_golpes = shared_data->cantidad_golpes;
    shared_data->cantidad_golpes -= cantidad_golpes;
  } else {
    // si no, simplemente se le resta la cantidad de golpes a la piñata
    shared_data->cantidad_golpes -= cantidad_golpes;
  }
  // se imprime el hilo que rompió la piñata
  if (shared_data->cantidad_golpes == 0 && shared_data->rota == 1) {
    printf(
        "Thread %" PRIu64 "/%" PRIu64 " %" PRIu64 " hits, broke the pinata\n",
        private_data->numero_hilo, shared_data->thread_count, cantidad_golpes);
    // si la piñata es rota se le asigna un valor de 0
    // si la piñata aún no está rota se tiene un valor 1 que se definió
    // anteriormente
    shared_data->rota = 0;
  } else {
    // se imprime el hilo y la cantidad de golpes
    printf("Thread %" PRIu64 "/%" PRIu64 " %" PRIu64 " hits\n",
           private_data->numero_hilo, shared_data->thread_count,
           cantidad_golpes);
  }
  // se desbloquea el mutex
  pthread_mutex_unlock(&shared_data->puede_golpear);
  return NULL;
}
