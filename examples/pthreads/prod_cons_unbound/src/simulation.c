// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

#include "common.h"
#include "consumer.h"
#include "producer.h"
#include "simulation.h"

/// @brief Maneja los argumentos de la simulación
/// @param simulation Objeto de simulación
/// @param argc Cantidad de argumentos
/// @param argv Argumentos
/// @return variable de error
int analyze_arguments(simulation_t *simulation, int argc, char *argv[]);

/// @brief Crea hilos para productores y consumidores @see producer.h @see
/// consumer.h
/// @param simulation Objeto de simulación
/// @return variable de error
int create_consumers_producers(simulation_t *simulation);

/// @brief Se encarga de destruir los hilos
/// @param count cantidad de hilos
/// @param threads arreglo de hilos
/// @return variable de error
int join_threads(size_t count, pthread_t *threads);

simulation_t *simulation_create() {
  simulation_t *simulation = (simulation_t *)calloc(1, sizeof(simulation_t));
  if (simulation) {
    simulation->unit_count = 0;
    simulation->producer_count = 0;
    simulation->consumer_count = 0;
    simulation->producer_min_delay = 0;
    simulation->producer_max_delay = 0;
    simulation->consumer_min_delay = 0;
    simulation->consumer_max_delay = 0;
    queue_init(&simulation->queue);
    pthread_mutex_init(&simulation->can_access_next_unit, /* attr */ NULL);
    simulation->next_unit = 0;
    sem_init(&simulation->can_consume, /* pshared */ 0, /* value */ 0);
    pthread_mutex_init(&simulation->can_access_consumed_count, /* attr */ NULL);
    simulation->consumed_count = 0;
  }
  return simulation;
}

void simulation_destroy(simulation_t *simulation) {
  assert(simulation);
  pthread_mutex_destroy(&simulation->can_access_consumed_count);
  sem_destroy(&simulation->can_consume);
  pthread_mutex_destroy(&simulation->can_access_next_unit);
  queue_destroy(&simulation->queue);
  free(simulation);
}

int simulation_run(simulation_t *simulation, int argc, char *argv[]) {
  int error = analyze_arguments(simulation, argc, argv);
  if (error == EXIT_SUCCESS) {
    unsigned int seed = 0;
    getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
    srandom(seed);

    struct timespec start_time;
    clock_gettime(/*clk_id*/ CLOCK_MONOTONIC, &start_time);

    error = create_consumers_producers(simulation);

    struct timespec finish_time;
    clock_gettime(/*clk_id*/ CLOCK_MONOTONIC, &finish_time);

    double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
                     (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    printf("execution time: %.9lfs\n", elapsed);
  }
  return error;
}

int analyze_arguments(simulation_t *simulation, int argc, char *argv[]) {
  int error = EXIT_SUCCESS;
  if (argc == 8) {
    if (sscanf(argv[1], "%zu", &simulation->unit_count) != 1 ||
        simulation->unit_count == 0) {
      fprintf(stderr, "error: invalid unit count\n");
      error = ERR_UNIT_COUNT;
    } else if (sscanf(argv[2], "%zu", &simulation->producer_count) != 1 ||
               simulation->producer_count == 0) {
      fprintf(stderr, "error: invalid producer count\n");
      error = ERR_PRODUCER_COUNT;
    } else if (sscanf(argv[3], "%zu", &simulation->consumer_count) != 1 ||
               simulation->consumer_count == 0) {
      fprintf(stderr, "error: invalid consumer count\n");
      error = ERR_CONSUMER_COUNT;
    } else if (sscanf(argv[4], "%u", &simulation->producer_min_delay) != 1) {
      fprintf(stderr, "error: invalid min producer delay\n");
      error = ERR_MIN_PROD_DELAY;
    } else if (sscanf(argv[5], "%u", &simulation->producer_max_delay) != 1) {
      fprintf(stderr, "error: invalid max producer delay\n");
      error = ERR_MAX_PROD_DELAY;
    } else if (sscanf(argv[6], "%u", &simulation->consumer_min_delay) != 1) {
      fprintf(stderr, "error: invalid min consumer delay\n");
      error = ERR_MIN_CONS_DELAY;
    } else if (sscanf(argv[7], "%u", &simulation->consumer_max_delay) != 1) {
      fprintf(stderr, "error: invalid max consumer delay\n");
      error = ERR_MAX_CONS_DELAY;
    }
  } else {
    fprintf(stderr, "usage: producer_consumer buffer_capacity rounds"
                    " producer_min_delay producer_max_delay"
                    " consumer_min_delay consumer_max_delay\n");
    error = ERR_NO_ARGS;
  }
  return error;
}

/// @note Esto fue documentado anteriormente en otros ejemplos
/// además de que su cuerpo es parecido a create_threads_team
pthread_t *create_threads(size_t count, void *(*subroutine)(void *),
                          void *data) {
  pthread_t *threads = (pthread_t *)calloc(count, sizeof(pthread_t));
  if (threads) {
    for (size_t index = 0; index < count; ++index) {
      if (pthread_create(&threads[index], /*attr*/ NULL, subroutine, data) ==
          EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        join_threads(index, threads);
        return NULL;
      }
    }
  }
  return threads;
}

int join_threads(size_t count, pthread_t *threads) {
  int error = EXIT_SUCCESS;
  for (size_t index = 0; index < count; ++index) {
    // todo: sum could not be right
    error += pthread_join(threads[index], /*value_ptr*/ NULL);
  }
  free(threads);
  return error;
}

int create_consumers_producers(simulation_t *simulation) {
  assert(simulation);
  int error = EXIT_SUCCESS;

  /// @note Se crean los hilos de productores y consumidores con sus respectivas
  /// funciones
  pthread_t *producers =
      create_threads(simulation->producer_count, produce, simulation);
  pthread_t *consumers =
      create_threads(simulation->consumer_count, consume, simulation);
  if (producers && consumers) {
    join_threads(simulation->producer_count, producers);
    join_threads(simulation->consumer_count, consumers);
  } else {
    fprintf(stderr, "error: could not create threads\n");
    error = ERR_CREATE_THREAD;
  }

  return error;
}
