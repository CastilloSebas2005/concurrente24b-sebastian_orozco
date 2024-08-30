// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// thread_private_data_t
/** @struct private_data
 *  @brief Esta es una estructura de datos o registro hecha para dar un ejemplo
 * de lo que es memoria privada en el concepto de hilos, es decir, que otros
 * hilos no pueden acceder a esta sin permiso. Solamente puede acceder a los
 * datos el hilo designado.
 */

/**@var private_data::thread_number
 * Esta variable guarda el numero de hilo asignado por un for.
 * @var private_data::thread_count
 * Esta variable guarda el total del numero de hilos (pasados por argumento
 * cuando se ejecuta el archivo).
 * @var private_data::next
 * Esta variable a mi parecer es irrelevante en éste código, no se está usando,
 * de hecho la borré y el código se ejecuta igual no da warnings ni nada.
 */

typedef struct private_data {
  uint64_t thread_number;  // rank
  uint64_t thread_count;
  // puntero a estructura de datos privados
  struct private_data *next;
} private_data_t;

void *greet(void *data);

int create_threads(uint64_t thread_count);

// procedure main(argc, argv[])

/// @brief Es la función main, se encarga de recibir los argumentos, validar que
/// estén bien y pasarlos a create_threads.
/// @param argc cantidad de argumentos que se pasaron.
/// @param argv cantidad de hilos que se pasan.
/// @return retorna error o 0 si todo sale bien.
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

/// @brief Función encargada de crear los hilos.
/// @param thread_count Cantidad de hilos que posteriormente son usados con
/// malloc para hacer el arreglo de hilos y el arreglo de estructuras privadas.
/// @return retorna los errores respectivos o 0 si todo sale bien.
/** @note create_threads::threads
 * Con el parámetro que se recibe(el parametro es la cantidad de elementos que
 * se van a crear en el arreglo) se crea un arreglo de tipo pthreads_t se llama
 * a malloc y se multiplica el espacio que ocupa el tipo * cantidad de hilos
 * simplemente es un arreglo de hilos.
 */
/** @note create_threads::private_data_t
 * Con el parámetro que se recibe(el parametro es la cantidad de elementos que
 * se van a crear en el arreglo) se crea un arreglo de tipo private_data se
 * llama a calloc y se multiplica el espacio que ocupa el tipo * cantidad de
 * hilos(ya que en este caso se ocupan de igual cantidad que los hilos)
 * simplemente es un arreglo de private_data, notese que se llama a calloc
 * porque como es una estructura de datos creada por nosotros mismos se necesita
 * que esta memoria con la que se crea sea "limpia".
 */

/** @note A las variables create_threads::private_data y
 * create_threads::threads se van a inicializar durante
 * un ciclo for, primero se creará la estructura de datos privada con todo lo
 * documentado anteriormente posteriormente se le asignará a un hilo y luego se
 * llamará a la función greet.
 */
int create_threads(uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
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

/// @brief Esta es una función que recibe una estructura de datos privada, tiene
/// la firma de pthreads por lo tanto es invocada por un hilo y lo que hace es
/// imprimir un saludo dicho saludo contiene: el número de hilo desde el que se
/// saluda y la cantidad de hilos ingresada
/// @param data Parámetro tipo void se castea el parametro a private_data_t
/// @return Simplemente retorna el valor de NULL que es 0
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
