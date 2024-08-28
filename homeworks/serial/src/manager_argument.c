// Copyright [2024] <Sebastián Orozco>

#include <stdio.h>
#include <stdlib.h>
#include "manager_argument.h"
#include "path_creator.h"

void init_managerArgument(manager_argument *manager, char *argv[]) {
  // create the path of job file
  char* path_job = make_path(argv[2], argv[1]);
  char* path_output = make_path(argv[3], "output.txt");
  manager->jobPath = path_job;
  manager->outputPath = path_output;
  free(path_job);
  free(path_output);
}

char* get_jobPath(manager_argument *manager){
  // to valitate jobPath exist
  if(!manager->jobPath){
    fprintf(stderr,"Not found jobPath");
    return NULL;
  }
  return manager->jobPath;
}

char* get_outputPath(manager_argument *manager){
  // to valitate outputPath exist
  if(!manager->outputPath){
    fprintf(stderr,"Not found jobPath");
    return NULL;
  }
  return manager->outputPath;
}
