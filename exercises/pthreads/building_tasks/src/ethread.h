// Copyright 2024 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#ifndef ETHREAD_H  // ECCI-Threads wrapper
#define ETHREAD_H

#include <pthread.h>

// Opaque records
/// Private memory for a thread
struct thread_data;
/// A team of threads
struct thread_team;

/// Create a single thread
/// @param data Data to be used by the thread, or NULL if none
struct thread_data* create_thread(void* (*routine)(void* data), void* data);

/// Create a team of threads running the same routine
/// @param data Shared data by all threads
struct thread_team* create_threads(const size_t thread_count,
    void* (*routine)(void* data), void* data);

/// Create a team of threads to run different tasks
/// @param data Shared data by all threads
struct thread_team* reserve_threads(const size_t capacity, void* data);

/// Add a single thread to the given team
int add_thread(struct thread_team* team, void* (*routine)(void* data));

/// Add more threads to an existing team
// TODO(jhc): pending
// int add_threads(struct thread_team* team, const size_t count,
//     void* (*routine)(void* data));

/// Wait for a thread until it finishes
int join_thread(struct thread_data* thread);

/// Wait for all threds in team
int join_threads(struct thread_team* team);

/// Get the thread number if it is part of a team, 0 otherwise
size_t get_thread_number(const void* data);

/// Get the number of threads if thread is part of team, 1 otherwise
size_t get_thread_count(const void* data);

/// Get a pointer to the shared data given to the routine
void* get_shared_data(void* data);

#endif  // ETHREAD_H
