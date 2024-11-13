// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define fail(msg) throw std::runtime_error(msg)

/// @brief Permite simular una carrera de relevos
/// @param argc Cantidad de argumentos
/// @param argv Argumentos
/// @param process_number Número de proceso
/// @param process_count Cantidad de procesos
void simulate_relay_race(int argc, char *argv[], int process_number,
                         int process_count);

/// @brief Primera etapa de la carrera de relevos
/// @param stage1_delay Tiempo de espera en milisegundos
/// @param process_number Número de proceso
/// @param team_count Cantidad de equipos
void run_stage1(int stage1_delay, int process_number, int team_count);

/// @brief Segunda etapa de la carrera de relevos
/// @param stage2_delay Tiempo de espera en milisegundos de la primera etapa
/// @param process_number Número de proceso
/// @param team_count Cantidad de equipos
void run_stage2(int stage2_delay, int process_number, int team_count);

/// @brief Función que se encarga de recibir los mensajes de los equipos
/// @param team_count Cantidad de equipos
void referee(int team_count);

/// @brief Función principal
/// @param argc Cantidad de argumentoss
/// @param argv Argumentos
/// @return Código de salida
int main(int argc, char *argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    try {
      int process_number = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

      int process_count = -1;
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);

      char process_hostname[MPI_MAX_PROCESSOR_NAME] = {'\0'};
      int hostname_length = -1;
      MPI_Get_processor_name(process_hostname, &hostname_length);

      simulate_relay_race(argc, argv, process_number, process_count);
    } catch (const std::runtime_error &exception) {
      std::cout << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void simulate_relay_race(int argc, char *argv[], int process_number,
                         int process_count) {
  // Si el número de argumentos es 3 entonces se ejecuta el código
  if (argc == 3) {
    // Si el número de procesos es mayor o igual a 3 y es impar
    // entonces se ejecuta el código
    if (process_count >= 3 && process_count % 2 == 1) {
      // Se calcula el número de equipos
      const int team_count = (process_count - 1) / 2;
      // Se obtienen los valores de los argumentos
      const int stage1_delay = atoi(argv[1]);
      // Se obtiene el valor del segundo argumento
      const int stage2_delay = atoi(argv[2]);
      // Si el proceso es el 0 entonces se ejecuta el código del referee
      if (process_number == 0) {
        referee(team_count);
        // Si el proceso es menor o igual al número de equipos entonces
        // se ejecuta el código de la etapa 1
      } else if (process_number <= team_count) {
        run_stage1(stage1_delay, process_number, team_count);
        // Si no se ejecuta el código de la etapa 2
      } else {
        run_stage2(stage2_delay, process_number, team_count);
      }
    } else {
      fail("error: process count must be odd and greater than 3");
    }
  } else {
    fail("usage: relay_race_dist stage1_delay stage2_delay");
  }
}

void run_stage1(int stage1_delay, int process_number, int team_count) {
  // Se espera a que todos los procesos lleguen a este punto
  // si no se envía un mensaje de error
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could wait for barrier");
  }
  // Se espera un tiempo en milisegundos
  usleep(1000 * stage1_delay);
  // Se calcula el número del proceso que se va a enviar el mensaje
  const int peer = process_number + team_count;
  // Se batón se inicializa en true
  bool baton = true;
  // send(&baton, 1, peer)

  // Se envía el mensaje al proceso peer con el valor de baton
  if (MPI_Send(&baton, /*count*/ 1, MPI_C_BOOL, peer, /*tag*/ 0,
               MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not send baton");
  }
}

void run_stage2(int stage2_delay, int process_number, int team_count) {
  // Se espera a que todos los procesos lleguen a este punto
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could wait for barrier");
  }
  // Se calcula el número del proceso que se va a enviar el mensaje
  int peer = process_number - team_count;
  // Se inicializa el valor de baton en false
  bool baton = false;
  // receive(&baton, 1, peer)
  // Se recibe el mensaje del proceso peer con el valor de baton
  if (MPI_Recv(&baton, /*capacity*/ 1, MPI_C_BOOL, /*source*/ peer, /*tag*/ 0,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
    fail("could not receive baton");
  }
  // Se espera un tiempo en milisegundos
  usleep(1000 * stage2_delay);
  // send(&peer, 1, 0)
  // Se envía el mensaje al proceso 0 con el valor de peer
  if (MPI_Send(&peer, /*count*/ 1, MPI_INT, /*target*/ 0, /*tag*/ 0,
               MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not send team number to referee");
  }
}

void referee(int team_count) {
  // Se inicializa el tiempo de inicio
  const double start_time = MPI_Wtime();
  // wait_barrier()
  // Se espera a que todos los procesos lleguen a este punto
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could wait for barrier");
  }
  // Se inicializa el valor de place en 0
  int place = 0;
  // Se recorre el número de equipos
  for (int index = 0; index < team_count; ++index) {
    int team = 0;
    // receive(&team, 1, any_process)
    // Se recibe el mensaje de cualquier proceso con el valor de team
    if (MPI_Recv(&team, /*capacity*/ 1, MPI_INT, MPI_ANY_SOURCE, /*tag*/ 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      fail("error: could not receive team number");
    }
    // Se calcula el tiempo transcurrido
    const double elapsed = MPI_Wtime() - start_time;
    ++place;
    // Se imprime el lugar, el equipo y el tiempo transcurrido
    std::cout << "Place " << place << ": team " << team << " in " << elapsed
              << "s" << std::endl;
  }
}
