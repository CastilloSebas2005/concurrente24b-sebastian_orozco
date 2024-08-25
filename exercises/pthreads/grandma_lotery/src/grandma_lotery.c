// Copyright [2024] <Sebastián Orozco>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *numberGrandson1();
void *numberGrandson2();

// grandma thread
int main(void) {
  // declaration of secundary threads
  pthread_t grandson1;
  pthread_t grandson2;
  // creation of secundary threads
  int error1 = pthread_create(&grandson1, NULL, numberGrandson1, NULL);
  int error2 = pthread_create(&grandson2, NULL, numberGrandson2, NULL);
  // this is to save return value of numberGrandson on both threads
  void *numberGrandson1 = NULL;
  void *numberGrandson2 = NULL;
  if (error1 == EXIT_SUCCESS && error2 == EXIT_SUCCESS) {
    // extraction of the number of the lotery
    pthread_join(grandson1, &numberGrandson1);
    pthread_join(grandson2, &numberGrandson2);
    // cast the void to int
    __int8_t *number1 = (__int8_t *)numberGrandson1;
    printf("Number of grandson 1 is: %i\n", *number1);
    printf("Number of grandson 2 is: %i\n", numberGrandson2);
    free(number1);
  }
  return 0;
}

void *numberGrandson1() {
  unsigned int seed = (unsigned int)time(NULL);
  __int8_t *number = malloc(sizeof(__int8_t));
  // generate a pseudorandom number [0,100]
  *number = rand_r(&seed) % 101;
  return number;
}

void *numberGrandson2() {
  // generate a pseudorandom number [0,100]
  // we need to save the number in the direction of memory
  // this is the "false" number in direction of memory
  unsigned int seed = (unsigned int)getpid();
  return (void *)(rand_r(&seed) % 101);
}
