// Copyright 2024 ECCI-UCR CC-BY 4.0

#include <omp.h>

#include <iostream>

/// @mainpage hay una directiva de omp que se encarga de crear un equipo de
/// hilos, sin embargo, para este ejemplo no se usó debido a que solo crea la
/// cantidad de cpu's que hayan en el sistema, la directiva de la creación de
/// se llama #pragma omp teams, como se dijo, no se usó en este ejemplo.

/// @brief Función principal
/// @param argc Número de argumentos
/// @param argv Argumentos
int main(int argc, char *argv[]) {
  /// @brief team_count: cantidad de hilos
  /// Si argc es mayor o igual a 2, se convierte el segundo argumento a entero
  /// Si no, se obtiene la cantidad de procesadores disponibles en el sistema
  const int team_count = argc >= 2 ? ::atoi(argv[1]) : omp_get_num_procs();

/// @note omp parallel num_threads(team_count) default(none) shared(std::cout,
/// team_count) Es la directiva que se encarga de crear el equipo de hilos en
/// este caso se le indica la cantidad de hilos que se desea y se le indica que
/// se comparta la salida estándar y la cantidad de hilos en este caso
/// team_count para que todos los hilos puedan acceder a la variable para este
/// ejemplo la shared_data es la salida estándar y la cantidad de hilos no hay
/// private_data
#pragma omp parallel num_threads(team_count) default(none)                     \
    shared(std::cout, team_count) // NOLINT

/// @note omp critical se encarga de que solo un hilo pueda ejecutar el bloque
/// de código a la vez, funciona como un mutex, no importa que hilo acceda, esto
/// se usa solamente para que no se impriman los mensajes de manera caótica,
/// pero el hilo 10 puede imprimir antes que el hilo 1
#pragma omp critical
  {
    std::cout << "Hello from thread " << omp_get_thread_num() << '/'
              << omp_get_num_threads() << std::endl;
  }
}
