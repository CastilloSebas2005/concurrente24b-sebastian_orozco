// Copyright 2024 ECCI-UCR CC-BY 4.0
#include <omp.h>

#include <iostream>

/// @brief Función principal
/// @param argc Cantidad de argumentos
/// @param argv Argumentos
int main(int argc, char *argv[]) {
  /**
   * thread_count: cantidad de hilos
   * iteration_count: número de itración
   */
  const int thread_count = argc >= 2 ? ::atoi(argv[1]) : omp_get_num_procs();
  const int iteration_count = argc >= 3 ? ::atoi(argv[2]) : thread_count;

/// @note omp parallel for num_threads(thread_count) schedule(dynamic, 2)
/// default(none) shared(std::cout, thread_count, iteration_count)
/// Es la directiva que se encarga de paralelizar el for
/// es decir crea hilos y los distribuye en la cantidad de hilos
/// que se le indique en este caso thread_count y se encarga de
/// distribuir las iteraciones de manera dinámica en este caso
/// se le indica que se distribuyan de 2 en 2
#pragma omp parallel for num_threads(thread_count)                             \
    schedule(dynamic, 2) default(none)                                         \
        shared(std::cout, thread_count, iteration_count) // NOLINT

  for (int iteration = 0; iteration < iteration_count; ++iteration) {
/// @note omp critical
/// En este caso se utiliza la directiva omp critical
/// para que solo un hilo pueda ejecutar el bloque de código
/// a la vez, funciona como un mutex, no importa que hilo acceda
/// entonces sí hay indeterminismo en el acceso a la variable
#pragma omp critical
    {
      std::cout << omp_get_thread_num() << '/' << thread_count << ": iteration "
                << iteration << '/' << iteration_count << std::endl;
    }
  }
}
