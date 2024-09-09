// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// Simulates a producer and a consumer that share a bounded buffer

/// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

/// @enum constants
/// @brief simplemente se definen enums
/// son constantes globales que se pueden usar
/// en cualquier momento
enum constants{
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_BUFFER_CAPACITY,
  ERR_ROUND_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};

/// @struct shared_data
/// @brief Memoria compartida, no se expliacarán
/// variables vistas en códigos anteriores

/** @var shared_data::buffer_capacity
 *  Es la capacidad del buffer ingresada por argumento.
 *  Capacidad o tamaño.
 *  @var shared_data::buffer
 *  Es el buffer donde se van a guardar(producir) y quitar(consumir) los "productos".
 *  @var shared_data::rounds
 *  Cantidad de vueltas que se le tienen que dar al buffer.
 *  @var shared_data::can_produce
 *  Semáforo que controla si el "productor puede producir".
 *  shared_data::can_consume
 *  Semáforo que controla si el "consumidor puede consumir".
 *  shared_data::producer_min_delay
 *  Duración mínima en milisegundos que el productor tarda en generar un producto.
 *  shared_data::producer_max_delay
 *  Duración máxima en milisegundos que el productor tarda en generar un producto.
 *  shared_data::consumer_min_delay
 *  Duración mínima en milisegundos que el consumidor tarda en consumir un producto.
 *  shared_data::consumer_max_delay
 *  Duración máxima en milisegundos que el consumidor tarda en consumir un producto.
 */
typedef struct shared_data {
  size_t thread_count;
  size_t buffer_capacity;
  double* buffer;
  size_t rounds;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;

  sem_t can_produce;
  sem_t can_consume;
} shared_data_t;

/// @struct private_data
/// @brief memoria privada, vista en códigos anteriores
/// de hecho, en este ejemplo no es importante este struct
typedef struct private_data {
  size_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

/// @brief Simplemente es para validar que los argumentos estén bien.
/// @param argc Número de argumentos.
/// @param argv Argumentos.
/// @param shared_data Memoria compartida.
/// @return variable error, contiene un enum definido anteriormente.
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);

/// @brief Función vista en ejemplos pasados, con la diferencia que
/// en vez de usar un ciclo para la creación de hilos, simplemente crea dos
/// el hilo del consumidor y el hilo del productor, por lo que se hacen
/// manualmente
/// @param shared_data el struct de la memoria compartida
/// @return variable error
int create_threads(shared_data_t* shared_data);

/// @brief Es la función para el productor.
/// Usa los semáforos apropiados para el productor(más adelante se explica).
/// También en esta función se le están dando "vueltas al buffer"
/// es decir, se está recorriendo para poder producir si el semáforo lo indica.
/// la diferencia de esta función con consume es que está función ASIGNA
/// un valor al buffer.
/// @param data memoria compartida
/// @return NULL
void* produce(void* data);

/// @brief Es la función para el consumidor.
/// Usa los semáforos apropiados para el consumidor(más adelante se explica).
/// @param data memoria compartida
/// @return NULL
void* consume(void* data);

/// @brief Esta función es simplmente genera un número aleatorio
/// entre los parámetros min y max, para esta función es que se genera
/// la semilla en main. Esta función está conectada a consume() y produce()
/// ya que estas funciones trabajan con números pseudoaleatorios.
/// Revisar el struct shared_data para saber cuales valores son max y min
/// estos valores pueden variar dependiendo de la función de donde se llamen
/// si se llaman desde consumer() son los máximos y mínimos del consumer
/// y si se llaman desde produce() son los máximos y mínimes del producer
/// @param min valor mínimo del rango.
/// @param max valor máximo del rango.
/// @return si max > min retorna un número pseudoalaterio entre 0 y max - min
/// si max < min  la expresión es false y devuelve un 0
useconds_t random_between(useconds_t min, useconds_t max);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  shared_data_t* shared_data = (shared_data_t*) // NOLINT
    calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    error = analyze_arguments(argc, argv, shared_data);
    /** @note si la función de analyze_arguments() no retorna error
     *  entonces se procede a crear el buffer. Se pide memoria dinámica,
     *  recordando que el buffer es un arreglo de doubles en este caso.
     *  Nota: las variables que sean inicializadas en esta función
     *  ya fueron descritas en shared_data.
     * 
     */ 
    if (error == EXIT_SUCCESS) {
      shared_data->buffer = (double*) // NOLINT
        calloc(shared_data->buffer_capacity, sizeof(double));
      if (shared_data->buffer) {
        /** @note si el buffer no es 0, entonces se inicializan los semaforos de shared_data
         *  los semáforos que se inicializan son tanto los del consumidor como los del productor
         *  y se genera una semilla debido a que las funciones de este programan van a trabajar
         *  con números pseudo aleatorios.
         *  Posteriormente estos se destruyen, pero hasta el final de la función.
         */
        sem_init(&shared_data->can_produce, /*pshared*/ 0,
          shared_data->buffer_capacity);
        sem_init(&shared_data->can_consume, /*pshared*/ 0, /*value*/ 0);
        unsigned int seed = 0u;
        getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
        srandom(seed);

        struct timespec start_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

        error = create_threads(shared_data);

        struct timespec finish_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

        double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed);

        sem_destroy(&shared_data->can_consume);
        sem_destroy(&shared_data->can_produce);
        free(shared_data->buffer);
      } else {
        fprintf(stderr, "error: could not create buffer\n");
        error = ERR_NOMEM_BUFFER;
      }
    }

    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = ERR_NOMEM_SHARED;
  }

  return error;
}

int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  if (argc == 7) {
    if (sscanf(argv[1], "%zu", &shared_data->buffer_capacity) != 1
      || shared_data->buffer_capacity == 0) {
        fprintf(stderr, "error: invalid buffer capacity\n");
        error = ERR_BUFFER_CAPACITY;
    } else if (sscanf(argv[2], "%zu", &shared_data->rounds) != 1
      || shared_data->rounds == 0) {
        fprintf(stderr, "error: invalid round count\n");
        error = ERR_ROUND_COUNT;
    } else if (sscanf(argv[3], "%u", &shared_data->producer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min producer delay\n");
        error = ERR_MIN_PROD_DELAY;
    } else if (sscanf(argv[4], "%u", &shared_data->producer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max producer delay\n");
        error = ERR_MAX_PROD_DELAY;
    } else if (sscanf(argv[5], "%u", &shared_data->consumer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min consumer delay\n");
        error = ERR_MIN_CONS_DELAY;
    } else if (sscanf(argv[6], "%u", &shared_data->consumer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max consumer delay\n");
        error = ERR_MAX_CONS_DELAY;
    }
  } else {
    fprintf(stderr, "usage: prod_cons_bound buffer_capacity rounds"
      " producer_min_delay producer_max_delay"
      " consumer_min_delay consumer_max_delay\n");
      error = ERR_NO_ARGS;
  }
  return error;
}

int create_threads(shared_data_t* shared_data) {
  assert(shared_data);
  int error = EXIT_SUCCESS;

  pthread_t producer, consumer;
  error = pthread_create(&producer, /*attr*/ NULL, produce, shared_data);
  if (error == EXIT_SUCCESS) {
    error = pthread_create(&consumer, /*attr*/ NULL, consume, shared_data);
    if (error != EXIT_SUCCESS) {
      fprintf(stderr, "error: could not create consumer\n");
      error = ERR_CREATE_THREAD;
    }
  } else {
    fprintf(stderr, "error: could not create producer\n");
    error = ERR_CREATE_THREAD;
  }

  if (error == EXIT_SUCCESS) {
    pthread_join(producer, /*value_ptr*/ NULL);
    pthread_join(consumer, /*value_ptr*/ NULL);
  }

  return error;
}

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data; // NOLINT
  size_t count = 0;
  for (size_t round = 0; round < shared_data->rounds; ++round) {
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) {
      // wait(can_produce)
      /** @note 
       *  En esta función el semaforo es sem_wait(&shared_data->can_produce)
       *  Explicación: Este semáforo no puede avanzar si el buffer está lleno
       *  cada vez que se produce se le resta uno al semáforo del productor
       *  si el semáforo del productor queda en 0, este no puede seguir produciendo.
       *  En ese caso debe esperar a que el consumidor, consuma al menos 1 producto para 
       *  que a su semáforo se le sume uno y pueda seguir produciento.
       *  En esta otra parte se usa sem_post(&shared_data->can_consume);
       *  esto lo que hace es si produce algo, se le suma uno al semáforo del consumidor
       *  indicando que hay un producto por consumir.
       */
      sem_wait(&shared_data->can_produce);

      /** @note usleep(1000 * random_between(shared_data->producer_min_delay
        , shared_data->producer_max_delay))
       * con esto lo que se hace es esperar un milisegundo, número generado por
       * la función random_between() y se envían variables descritas en el struct shared_data
       */
      usleep(1000 * random_between(shared_data->producer_min_delay
        , shared_data->producer_max_delay));
      shared_data->buffer[index] = ++count;
      printf("Produced %lg\n", shared_data->buffer[index]);

      // signal(can_consume)
      sem_post(&shared_data->can_consume);
    }
  }

  return NULL;
}

void* consume(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data; // NOLINT
  for (size_t round = 0; round < shared_data->rounds; ++round) {
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) {
      // wait(can_consume)
      /** @note 
       *  En esta función el semaforo es sem_wait(&shared_data->can_consume)
       *  Explicación: Este semáforo no puede avanzar si el buffer está vacío ya que este semáforo empieza en 0
       *  cada vez que se produce se le suma uno al semáforo del consumidor el valor de 0 cambia a 1
       *  indicandole a esta función que el buffer tiene algo disponible para extraer(consumir)
       *  una vez este consume, le suma uno al semáforo del productor sem_post(&shared_data->can_produce);, 
       *  indicandole que ha consumido algo
       *  que puede volver a producir.
       *  En el caso de que no se haya producido nada este semáforo se mantiene en "rojo".
       */
      sem_wait(&shared_data->can_consume);

      /** @note usleep(1000 * random_between(shared_data->consumer_min_delay
        , shared_data->consumer_max_delay))
       * con esto lo que se hace es esperar un milisegundo, número generado por
       * la función random_between() y se envían variables descritas en el struct shared_data
       */
      double value = shared_data->buffer[index];
      usleep(1000 * random_between(shared_data->consumer_min_delay
        , shared_data->consumer_max_delay));
      printf("\tConsumed %lg\n", value);

      // signal(can_produce)
      sem_post(&shared_data->can_produce);
    }
  }

  return NULL;
}

useconds_t random_between(useconds_t min, useconds_t max) {
  return min + (max > min ? (random() % (max - min)) : 0);
}
