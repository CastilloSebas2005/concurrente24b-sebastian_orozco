// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>

#include <iostream>

/// @brief Programa principal que ejecuta varios for en paralelo con OpenMP
/// @param argc Cantidad de argumentos
/// @param argv Argumentos de la línea de comandos
int main(int argc, char *argv[]) {
  /// @note Cantidad de hilos a utilizar en la ejecución
  int thread_count = omp_get_max_threads();
  /// @note Cantidad de iteraciones a realizar en cada for
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }
  int iteration_count = thread_count;

  /// @note Cantidad de iteraciones a realizar en cada for
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  /// @note Instrucción que crea los hilos
  /// Se crea un bloque paralelo con la cantidad de hilos
  /// Se comparte la cantidad de iteraciones y la salida estándar
#pragma omp parallel num_threads(thread_count) default(none)                   \
    shared(iteration_count, std::cout)
  { // NOLINT
/// @note Se crea un for que se ejecuta en paralelo
/// se usa omp for para que se repartan las iteraciones entre los hilos
#pragma omp for
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
#pragma omp critical(stdout)
      std::cout << "stage 1: " << omp_get_thread_num() << '/'
                << omp_get_num_threads() << ": iteration " << iteration << '/'
                << iteration_count << std::endl;
    }
/// @note Esto es para que solo un hilo del equipo imprima un salto de línea
/// Es decir, para que uno de los hilos ejecute el std::cout << std::endl;
/// Que es el bloque de código que se le está asignando.
/// El omp single tiene una barrera implícita al final.
#pragma omp single
    std::cout << /*omp_get_thread_num() <<*/ std::endl;
// #pragma omp barrier
/// @note Se crea un for que se ejecuta en paralelo
/// se usa omp for para que se repartan las iteraciones entre los hilos
#pragma omp for
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
#pragma omp critical(stdout)
      std::cout << "stage 2: " << omp_get_thread_num() << '/'
                << omp_get_num_threads() << ": iteration " << iteration << '/'
                << iteration_count << std::endl;
    }

/// @note Esto es para que solo un hilo del equipo imprima un salto de línea
/// Se comentó anteriormente
#pragma omp single
    std::cout << /*omp_get_thread_num() <<*/ std::endl;
    // #pragma omp barrier

#pragma omp for
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
      /// @note Sección crítica para imprimir en la salida estándar
      /// Se imprime el número de hilo, la cantidad de hilos, la iteración
      /// actual y la cantidad de iteraciones Se usa la directiva omp critical
      /// para que solo un hilo pueda imprimir a la vez, es decir un mutex
#pragma omp critical(stdout)
      std::cout << "stage 3: " << omp_get_thread_num() << '/'
                << omp_get_num_threads() << ": iteration " << iteration << '/'
                << iteration_count << std::endl;
    }
  }
  /// @note En el último bloque se hace el join de los hilos
}
