// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// TODO(all): Implement a thread-safe queue

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>

/// @brief Nodo de la cola
typedef struct queue_node {
  size_t data;
  struct queue_node* next;
} queue_node_t;

/// @brief Cola
typedef struct {
  pthread_mutex_t can_access_queue;
  queue_node_t* head;
  queue_node_t* tail;
} queue_t;

/**
 * @todo: document all interfaces
 * @remarks This subroutine is NOT thread-safe
 */

/// @brief Inicializa la cola
/// @param queue Objeto de cola
/// @return variable de error
int queue_init(queue_t* queue);

/// @brief Destruye la cola y libera memoria
/// @param queue Objeto de cola
/// @return variable de error
int queue_destroy(queue_t* queue);

/**
 * @remarks This subroutine is thread-safe
 */

/// @brief Verifica si la cola está vacía
/// @param queue Objeto de cola
/// @return true si la cola está vacía, false de lo contrario
bool queue_is_empty(queue_t* queue);

/// @brief Agrega un elemento a la cola
/// @param queue Objeto de cola
/// @param data Dato a agregar
/// @return variable de error
int queue_enqueue(queue_t* queue, const size_t data);

/// @brief Elimina un elemento de la cola
/// @param queue Objeto de cola
/// @param data Dato eliminado
/// @return variable de error
int queue_dequeue(queue_t* queue, size_t* data);

/// @brief Elimina todos los elementos de la cola
/// @param queue Objeto de cola
void queue_clear(queue_t* queue);

#endif  // QUEUE_H
