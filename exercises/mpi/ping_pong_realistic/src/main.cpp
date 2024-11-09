// Copyright 2024 [Sebastian Orozco Castillo]

#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <random>
#include <stdexcept>
#include <unistd.h>

/// @brief This function is going to do the work
/// @param process_rank Number of process
/// @param argv Arguments by the user
void ping_pong(int process_rank, char *argv[]);

/// @brief Main function
/// @param argc Count of arguments
/// @param argv Arguments by the user
/// @return error variable
int main(int argc, char *argv[]) {
  int error = EXIT_SUCCESS;
  if (argc == 3) {
    if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
      int process_number = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

      int process_count = -1;
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);

      if (process_count != 2) {
        std::cerr << "READ THE README, CAN'T MAKE THAT COUNT OF PROCCESS" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
      }

      try {
        ping_pong(process_number, argv);
      } catch (const std::runtime_error &exception) {
        std::cerr << "error: " << exception.what() << std::endl;
        error = EXIT_FAILURE;
      }

      MPI_Finalize();
    } else {
      std::cerr << "error: could not init MPI" << std::endl;
      error = EXIT_FAILURE;
    }
    return error;
  }
  error = EXIT_FAILURE;
  std::cerr << "Your arguments are incorrect, read the readme!!!!" << std::endl;
  return error;
}

void ping_pong(int process_rank, char *argv[]) {
  int services = 0;
  int score[] = {0, 0};
  int roundsWining[] = {0, 0};
  int round = 0;
  /// to decides probability
  float probability = process_rank ? std::atof(argv[3]) : std::atof(argv[2]);
  /// Random number generator setup
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(1.0, 100.0);

  /// Initialice the game
  if (process_rank == 0) {
    services++;
    MPI_Send(&services, 1, MPI_INT, !process_rank, 0, MPI_COMM_WORLD);
    MPI_Send(&score, 2, MPI_INT, !process_rank, 0, MPI_COMM_WORLD);
    MPI_Send(&round, 1, MPI_INT, !process_rank, 0, MPI_COMM_WORLD);
    MPI_Send(&roundsWining, 2, MPI_INT, !process_rank, 0, MPI_COMM_WORLD);
  }

  /// Victory condition
  while (score[0] < std::atoi(argv[1]) && score[1] < std::atoi(argv[1])) {
    MPI_Recv(&services, 1, MPI_INT, !process_rank, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(&score, 2, MPI_INT, !process_rank, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(&round, 1, MPI_INT, !process_rank, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(&roundsWining, 2, MPI_INT, !process_rank, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    float random_number = dis(gen);

    services++;

    if (random_number <= probability) {
      score[process_rank]++;
    } else {
      round++;
      roundsWining[!process_rank]++;
      usleep(7000);
      std::cout << round << ": " << services << " " << !process_rank
                << std::endl;
    }

    MPI_Send(&services, 1, MPI_INT, !process_rank, 0, MPI_COMM_WORLD);
    MPI_Send(&score, 2, MPI_INT, !process_rank, 0, MPI_COMM_WORLD);
    MPI_Send(&round, 1, MPI_INT, !process_rank, 0, MPI_COMM_WORLD);
    MPI_Send(&roundsWining, 2, MPI_INT, !process_rank, 0, MPI_COMM_WORLD);
  }
  if (process_rank == 1) {
    if (roundsWining[0] > roundsWining[1]) {
      std::cout << "0 wins " << roundsWining[0] << " to " << roundsWining[1]
                << std::endl;
    } else if (roundsWining[1] > roundsWining[0]) {
      std::cout << "1 wins " << roundsWining[1] << " to " << roundsWining[0]
                << std::endl;
    } else {
      std::cout << "It's a tie " << roundsWining[1] << " to " << roundsWining[0]
                << std::endl;
    }
  }
}
