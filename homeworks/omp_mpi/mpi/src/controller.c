// Copyright [2024] <Sebastián Orozco>

#include <assert.h>
#include <mpi.h>
#include <stdbool.h>
#include <time.h>

#include "controller.h"
#include "manager_argument.h"
#include "plate.h"
#include "simulation.h"

/**
 * @brief This is the shared data of threads
 * @struct shared_data
 * @var shared_data::line_report
 * This is the array of lines to report, this contains the lines formatted
 * and this is saved in the output file
 */
typedef struct shared_data {
  char **line_report;
  uint64_t thread_count;
} shared_data_t;

/**
 * @brief this is the private data of threads
 * @struct private_data
 * @var private_data::plates
 * This is the array of plates, this contains the plates to make the simulation
 * @var private_data::linesToRead
 * This is the number of lines to read, this is unique en all threads
 * because all threads read the same file but plates are different
 * @var private_data::lines_to_write
 * This is the array of lines to write, this contains the number of line
 * that must write in the output file.
 * @var private_data::shared_data
 * This is the shared data of threads, this contains the lines to report
 *
 */
typedef struct private_data {
  plate_t *plates;
  uint64_t linesToRead;
  uint64_t *lines_to_write;
  shared_data_t *shared_data;
} private_data_t;

/// @brief This is to create the threads
/// @param report This is the array of lines to report
/// @param output_path This is the path of output file
/// @param linesToRead This is the number of lines to read
/// @return 0 if all is good 1 if occurs an error
uint8_t make_report(char **report, char *output_path, uint64_t linesToRead);

void init_controller(char *argv[], int rank, int rank_count) {
  manager_argument_t manager_argument;
  init_managerArgument(&manager_argument, argv);
  // get the path and verificate that path exist of job file and output file
  char *jobPath = get_jobPath(&manager_argument);
  char *output_Path = get_outputPath(&manager_argument);
  uint64_t thread_count = manager_argument.thread_count;
  if (!jobPath || !output_Path) {
    destruct_manager(&manager_argument);
    return;
  }
  uint64_t linesToRead = get_lines_to_read(jobPath);
  if (linesToRead == 0) {
    destruct_manager(&manager_argument);
    return;
  }
  if (thread_count == 0) {
    destruct_manager(&manager_argument);
    return;
  }
  plate_t plates[linesToRead]; // NOLINT
  shared_data_t *shared_data =
      (shared_data_t *)calloc(1, sizeof(shared_data_t));
  shared_data->thread_count = thread_count;
  shared_data->line_report = (char **)calloc(linesToRead, sizeof(char *));
  uint64_t work_to_do = 0;
  uint64_t work_done = 0;
  uint64_t states_to_report[linesToRead]; // NOLINT
  MPI_Status status;

  if (rank_count == 1) {
    // Execute in a single process
    for (size_t i = 0; i < (size_t)linesToRead; i++) {
      init_plate(&plates[i], jobPath, argv[2], i);
      if (plates[i].blankline == 0) {
        uint64_t states = init_simulation(plates[i], thread_count);
        time_t time = (time_t)(plates[i].time * states);
        char *line = make_line_to_report(plates[i].lineReaded, time, states);
        shared_data->line_report[i] = line;
        printf("Plate number: %" SCNu64 " calculated correctly...\n", i);
      }
    }
    make_report(shared_data->line_report, output_Path, linesToRead);
    for (uint64_t i = 0; i < linesToRead; i++) {
      if (plates[i].blankline == 0) {
        destruct_plate(&plates[i]);
      }
    }
    for (uint64_t i = 0; i < linesToRead; i++) {
      if (shared_data->line_report[i]) {
        free(shared_data->line_report[i]);
      }
    }
    free(shared_data->line_report);
    free(shared_data);
    destruct_manager(&manager_argument);
  } else if (rank == 0) {
    // Distribute initial tasks
    for (size_t i = 0; i < (size_t)linesToRead; i++) {
      init_plate(&plates[i], jobPath, argv[2], i);
    }
    for (int i = 1; i < rank_count && work_to_do < linesToRead; i++) {
      MPI_Send(&work_to_do, 1, MPI_UINT64_T, i, 0, MPI_COMM_WORLD);
      work_to_do++;
    }

    while (work_done < linesToRead) {
      uint64_t completed_task;
      MPI_Recv(&completed_task, 1, MPI_UINT64_T, MPI_ANY_SOURCE, 0,
               MPI_COMM_WORLD, &status);
      work_done++;
      uint64_t received_state;
      MPI_Recv(&received_state, 1, MPI_UINT64_T, status.MPI_SOURCE, 0,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      states_to_report[completed_task] = received_state;
      if (work_to_do < linesToRead) {
        MPI_Send(&work_to_do, 1, MPI_UINT64_T, status.MPI_SOURCE, 0,
                 MPI_COMM_WORLD);
        work_to_do++;
      } else {
        uint64_t no_more_work = linesToRead;
        MPI_Send(&no_more_work, 1, MPI_UINT64_T, status.MPI_SOURCE, 0,
                 MPI_COMM_WORLD);
      }
    }

    for (size_t i = 0; i < (size_t)linesToRead; i++) {
      if (plates[i].blankline == 0) {
        time_t time = (time_t)(plates[i].time * states_to_report[i]);
        char *line = make_line_to_report(plates[i].lineReaded, time,
                                         states_to_report[i]);
        shared_data->line_report[i] = line;
      }
    }
    make_report(shared_data->line_report, output_Path, linesToRead);
    for (uint64_t i = 0; i < linesToRead; i++) {
      if (plates[i].blankline == 0) {
        destruct_plate(&plates[i]);
      }
    }
    for (uint64_t i = 0; i < linesToRead; i++) {
      if (shared_data->line_report[i]) {
        free(shared_data->line_report[i]);
      }
    }

    free(shared_data->line_report);
    free(shared_data);
    destruct_manager(&manager_argument);
  } else {
    while (true) {
      MPI_Recv(&work_to_do, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      if (work_to_do >= linesToRead) {
        break;
      }
      init_plate(&plates[work_to_do], jobPath, argv[2], work_to_do);
      if (plates[work_to_do].blankline == 0) {
        printf("Rank %i calculating\n", rank);
        uint64_t states = init_simulation(plates[work_to_do], thread_count);
        printf("Plate number %li calculated correctly by rank: %i\n",
               work_to_do, rank);
        MPI_Send(&work_to_do, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&states, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD);
        destruct_plate(&plates[work_to_do]);
      } else {
        MPI_Send(&work_to_do, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD);
        uint64_t no_state = 0;
        MPI_Send(&no_state, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD);
      }
    }
    for (uint64_t i = 0; i < linesToRead; i++) {
      if (shared_data->line_report[i]) {
        free(shared_data->line_report[i]);
      }
    }
    free(shared_data->line_report);
    free(shared_data);
    destruct_manager(&manager_argument);
  }
}

uint8_t make_report(char **report, char *output_path, uint64_t linesToRead) {
  if (!report && !output_path) {
    fprintf(stderr, "Error: can't make the report\n");
    return 1;
  }
  FILE *file = fopen(output_path, "w");
  if (!file) {
    fprintf(stderr, "Error: can't create report, change your directory %s\n",
            output_path);
    return 1;
  }
  for (uint64_t i = 0; i < linesToRead; i++) {
    if (report[i]) {
      fputs(report[i], file);
    }
  }
  printf("Report completely\n");
  fclose(file); // NOLINT
  return 0;
}
