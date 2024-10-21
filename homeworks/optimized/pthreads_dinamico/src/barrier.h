// Copyright [2024] <Sebastián Orozco>

#ifndef BARRIER_H
#define BARRIER_H

#include <pthread.h>

/// @brief This is the barrier
/// @struct barrier
/// @var barrier::mutex
/// This is the mutex to protect the barrier
/// @var barrier::cond
/// This is the condition to wait the barrier
/// @var barrier::count
/// This is the count of threads
/// @var barrier::thread_count
/// This is the count of threads
typedef struct barrier {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  uint64_t count;
  uint64_t thread_count;
} barrier_t;

/// @brief This is to initialize the barrier
/// @param barrier This is the barrier
/// @param count This is the count of threads
/// @return 0 if all is good 1 if occurs an error
uint8_t init_barrier(barrier_t *barrier, uint64_t count);

/// @brief This is to wait the barrier
/// @param barrier This is the barrier
/// @return 0 if all is good 1 if occurs an error
uint8_t wait_barrier(barrier_t *barrier);

/// @brief This is to destruct the barrier
/// @param barrier This is the barrier
/// @return 0 if all is good 1 if occurs an error
uint8_t destruct_barrier(barrier_t *barrier);

#endif
