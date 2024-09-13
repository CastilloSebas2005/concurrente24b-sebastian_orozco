// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef SIMULATION_H
#define SIMULATION_H

/// @brief Registro de simulación opaco
typedef struct simulation simulation_t;  // opaque record

/// @brief Crea un registro de simulación
/// @return Dirección de memoria del registro de simulación
simulation_t *simulation_create();

/// @brief Ejecuta la simulación
/// @param simulation Objeto de simulación
/// @param argc Cantidad de argumentos
/// @param argv Argumentos
/// @return variable de error
int simulation_run(simulation_t *simulation, int argc, char *argv[]);

/// @brief Destruye un registro de simulación, para liberar memoria
/// correctamente
/// @param simulation Objeto de simulación
void simulation_destroy(simulation_t *simulation);

#endif  // SIMULATION_H
