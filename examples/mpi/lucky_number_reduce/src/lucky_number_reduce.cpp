#include <mpi.h>
#include <iostream>
#include <stdexcept>

#include "UniformRandom.hpp"

#define fail(msg) throw std::runtime_error(msg)

/// @brief Generates lucky numbers and calculates statistics
/// and extracts the minimum, maximum, and average of the lucky numbers
/// from all processes using MPI_Reduce
/// @param process_number number of the process or rank
/// @param process_count total number of processes or ranks
void generate_lucky_statistics(int process_number, int process_count);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    try {
      int process_number = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

      int process_count = -1;
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);

      char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostname_length = -1;
      // 
      MPI_Get_processor_name(process_hostname, &hostname_length);

      generate_lucky_statistics(process_number, process_count);
    } catch (const std::runtime_error& exception) {
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
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_min, /*count*/ 1
    , MPI_INT, MPI_MIN, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce min");
  }
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_max, /*count*/ 1
    , MPI_INT, MPI_MAX, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce max");
  }
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_sum, /*count*/ 1
    , MPI_INT, MPI_SUM, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce sum");
  }

  if (process_number == 0) {
    const double all_average = double(all_sum) / process_count;
    std::cout << "Process " << process_number << ": all minimum = "
      << all_min << std::endl;
    std::cout << "Process " << process_number << ": all average = "
      << all_average << std::endl;
    std::cout << "Process " << process_number << ": all maximum = "
      << all_max << std::endl;
  }
}
