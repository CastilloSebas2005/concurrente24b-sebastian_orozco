// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/** @struct shared_data
 *  @brief Este es el struct que va a contener la memoria compartida, todos los
 * hilos van a poder acceder a este bloque de memoria.
 */
/**@var shared_data::thread_count
 * Esta variable de tipo entero sin signo de 64 bits, es la "memoria compartida"
 * a la que los hilos van a poder acceder, que es la cantidad de hilos totales
 * que se ingresaron, entonces más adelante van a poder estar accediendo a esta
 * información, sin necesidad de que cada hilo tenga que tener este entero esto
 * ayuda a que la memoria del programa se reduzca y sea más amigable el programa
 * en términos de memoria
 */

// thread_shared_data_t
typedef struct shared_data {
  uint64_t thread_count;
} shared_data_t;

/** @struct private_data
 *  @brief Este struct es el mismo que el ejemplo de hello_iw_pr, sin embargo
 * tiene una particularidad que se explicará a continuacións
 */
/** @var private_data::thread_number
 * Esta variable de tipo entero sin signo de 64 bits, es la memoria privada de
 * cada hilo ya que este más adelante es asignado por un for, este es igual que
 * el ejemplo de hello_iw_pr.
 * @var private_data::shared_data
 * Este va a ser un puntero al struct shared_data que es el struct de la memoria
 * compartida, al cual todos los hilos van a tener acceso
 */

// thread_private_data_t
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t *shared_data;
} private_data_t;

void *greet(void *data);
int create_threads(shared_data_t *shared_data);

/// @brief Esta es la función main, se encarga de recibir como argumento la
/// cantidad de hilos a crearse, posteriormente los guarda en el struct
/// shared_data también se encarga de medir el tiempo en segundos en que los
/// hilos tardan en saludar, se explicará a continuación
/// @param argc número de argumentos
/// @param argv arreglo de los argumentos
/// @return retorna un código de error si todo sale bien retorna 0

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

  /// @note shared_data es inicializado con calloc, porque como es una
  /// estructura propia, debe tener su memoria limpia el contenido de
  /// shared_data ya fue documentado anteriormente @see shared_data
  shared_data_t *shared_data =
      (shared_data_t *)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->thread_count = thread_count;

    /// @note se llama al struct timespec que es de la biblioteca <time.h>
    /// y se declara el tiempo inicial(start_time) y el tiempo final(final_time)
    struct timespec start_time, finish_time;

    /// @note Se llama a la función dentro del main::clock_get(CLOCK_MONOTONIC,
    /// &start_time) esto con el fin de obtener el tiempo inicial.
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    /// @note Se llama a la función create_threads(shared_data) shared_data ya
    /// fue inicializado anteriormente notese que el tiempo se mide hasta que se
    /// crean los hilos, no antes, ya que esto no es relevante para el programa.
    error = create_threads(shared_data);

    /// @note Luego de crear los hilos, se vuelve a llamar a
    /// clock_gettime(CLOCK_MONOTONIC, &finish_time), sin embargo, esta vez con
    /// el tiempo final,
    /// posterior a eso se procede a calcular el tiempo en segundos y se muestra
    /// en la salida estándar.

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

/// @brief esta función es la encargada de crear los hilos al igual que en
/// ejemplos pasados
/// @param shared_data este es el struct shared_data que es la memoria en común
/// que van a tener todos lo hilos, como se explicó anteriormente shared_data
/// todo lo que contiene es thread_count que es la cantidad de hilos creados
/// entonces esta función al ser hilo principal también tiene acceso por lo que
/// usa esta función para crear la cantidad de "memorias privadas" de cada hilo
/// y los hilos que serán respectivamente asignados en los ciclos for(la
/// documentación de los for fue hecha en ejemplos pasados) igual, los el for de
/// este ciclo lo único que hace es: asignar al struct private_data el número de
/// hilo creado de la n-esima iteración por la que el ciclo va y asignar a
/// private_data el puntero que tiene de shared_data(este es pasado como
/// parámetro) y además crea los hilos(como se vio anteriormente hay un arreglo
/// de hilos) y les asigna un trabajo(en este caso ir a la función greed)
/// @return retorna un código de error si algo falla, si nada falla retorna 0
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
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
         ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n",
            shared_data->thread_count);
    error = 22;
  }

  return error;
}

/// @brief Esta función ya se ha visto en los ejemplos anteriores, sin embargo
/// esta tiene una particularidad que se explicará a continuación
/// @param data Note que se envía por parámetro un puntero del struct
/// private_data, sin embargo este empaqueta a shared_data que esta es la
/// memoria compartida y todos los private_data lo contienen, entonces de este
/// parámetro se sacan dos por así decirlo se saca el parámetro 1: contiene a
/// private_data se saca el parámetro 2(private_data contiene este) que es el
/// shared_data, este es común para todos los hilos creados
/// @return simplemente retorna NULL

// procedure greet:
void *greet(void *data) {
  // assert(data);
  private_data_t *private_data = (private_data_t *)data;
  shared_data_t *shared_data = private_data->shared_data;

  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n",
         private_data->thread_number, shared_data->thread_count);
  return NULL;
}  // end procedure
