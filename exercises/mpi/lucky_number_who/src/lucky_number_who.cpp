// Copyright [2024] <Sebastian Orozco>

#include <mpi.h>

#include <iostream>
#include <stdexcept>

#include "UniformRandom.hpp"

#define fail(msg) throw std::runtime_error(msg)

/// @brief Generates lucky numbers and statistics for each process
/// in this MPI program it uses the MPI_Allreduce function to
/// calculate the minimum, maximum, sum and average of the lucky numbers
/// for all processes.
/// @param process_number
/// @param process_count
void generate_lucky_statistics(int process_number, int process_count);

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

      generate_lucky_statistics(process_number, process_count);
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

void generate_lucky_statistics(int process_number, int process_count) {
  // Generate my lucky number
  UniformRandom<int> uniformRandom(process_number);
  const int my_lucky_number = uniformRandom.between(0, 100);

  std::cout << "Process " << process_number << ": my lucky number is "
            << my_lucky_number << std::endl;

  int all_min = -1;
  int all_max = -1;
  int all_sum = -1;

  // Update distributed statistics from processes' lucky numbers
  // this uses the minium param to get the minimum value
  if (MPI_Allreduce(/*input*/ &my_lucky_number, /*output*/ &all_min,
                    /*count*/ 1, MPI_INT, MPI_MIN,
                    MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce min");
  }
  if (MPI_Allreduce(/*input*/ &my_lucky_number, /*output*/ &all_max,
                    /*count*/ 1, MPI_INT, MPI_MAX,
                    MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce max");
  }
  if (MPI_Allreduce(/*input*/ &my_lucky_number, /*output*/ &all_sum,
                    /*count*/ 1, MPI_INT, MPI_SUM,
                    MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce sum");
  }

  const double all_average = static_cast<double>(all_sum) / process_count;

  // this to report the statistics of the lucky numbers in specific process
  // but note that the statistics are calculated for all processes
  // in this case the proccess 0 not necessarily has the minimum value
  // but other process could have the minimum value and report it
  if (my_lucky_number == all_min) {
    std::cout << "Process " << process_number << ": my lucky number ("
              << my_lucky_number << ") is the minimum (" << all_min << ")"
              << std::endl;
  }

  if (my_lucky_number < all_average) {
    std::cout << "Process " << process_number << ": my lucky number ("
              << my_lucky_number << ") is less than the average ("
              << all_average << ")" << std::endl;
  } else if (my_lucky_number > all_average) {
    std::cout << "Process " << process_number << ": my lucky number ("
              << my_lucky_number << ") is greater than the average ("
              << all_average << ")" << std::endl;
  } else {
    std::cout << "Process " << process_number << ": my lucky number ("
              << my_lucky_number << ") is equal to the average (" << all_average
              << ")" << std::endl;
  }

  if (my_lucky_number == all_max) {
    std::cout << "Process " << process_number << ": my lucky number ("
              << my_lucky_number << ") is the maximum (" << all_max << ")"
              << std::endl;
  }
}
