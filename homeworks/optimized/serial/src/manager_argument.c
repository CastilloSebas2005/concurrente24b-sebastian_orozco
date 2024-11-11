// Copyright [2024] <Sebastián Orozco>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "manager_argument.h"
#include "path_creator.h"

char *extract_outputName(char *jobFile);

void init_managerArgument(manager_argument_t *manager, char *argv[]) {
  // create the path of job file
  char *path_job = make_path(argv[2], argv[1]);
  char *jobName = extract_outputName(argv[1]);
  char *path_output = make_path(argv[3], jobName);
  manager->jobPath = path_job;
  manager->outputPath = path_output;
  free(jobName);
}

char *get_jobPath(manager_argument_t *manager) {
  // to valitate jobPath exist
  if (!manager->jobPath) {
    fprintf(stderr, "Error: not found jobPath\n");
    return NULL;
  }
  return manager->jobPath;
}

char *get_outputPath(manager_argument_t *manager) {
  // to valitate outputPath exist
  if (!manager->outputPath) {
    fprintf(stderr, "Error: not found outputPath\n");
    return NULL;
  }
  return manager->outputPath;
}
__uint64_t get_lines_to_read(char *path) {
  FILE *file = fopen(path, "r");
  if (!file) {  // validate that path exist
    fprintf(stderr,"Error: can't count the lines\n"); //NOLINT
    return 0;
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
  lines++;
  fclose(file);
  return lines;
}

/// @brief this is to extract the jobxxx.txt and create the jobxxx.txt
/// @param jobFile name of file
/// @return jobxxx.tsv or NULL if don't exist job file
char *extract_outputName(char *jobFile) {
  // if exist the name of job
  if (!jobFile) {
    fprintf(stderr, "Error: not found jobFile\n");
    return NULL;
  }
  if (jobFile[0] == 'j' && jobFile[1] == 'o' && jobFile[2] == 'b') {
    size_t size = 0;
    while (jobFile[size] != '.') {
      size++;
    }
    // to .tsv'\0'
    size = size + 5;
    char *jobName = malloc(size * sizeof(char));
    __uint8_t i;
    for (i = 0; jobFile[i] != '.'; i++) {
      jobName[i] = jobFile[i];
    }
    char *extention = ".tsv";
    for (__uint8_t j = 0; extention[j] != '\0'; j++, i++) {
      jobName[i] = extention[j];
    }
    jobName[i++] = '\0';
    return jobName;
  }
  fprintf(stderr, "Error: this program need a file called jobxxx.txt\n"); //NOLINT
  return NULL;
}

void destruct_manager(manager_argument_t *manager) {
  free(manager->jobPath);
  free(manager->outputPath);
}
