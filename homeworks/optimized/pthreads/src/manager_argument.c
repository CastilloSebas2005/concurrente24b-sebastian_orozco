// Copyright [2024] <Sebastián Orozco>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
  // to get the count of threads that are in the computer
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (sscanf(argv[4], "%" SCNu64, &thread_count) == 1) {
    manager->thread_count = thread_count;
  } else {
    manager->thread_count = 0;
    fprintf(stderr, "Error: invalid thread count\n");
    free(jobName);
    return;
  }
  if (thread_count == 0) {
    manager->thread_count = sysconf(_SC_NPROCESSORS_ONLN);;
  }
  free(jobName);
}

char *get_jobPath(manager_argument_t *manager) {
  // to valitate jobPath exist
  if (!manager->jobPath) {
    fprintf(stderr, "Error: jobpath is NULL\n");
    return NULL;
  }
  FILE *file = fopen(manager->jobPath, "r");
  if (!file) {
    fprintf(stderr, "Error: the path of the jobfile is incorrect\n");
    return NULL;
  }
  fclose(file);
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
  if (!file) {  // validate that path exists
    fprintf(stderr, "Error: can't open the file to count the lines\n");
    return 0;
  }

  // Check if the file is empty
  int first_char = fgetc(file);
  if (first_char == EOF) {
    fprintf(stderr, "Error: the file %s is empty\n", path);
    fclose(file);
    return 0;
  }

  // Reset the file pointer to the beginning of the file
  rewind(file);

  // Now count the lines of the file
  __uint64_t lines = 0;
  char c;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      lines++;
    }
  }

  // If the last character is not a newline, add one more line
  if (first_char != '\n') {
    lines++;
  }

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
  fprintf(stderr, "Error: this program need a file called jobxxx.txt\n");
  return NULL;
}

void destruct_manager(manager_argument_t *manager) {
  free(manager->jobPath);
  free(manager->outputPath);
}
