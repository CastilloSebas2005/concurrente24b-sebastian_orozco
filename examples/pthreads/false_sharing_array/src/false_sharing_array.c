// Copyright 2021 Allan Berrocal <allan.berrocal@ucr.ac.cr > and
// Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0.
// Adapted from Mr. Jainam M https://github.com/MJjainam/falseSharing

/**
 * @mainpage Este programa lo que hace es comparar el tiempo de ejecución de dos
 * funciones que actualizan dos elementos de un arreglo de 1000 elementos. La
 * primera función actualiza los elementos de manera secuencial y la segunda de
 * manera concurrente. Lo que hace es actualizar dos celdas del arreglo y en
 * cada actualización se hacen 100000000 de modificaciones sobre esas celdas. Se
 * compara el tiempo de ejecución de ambas funciones.
 */

#define _DEFAULT_SOURCE

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/// @brief Numero de elementos en el arreglo
const size_t ELEMENT_COUNT = 1000;
/// @brief Indice del primer elemento
const size_t FIRST_ELEMENT = 0;
/// @brief Indice del segundo elemento
const size_t SECOND_ELEMENT = 1;
/// @brief Indice del ultimo elemento
const size_t LAST_ELEMENT = ELEMENT_COUNT - 1;
/// @brief Numero de iteraciones
const size_t ITERATION_COUNT = 100000000;

/// @brief Descripción de los modos de ejecución
const char *description[] = {
    "sequential(first, second)",
    "sequential(first, last)",
    "concurrent(first, second)",
    "concurrent(first, last)",
};

/// @brief Arreglo de enteros
int *array = NULL;

// Updates an element of the array in a suboptimal manner

/// @brief Actualiza un elemento del arreglo
/// @param data El indice del elemento a actualizar
/// @return NULL
void *update_element(void *data) {
  const size_t my_index = (size_t)data;

  for (size_t iteration = 0; iteration < ITERATION_COUNT; ++iteration) {
    array[my_index] += iteration % ELEMENT_COUNT;
  }

  return NULL;
}

/// @brief Actualiza dos elementos de manera secuencial
/// @param index0 El indice del primer elemento
/// @param index1 El indice del segundo elemento
void run_sequential(size_t index0, size_t index1) {
  /// @note se hacen las 100 millones de actualizaciones sobre el primer
  /// elemento
  update_element((void *)index0);
  /// @note posteriormente se hacen las 100 millones de actualizaciones sobre el
  /// segundo elemento
  update_element((void *)index1);
}

/// @brief Actualiza dos elementos de manera concurrente
/// @param index0 El indice del primer elemento
/// @param index1 El indice del segundo elemento
void run_concurrent(size_t index0, size_t index1) {
  pthread_t thread0, thread1;

  /// @note se crean dos hilos que actualizan los elementos de manera
  /// concurrente pero se está usando seguridad condicional por lo que no está
  /// generando condiciones de carrrera, el hilo 0 actualiza un elemento y el
  /// hilo 1 actualiza el otro
  pthread_create(&thread0, NULL, update_element, (void *)index0);
  pthread_create(&thread1, NULL, update_element, (void *)index1);
  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);
}

/// @brief Función principal del programa que ejecuta las funciones secuencial y
/// concurrente
/// @param argc Cantidad de argumentos
/// @param argv Argumentos
/// @return EXIT_SUCCESS
int main(int argc, char *argv[]) {
  const int mode = argc >= 2 ? atoi(argv[1]) : 0;
  /// @note se crea un arreglo de 1000 elementos
  array = (int *)calloc(ELEMENT_COUNT, sizeof(int));
  /// @note se verifica que el arreglo se haya creado correctamente
  assert(array);
  /// @note se obtiene el tiempo de inicio
  struct timespec start_time, finish_time;
  clock_gettime(CLOCK_REALTIME, &start_time);
  /// @note se ejecuta la función correspondiente al modo
  switch (mode) {
  case 0:
    run_sequential(FIRST_ELEMENT, SECOND_ELEMENT);
    break;
  case 1:
    run_sequential(FIRST_ELEMENT, LAST_ELEMENT);
    break;
  case 2:
    run_concurrent(FIRST_ELEMENT, SECOND_ELEMENT);
    break;
  case 3:
    run_concurrent(FIRST_ELEMENT, LAST_ELEMENT);
    break;
  }

  /// @note se obtiene el tiempo de finalización
  clock_gettime(CLOCK_REALTIME, &finish_time);

  /// @note se calcula el tiempo de ejecución
  double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
                        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

  /// @note se imprime el tiempo de ejecución
  if (mode >= 0 && mode < 4) {
    printf("%.6lfs: %s\n", 1000 * elapsed_time, description[mode]);
  }

  free(array);
  return EXIT_SUCCESS;

  /**
   *  @note El programa tiene 4 modos de ejecución:
   * modo 0: Actualiza los elementos de manera secuencial (first, second), es
   * decir, primero actualiza el primer elemento y luego el segundo del arreglo.
   * modo 1: Actualiza los elementos de manera secuencial (first, last), es
   * decir, primero actualiza el primer elemento y luego el último del arreglo.
   * modo 2: Actualiza los elementos de manera concurrente (first, second), es
   * decir, actualiza con un hilo el primer elemento y con otro hilo el segundo
   * del arreglo. modo 3: Actualiza los elementos de manera concurrente (first,
   * last), es decir, actualiza con un hilo el primer elemento y con otro hilo
   * el último del arreglo.
   */
}
