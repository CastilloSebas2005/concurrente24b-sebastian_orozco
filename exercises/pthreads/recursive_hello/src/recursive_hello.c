// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>

/**
 * @brief ...
 */
void* greet(void* data);

// procedure main:
int main(void) {
  // create_thread(greet)
  pthread_t thread;
  size_t number = 2;
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ &number);
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from main thread\n");
    pthread_join(thread, /*value_ptr*/ NULL);
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // end procedure

// procedure greet:
void* greet(void* data) {
  //se hace la conversión de datos para no afectar la firma de pthreads y que el programa no falle
  size_t number = *(size_t*)(data); 
  //se define el hilo que se usará posteriormente
  if(number == 0){ 
    printf("Good bye from other thread\n");
    //se hace el return para que se salga de la función cuando el valor es 0
    return NULL;
  }else if(number > 0){
    printf("Hello from thread %li\n", number);
    number--;
    //se crea un nuevo hilo y se llama a la función greet con un valor menor
    pthread_t thread;
    int error = pthread_create(&thread, NULL, greet, &number);
    if (error == EXIT_SUCCESS) {
      pthread_join(thread, NULL);
    }
    //se llama a la función greet con un valor menos para number
  }
  return NULL;
}  // end procedure