// Copyright [2024] <Sebastián Orozco>

#include "controller.h"
#include "manager_argument.h"
#include "plate.h"
#include "simulation.h"

void init_controller(char *argv[]) {
  manager_argument_t manager_argument;
  init_managerArgument(&manager_argument, argv);
  char *jobPath = get_jobPath(&manager_argument);
  char *output_Path = get_outputPath(&manager_argument);
  if (!jobPath && !output_Path) {
    fprintf(stderr, "Error: don't exist the subdirectory, plase try again\n");
    return;
  }
  uint64_t linesToRead = get_lines_to_read(jobPath);
  if (linesToRead == 0) {
    fprintf(stderr, "Error: something bad in your arguments or jobFile\n");
    destruct_manager(&manager_argument);
    return;
  }
  plate_t plates[linesToRead];  //NOLINT
  printf("%li", linesToRead); //NOLINT
  for (uint64_t i = 0; i < linesToRead-1; i++) {
    uint8_t error1 = init_plate(&plates[i], jobPath, argv[2], i);
    if (!error1) {
      destruct_plate(&plates[i]);
    }
  }

  for (uint64_t i = 0; i < linesToRead-1; i++) {
    uint8_t error2 = init_simulation(plates[i], output_Path);
    if (!error2) {
      fprintf(stderr, "Error: something bad in the simulation\n"); //NOLINT
      destruct_plate(&plates[i]);
      destruct_manager(&manager_argument);
      return;
    }
  }

  for (uint64_t i = 0; i < linesToRead-1; i++) {
    destruct_plate(&plates[i]);
  }
  destruct_manager(&manager_argument);
}
