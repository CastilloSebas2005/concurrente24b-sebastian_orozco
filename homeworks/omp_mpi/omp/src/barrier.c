// Copyright [2024] <Sebastián Orozco>

#include <stdint.h>

#include "barrier.h"

uint8_t init_barrier(barrier_t *barrier, uint64_t count) {
  barrier->count = count;
  barrier->thread_count = count;
  pthread_mutex_init(&barrier->mutex, NULL);
  pthread_cond_init(&barrier->cond, NULL);
  return 0;
}

uint8_t wait_barrier(barrier_t *barrier) {
  pthread_mutex_lock(&barrier->mutex);
  barrier->count--;
  if (barrier->count == 0) {
    barrier->count = barrier->thread_count;
    pthread_cond_broadcast(&barrier->cond);
  } else {
    pthread_cond_wait(&barrier->cond, &barrier->mutex);
  }
  pthread_mutex_unlock(&barrier->mutex);
  return 0;
}

uint8_t destruct_barrier(barrier_t *barrier) {
  pthread_mutex_destroy(&barrier->mutex);
  pthread_cond_destroy(&barrier->cond);
  return 0;
}
