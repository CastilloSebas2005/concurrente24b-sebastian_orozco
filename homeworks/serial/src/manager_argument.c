// Copyright [2024] <Sebastián Orozco>

#include <stdio.h>
#include <stdlib.h>
#include "path_creator.h"
#include "manager_argument.h"

void init_managerArgument(manager_argument_t *manager, char *argv[]) {
  // create the path of job file
  char *path_job = make_path(argv[2], argv[1]);
  char *path_output = make_path(argv[3], "output.txt");
  manager->jobPath = path_job;
  manager->outputPath = path_output;
}

char *get_jobPath(manager_argument_t *manager) {
  // to valitate jobPath exist
  if (!manager->jobPath) {
    fprintf(stderr, "Not found jobPath");
    return NULL;
  }
  return manager->jobPath;
}

char *get_outputPath(manager_argument_t *manager) {
  // to valitate outputPath exist
  if (!manager->outputPath) {
    fprintf(stderr, "Not found jobPath");
    return NULL;
  }
  return manager->outputPath;
}

__uint64_t get_lines_to_read(char* path){
  FILE *file = fopen(path, "r");
  if(!file){  //validate that path exist
    perror("Error: can't count the lines");
    return 1;
  } 
  // now count the lines of file
  __uint64_t lines = 0;
  char c; 
  // when read the last line the loops ends, save the count and return the lines
   while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }
  fclose(file);
  return lines;
}