// Copyright [2024] <Sebastián Orozco>

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "controller.h"

/** @mainpage This program is a simulation of heat transfer
 * 
 */

/**
 @brief This is the main procedure, this is to validate the argument input is
 correct and call the principal function of program.
 @param argc Number of arguments.
 @param argv Array of elements(chars).
 @return An error code:
   0 for success.
   1 if numbers of arguments are 3 or less, no exist arguments.
   2 if a numbers of arguments are more than 3.
*/

int main(int argc, char *argv[]) {
  int error = 0;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int rank = -1;
    int rank_count = -1;
    if (argc == 5) {
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      MPI_Comm_size(MPI_COMM_WORLD, &rank_count);
      printf("Welcome\n"); // NOLINT
      init_controller(argv, rank, rank_count);
    } else if (argc <= 4) {
      fprintf(stderr, "Error: this program needs four arguments to work, no less\n"); // NOLINT
      error = 1;
    } else if (argc > 4) {
      fprintf(stderr, "Error: this program needs four arguments, no more\n"); // NOLINT
      error = 2;
    }
    MPI_Finalize();
  } else {
    fprintf(stderr, "Error: could not initialize MPI\n"); // NOLINT
    error = 3;
  }
  return error;
}
