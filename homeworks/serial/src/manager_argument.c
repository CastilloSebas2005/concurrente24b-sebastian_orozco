// Copyright [2024] <Sebastián Orozco>

#include "manager_argument.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool validateJobFile(char *argv);

void init_managerArgument(manager_argument *manager, char *argv[]) {
  manager->argv = argv[1];
  if (!validateJobFile(argv[1])) {
    fprintf(stderr, "Error: can't find the job file in the argument\n");
    return;
  }
}

// this is to validate that argument contains "job" but this is not perfect
// file manager is going to validate that file exist
bool validateJobFile(char *argv) { return strstr(argv, "job"); }
