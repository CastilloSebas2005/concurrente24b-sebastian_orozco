// Copyright 2024 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <assert.h>
#include <stdlib.h>

#include "ethread.h"

struct thread_data {
  size_t thread_number;  // rank
  pthread_t thread_id;
  void* data;
  struct thread_team* team;
};

// thread_team is not buffered, no capacity field
struct thread_team {
  size_t thread_count;
  struct thread_data* threads;
};

size_t get_thread_number(const void* data) {
  assert(data);
  return ((const struct thread_data*)data)->thread_number;
}

size_t get_thread_count(const void* data) {
  assert(data);
  const struct thread_data* private_data = (const struct thread_data*)data;
  return private_data->team ? private_data->team->thread_count : 1;
}

void* get_shared_data(void* data) {
  assert(data);
  return ((struct thread_data*)data)->data;
}

int create_team_thread(struct thread_data* thread,
      void* (*routine)(void* data), void* data) {
  assert(thread);
  assert(routine);
  if (data) {
    thread->data = data;
  }
  return pthread_create(&thread->thread_id, NULL, routine, thread);
}

struct thread_data* create_thread(void* (*routine)(void* data), void* data) {
  assert(routine);
  struct thread_data* thread = (struct thread_data*)
      calloc(1, sizeof(struct thread_data));
  if (thread) {
    if (create_team_thread(thread, routine, data) != 0) {
      free(thread);
      thread = NULL;
    }
  }
  return thread;
}

struct thread_team* create_threads(const size_t thread_count,
    void* (*routine)(void* data), void* data) {
  assert(thread_count);
  assert(routine);
  struct thread_team* team = (struct thread_team*)
    calloc(1, sizeof(struct thread_team));
  if (team) {
    team->threads = (struct thread_data*)
      calloc(thread_count, sizeof(struct thread_data));
    if (team->threads) {
      team->thread_count = thread_count;
      for (size_t thread_number = 0; thread_number < thread_count
          ; ++thread_number) {
        team->threads[thread_number].thread_number = thread_number;
        team->threads[thread_number].team = team;
        team->threads[thread_number].data = data;
        if (create_team_thread(&team->threads[thread_number], routine, data)) {
          team->thread_count = thread_number;
          join_threads(team);
          return NULL;
        }
      }
    }
  }
  return team;
}

struct thread_team* reserve_threads(const size_t capacity, void* data) {
  assert(capacity);
  struct thread_team* team = (struct thread_team*)
    calloc(1, sizeof(struct thread_team));
  if (team) {
    team->threads = (struct thread_data*)
      calloc(capacity, sizeof(struct thread_data));
    if (team->threads) {
      team->thread_count = 0;
      for (size_t thread_number = 0; thread_number < capacity
          ; ++thread_number) {
        team->threads[thread_number].thread_number = thread_number;
        team->threads[thread_number].team = team;
        team->threads[thread_number].data = data;
      }
    }
  }
  return team;
}

int add_thread(struct thread_team* team, void* (*routine)(void* data)) {
  assert(team);
  assert(routine);
  // TODO(jhc): Add capacity to team and check do not overflow it
  return create_team_thread(&team->threads[team->thread_count++], routine,
      NULL);
}

int join_team_thread(struct thread_data* thread) {
  assert(thread);
  return pthread_join(thread->thread_id, NULL);
}

int join_thread(struct thread_data* thread) {
  const int result = join_team_thread(thread);
  free(thread);
  return result;
}

int join_threads(struct thread_team* team) {
  assert(team);
  int result = EXIT_SUCCESS;
  for (size_t thread_number = 0; thread_number < team->thread_count
      ; ++thread_number) {
    const int error = join_team_thread(&team->threads[thread_number]);
    if (result == EXIT_SUCCESS) {
      result = error;
    }
  }
  free(team->threads);
  free(team);
  return result;
}
