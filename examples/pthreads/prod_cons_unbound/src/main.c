// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// Simulates a producer and a consumer that share a unbounded buffer

/// @mainpage Solo se va a documentar lo nuevo de este código
/// lo de ejemplos pasados no se va a documentar


/**
 * @mainpage Me gustaría decir que lo que cambia en las
 * diferentes versiones de los ejemplos es la implementación de la cola
 * y la forma en que se manejan los productores y consumidores. Lo que tiene
 * este ejemplo es que se maneja una cola sin límite de tamaño, no como el anterior
 * que tenía un límite de tamaño. Ya que se usaban arreglos en vez de una cola.
 */
#include <stdlib.h>
#include <stdio.h>

#include "simulation.h"

/// @brief Función principal
/// @param argc Cantidad de argumentos
/// @param argv Argumentos
/// @return error
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  /// @note Este es un objeto controlador de la simulación
  simulation_t* simulation = simulation_create();
  if (simulation) {
    error = simulation_run(simulation, argc, argv);
    simulation_destroy(simulation);
  } else {
    fprintf(stderr, "error: could not allocate simulation\n");
    error = EXIT_FAILURE;
  }
  return error;
}
