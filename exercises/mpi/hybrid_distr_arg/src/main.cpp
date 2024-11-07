// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>

/// @brief Calculate the start index for a process
/// @param rank Rank of the process
/// @param end End index
/// @param workers Number of processes
/// @param begin Begin index
/// @return Start index for the process
int calculate_start(int rank, int end, int workers, int begin);

/// @brief Calculate the finish index for a process
/// @param rank Rank of the process
/// @param end End index
/// @param workers Number of processes
/// @param begin Begin index
/// @return Finish index for the process
int calculate_finish(int rank, int end, int workers, int begin);

/// @brief Main function
/// @param argc Count of arguments
/// @param argv Array of arguments
/// @return Execution status
int main(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;  // rank

    /// Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;

    /// Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    /// Get the hostname of the process
    char process_hostname[MPI_MAX_PROCESSOR_NAME];

    int hostname_length = -1;

    /// Get the hostname length
    MPI_Get_processor_name(process_hostname, &hostname_length);

    if (argc == 3) {
      /// Get the start and finish values
      const int overall_start = atoi(argv[1]);
      const int overall_finish = atoi(argv[2]);

      /// Calculate the range for the process
      const int process_start = calculate_start(process_number, overall_finish
        , process_count, overall_start);
      const int process_finish = calculate_finish(process_number, overall_finish
        , process_count, overall_start);
      /// Calculate the size of the range
      const int process_size = process_finish - process_start;

      /// Print the range for the process
      std::cout << process_hostname << ':' << process_number << ": range ["
        << process_start << ", " << process_finish << "[ size " << process_size
        << std::endl;

      /// Create threads to process the range
      #pragma omp parallel default(none) shared(std::cout, process_hostname) \
        shared(process_number, process_start, process_finish)
      {  // NOLINT(whitespace/braces)
        int thread_start = -1;
        int thread_finish = -1;

        /// Map the threads to the range of the process
        /// In this case the threads are mapped statically
        #pragma omp for schedule(static)
        for (int index = process_start; index < process_finish; ++index) {
          if (thread_start == -1) {
            thread_start = index;
          }
          thread_finish = index;
        }

        ++thread_finish;
        const int thread_size = thread_finish - thread_start;

        #pragma omp critical(can_print)
        std::cout << '\t' << process_hostname << ':' << process_number << '.'
          << omp_get_thread_num() << ": range [" << thread_start << ", " <<
          thread_finish << "[ size " << thread_size << std::endl;
      }
    } else {
      std::cerr << "usage: hybrid_distr_arg start finish" << std::endl;
    }

    MPI_Finalize();
  }

  return 0;
}

int calculate_start(int rank, int end, int workers, int begin) {
  const int range = end - begin;
  return begin + rank * (range / workers) + std::min(rank, range % workers);
}

int calculate_finish(int rank, int end, int workers, int begin) {
  return calculate_start(rank + 1, end, workers, begin);
}