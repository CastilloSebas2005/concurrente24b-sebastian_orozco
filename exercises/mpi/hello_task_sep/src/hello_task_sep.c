// Copyright [2024] <Sebastian Orozco Castillo>

#include <mpi.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/// @brief Main function of the program
/// @param argc Number of arguments
/// @param argv Arguments
/// @return 0 if the program ends successfully, 1 otherwise
int main(int argc, char **argv) {
  /// Initialize MPI
  MPI_Init(&argc, &argv);

  /// Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  /// validate the number of processes
  if (world_size != 2) {
    if (world_size > 0) {
      fprintf(stderr,
              "Error: Este programa requiere exactamente 2 procesos.\n");
    }
    MPI_Abort(MPI_COMM_WORLD, 1);
    return 1;
  }

  /// Get the rank of the process
  int proccess = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &proccess);

 

  /// Proccess 0 prints a message
  if (proccess == 0) {
    printf("Hola desde el proceso 0\n"); // NOLINT
    /// process 1 generates a random number
  } else if (proccess == 1) {
    /// Seed for the random number
    unsigned int seed = (unsigned int)time(NULL);
    int random_number = rand_r(&seed) % 11;
    printf("Número aleatorio desde el proceso 1: %d\n", random_number); // NOLINT
  }
  /// END PROGRAM
  MPI_Finalize();
  return 0;
}
