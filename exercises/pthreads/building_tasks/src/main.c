// Copyright 2024 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#define _XOPEN_SOURCE 500

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#include "ethread.h"

struct shared_data {
    useconds_t max_duration;  // microseconds
  sem_t semafore1;  // walls
  sem_t semafore2;  // exterior_plumbing
  sem_t semafore3;  // roof
  sem_t semafore4;  // exterior_painting
  sem_t semafore5;  // interior_plumbing
  sem_t semafore6;  // electrical_installation
  sem_t semafore7;  // exterior_finishes
  sem_t semafore8;  // interior_painting1
  sem_t semafore9;  // floors
  sem_t semafore10;  // interior_finishes
  sem_t semafore11;  // interior_painting2
} shared_data_t;

int do_concurrent_tasks(struct shared_data* shared);
void* walls(void* data);
void* exterior_plumbing(void* data);
void* roof(void* data);
void* exterior_painting(void* data);
void* interior_plumbing(void* data);
void* electrical_installation(void* data);
void* exterior_finishes(void* data);
void* interior_painting(void* data);
void* floors(void* data);
void* interior_finishes(void* data);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  struct shared_data* shared = (struct shared_data*)
      calloc(1, sizeof(struct shared_data));
  assert(shared);
  if (argc == 2 && sscanf(argv[1], "%u", &shared->max_duration) == 1 &&
      shared->max_duration) {
    srand48(time(NULL) + clock());
    sem_init(&shared->semafore1, /*pshared*/ 0, /*value*/ 1);
    sem_init(&shared->semafore2, /*pshared*/ 0, /*value*/ 0);
    sem_init(&shared->semafore3, /*pshared*/ 0, /*value*/ 0);
    sem_init(&shared->semafore4, /*pshared*/ 0, /*value*/ 0);
    sem_init(&shared->semafore5, /*pshared*/ 0, /*value*/ 0);
    sem_init(&shared->semafore6, /*pshared*/ 0, /*value*/ 0);
    sem_init(&shared->semafore7, /*pshared*/ 0, /*value*/ 0);
    sem_init(&shared->semafore8, /*pshared*/ 0, /*value*/ 0);
    sem_init(&shared->semafore9, /*pshared*/ 0, /*value*/ 0);
    sem_init(&shared->semafore10, /*pshared*/ 0, /*value*/ 0);
    sem_init(&shared->semafore11, /*pshared*/ 0, /*value*/ 0);
    do_concurrent_tasks(shared);
  } else {
    fprintf(stderr, "Usage: building_tasks max_microseconds_duration\n");
    error = EXIT_FAILURE;
  }
  free(shared);
  return error;
}

int do_concurrent_tasks(struct shared_data* shared) {
  // We reserve and allocate memory for the teams
  struct thread_team* team = reserve_threads(11, shared);
  assert(team);
  // We send all the threads to make their tasks
  add_thread(team, walls);
  add_thread(team, exterior_plumbing);
  add_thread(team, roof);
  add_thread(team, exterior_painting);
  add_thread(team, interior_plumbing);
  add_thread(team, electrical_installation);
  add_thread(team, exterior_finishes);
  add_thread(team, interior_painting);
  add_thread(team, floors);
  add_thread(team, interior_finishes);
  // We send the team to make their wait and destroy
  join_threads(team);
  return EXIT_SUCCESS;
}

void* walls(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for the first semaphore that
  // has been initialized in 1 to procede with execution
  int error1 = sem_wait(&shared->semafore1);
  int error2 = 0;
  puts("1.2 walls start");
  assert(shared);
  usleep(lrand48() % shared->max_duration);
  puts("1.2 walls finish");
  // We increase both semaphores to procede with their execution branches
  error1 = sem_post(&shared->semafore2);
  error2 = sem_post(&shared->semafore3);
  if (error1 || error2) {
    fprintf(stderr, "error: could not increment semaphore\n");
  }
  return NULL;
}

void* exterior_plumbing(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for this semaphore until it is increased
  int error1 = sem_wait(&shared->semafore2);
  int error2 = 0;
  puts("2.1 exterior plumbing start");
  usleep(lrand48() % shared->max_duration);
  puts("2.1 exterior plumbing finish");
  // We increase both semaphores to procede with their execution branches
  error1 = sem_post(&shared->semafore4);
  error2 = sem_post(&shared->semafore5);
  if (error1 || error2) {
    fprintf(stderr, "error: could not increment semaphore\n");
  }
  return NULL;
}

void* roof(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for this semaphore until it is increased
  int error = sem_wait(&shared->semafore3);
  puts("2.3 roof start");
  usleep(lrand48() % shared->max_duration);
  puts("2.3 roof finish");
  // We increase the semaphore to procede with their execution branches
  error = sem_post(&shared->semafore6);
  if (error) {
    fprintf(stderr, "error: could not increment semaphore\n");
  }
  return NULL;
}

void* exterior_painting(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for this semaphore until it is increased
  int error = sem_wait(&shared->semafore4);
  puts("3.1 exterior painting start");
  usleep(lrand48() % shared->max_duration);
  puts("3.1 exterior painting finish");
  // We increase the semaphore to procede with their execution branch
  error = sem_post(&shared->semafore7);
  if (error) {
    fprintf(stderr, "error: could not increment semaphore\n");
  }
  return NULL;
}

void* interior_plumbing(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for this semaphore until it is increased
  int error = sem_wait(&shared->semafore5);
  puts("3.2 interior plumbing start");
  usleep(lrand48() % shared->max_duration);
  puts("3.2 interior plumbing finish");
  // We increase the semaphore to procede with their execution branch
  error = sem_post(&shared->semafore8);
  if (error) {
    fprintf(stderr, "error: could not increment semaphore\n");
  }
  return NULL;
}

void* electrical_installation(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for this semaphore until it is increased
  int error = sem_wait(&shared->semafore6);
  puts("3.3 electrical installation start");
  usleep(lrand48() % shared->max_duration);
  puts("3.3 electrical installation finish");
  // We increase the semaphore to procede with their execution branch
  error = sem_post(&shared->semafore11);
  if (error) {
    fprintf(stderr, "error: could not increment semaphore\n");
  }
  return NULL;
}

void* exterior_finishes(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for this semaphore until it is increased
  int error = sem_wait(&shared->semafore7);
  puts("4.1 exterior finishes start");
  usleep(lrand48() % shared->max_duration);
  puts("4.1 exterior finishes finish");
  return NULL;
}

void* interior_painting(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for this two semaphorea until they are increased
  int error1 = sem_wait(&shared->semafore8);
  int error2 = sem_wait(&shared->semafore11);
  puts("4.3 interior painting start");
  usleep(lrand48() % shared->max_duration);
  puts("4.3 interior painting finish");
  // We increase the semaphore to procede with their execution branch
  error1 = sem_post(&shared->semafore9);
  if (error1 || error2) {
    fprintf(stderr, "error: could not increment semaphore\n");
  }
  return NULL;
}

void* floors(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for this semaphore until it is increased
  int error = sem_wait(&shared->semafore9);
  puts("5.3 floor start");
  usleep(lrand48() % shared->max_duration);
  puts("5.3 floor finish");
  // We increase the semaphore to procede with their execution branch
  error = sem_post(&shared->semafore10);
  return NULL;
}

void* interior_finishes(void* data) {
  // We get the shared_data to make the thread task
  struct shared_data* shared = (struct shared_data*) get_shared_data(data);
  // We wait for this semaphore until it is increased
  int error = sem_wait(&shared->semafore10);
  puts("6.3 interior finishes start");
  usleep(lrand48() % shared->max_duration);
  puts("6.3 interior finishes finish");
  return NULL;
}
