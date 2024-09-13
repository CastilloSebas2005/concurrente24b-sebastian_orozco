// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef CONSUMER_H
#define CONSUMER_H

/// @brief Consume un dato de la cola
/// @param data Datos a consumir
/// @return NULL
void* consume(void* data);

#endif  // CONSUMER_H
