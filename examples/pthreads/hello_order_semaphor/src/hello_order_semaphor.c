// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
/// @note este <semaphore.h> es lo que vamos a usar para la implementación de
/// semáforos en este código
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// thread_shared_data_t

/// @struct shared_data
/// @brief Estructura de datos compartidos, lo que quiero hacer
/// notar es una variable
/** @var shared_data::can_greet
 * esta variable es de tipo sem_t, es usada para las operaciones con
 * los semáforos. En este caso es un puntero porque no se sabe el
 * tamaño que va a tomar el arreglo de semáforos. Este tamaño se
 * decidirá en tiempo de ejecución, por eso tiene que ser un puntero,
 * se necesita que sea dinámica.
 */
typedef struct shared_data {
  sem_t* can_greet;
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t

/// @struct private_data
/// @brief Ya vista en ejemplos anteriores
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief ...
 */
void* greet(void* data);
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])

/// @brief De esta subrutina solo voy a destacar la inicialización
/// del semáforo, lo demás fue visto en ejemplos comentados
/// @param argc Número de argumetos
/// @param argv Argumentos
/// @return la variable error o 11(en caso de que algo salga mal)
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
    /** @note Lo que quiero hacer notar de esta subrutina es lo siguiente
     *  de este ejemplo es que se inicializa la variable del struct shared_data
     *  de tipo sem_t con un calloc, ya que necesitamos crear un arreglo de semáforos
     *  para esta solución, se pide con calloc ya que necesitamos que la memoria sea
     *  limpia.
     */
    shared_data->can_greet = (sem_t*) calloc(thread_count, sizeof(sem_t));
    shared_data->thread_count = thread_count;
    /** @note Luego de crear el arreglo de sem_t y reservar la memoria,
     * se procede a inicializar cada uno de los sem_t con su respectiva función
     * de int sem_init(sem_t *__sem, int __pshared, unsigned int __value)
     * esta función recibe: 
     * 1. La direción de memoria de sem_t
     * 2. Pshared basicamente es si este semáforo es compartido entre procesos
     * en este caso no, por eso se le envía un 0(false) como parámetro.
     * 3. el value va ser el valor de inicialización de cada semáforo.
     * note que se niega thread_number con el operador ! como en el pseudo código:
     * can_greet[thread_number] := create_semaphore(not thread_number)
     */
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      // can_greet[thread_number] := create_semaphore(not thread_number)
      error = sem_init(&shared_data->can_greet[thread_number], /*pshared*/ 0
        , /*value*/ !thread_number);
    }

    /** @note posteriormente a la creación de semáforos
     * se verifica que se hayan creado correctamente(que no sea una dirección nula)
     * y se procede a medir el tiempo y crear los hilos.
     */
    if (shared_data->can_greet) {
      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

      printf("Execution time: %.9lfs\n", elapsed_time);

      /// @note free(shared_data->can_greet); libera al arreglo de
      /// semáforos creados
      free(shared_data->can_greet);
    } else {
      fprintf(stderr, "Error: could not allocate semaphores\n");
      error = 13;
    }
    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }
  return error;
}  // end procedure

/// @brief Igual que funciones anteriores, no posee nada relevante
/// respecto a los semáforos.
/// @param shared_data Memoria compartida que contiene el arreglo de semáforos
/// @return variable de error, 1 si todo salió bien
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
      if (error == EXIT_SUCCESS) {
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

      } else {
        fprintf(stderr, "Error: could not init semaphore\n");
        error = 22;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
      sem_destroy(&shared_data->can_greet[thread_number]);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 23;
  }

  return error;
}

// procedure greet:

/// @brief Es la función de saludar visto en ejemplos pasados
/// solo se comentará lo de semáforos
/// @param data en este caso es la memoria privada
/// @return NULL
void* greet(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  // Wait until it is my turn
  // wait(can_greet[thread_number])
  /** @note sem_wait()
   *  Acá lo que se hace es esperar al semáforo
   *  note que antes lo que había acá era la espera activa.
   * 
   */
  int error = sem_wait(&shared_data->can_greet[private_data->thread_number]);
  if (error) {
    fprintf(stderr, "error: could not wait for semaphore\n");
  }

  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count);

  // Allow subsequent thread to do the task
  // signal(can_greet[(thread_number + 1) mod thread_count])
  /** @note sem_post() Es lo mismo que decir signal y se incrementa el siguiente semáforo
   *  en otras palabras, esto recibe al semáforo que se desea esperar
   * next_thread simplemente es la lógica implementada para esperar al hilo siguiente
   * sin acceder a una posición inválida, por eso se usa el operador módulo
   */
  const uint64_t next_thread = (private_data->thread_number + 1)
    % shared_data->thread_count;
  error = sem_post(&shared_data->can_greet[next_thread]);
  if (error) {
    fprintf(stderr, "error: could not increment semaphore\n");
  }

  return NULL;
}  // end procedure
