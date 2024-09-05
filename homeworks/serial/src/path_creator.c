// Copyright [2024] <Sebastián Orozco>

#include "path_creator.h"
#include <stdio.h>
#include <stdlib.h>

char *make_path(char *subdirectory, char *nameArchive) {
  if (!subdirectory) {
    fprintf(stderr, "Error: subdirectory is null\n");
    return NULL;
  }
  if (!nameArchive) {
    fprintf(stderr, "Error: nameArchive is null\n");
    return NULL;
  }
  // this is to calculate the memory corretly
  size_t length1 = 0;
  while (subdirectory[length1] != '\0') {
    length1++;
  }
  size_t length2 = 0;
  while (nameArchive[length2] != '\0') {
    length2++;
  }
  length2 += 2;
  // assign the size of length2 + 2 and length1 to path
  char *path = malloc((length1 + length2) * sizeof(char));
  if (!path) {
    fprintf(stderr, "Error: can't assing memory correctly");
    return NULL;
  }
  size_t j = 0;
  // both for are to construct the path correctly
  for (int i = 0; subdirectory[i] != '\0'; i++) {
    path[j++] = subdirectory[i];
  }
  path[j++] = '/';
  for (int i = 0; nameArchive[i] != '\0'; i++) {
    path[j++] = nameArchive[i];
  }
  path[j++] = '\0';
  printf("%s\n", path);
  return path;
}
