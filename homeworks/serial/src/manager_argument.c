// Copyright [2024] <Sebastián Orozco>

#include "manager_argument.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool validateJobFile(char *argv);

void init_managerArgument(manager_argument *manager, char *argv[]) {
  manager->argv1 = argv[1];
  manager->argv2 = argv[2];
  manager->argv3 = argv[3];
}

char *get_arg1(manager_argument *manager) {
  printf("%s\n", manager->argv1);
  return manager->argv1;
}

char *get_arg2(manager_argument *manager) {
  printf("%s\n", manager->argv1);
  return manager->argv2;
}

char *get_arg3(manager_argument *manager) {
  printf("%s\n", manager->argv1);
  return manager->argv3;
}

// this is to validate that argument contains "job" but this is not perfect
// file manager is going to validate that file exist
bool validateJobFile(char *argv) { return strstr(argv, "job"); }
