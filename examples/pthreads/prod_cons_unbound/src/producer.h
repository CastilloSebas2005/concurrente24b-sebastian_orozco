// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef PRODUCER_H
#define PRODUCER_H

/// @brief Produce un dato y lo agrega a la cola
/// @param data Datos de producción
/// @return NULL
void* produce(void* data);

#endif  // PRODUCER_H
