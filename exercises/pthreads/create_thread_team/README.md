# enunciado

Se introducen dos procedimientos al pseudocódigo para crear y esperar por equipos de hilos:

procedure main():
  shared thread_count := 5
  declare team := create_threads(thread_count, routine)
  ...
  join_threads(team)
end procedure

procedure routine(thread_number)
  print("I am thread ", thread_number, " of ", thread_count)
end procedure
El procedimiento create_threads() crea un equipo de hilos, todos ejecutando el procedimiento routine() concurrentemente, y retorna un arreglo de valores que representan hilos de ejecución. De forma análoga, el procedimiento join_threads() recibe el arreglo de hilos y espera a que todos ellos finalicen su ejecución.

Contar con estos dos procedimientos reutilizables es sumamente útil para la implementación de equipos de hilos en el paralelismo de datos. Las siguientes son implementaciones iniciales usando Pthreads:

pthread_t* create_threads(size_t count, void*(*routine)(void*), void* data) {
  pthread_t* threads = (pthread_t*) calloc(count, sizeof(pthread_t));
  if (threads) {
    for (size_t index = 0; index < count; ++index) {
      if (pthread_create(&threads[index], NULL, routine, data) != 0) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        join_threads(index, threads);
        return NULL;
      }
    }
  }
  return threads;
}

int join_threads(size_t count, pthread_t* threads) {
  int error_count = 0;
  for (size_t index = 0; index < count; ++index) {
    const int error = pthread_join(threads[index], NULL);
    if (error) {
      fprintf(stderr, "error: could not join thread %zu\n", index);
      ++error_count;
    }
  }
  free(threads);
  return error_count;
}
Las implementaciones anteriores no proveen a los hilos secundarios su número de hilo en el equipo. Modifique la subrutina create_threads() para que cree en la memoria dinámica e inicialice un arreglo de registros de datos privados. Cada registro tiene cuatro valores: el número de hilo (obtenido del índice de un ciclo), la cantidad de hilos en el equipo de hilos, el puntero a los datos compartidos (data, recibido por parámetro), y el identificador del hilo (pthread_t) inicializado con pthread_create(). Envíe la dirección del registro privado a cada hilo al crearlo con pthread_create(). Finalmente create_threads() retorna la dirección de este arreglo de registros privados en lugar del arreglo de identificadores de hilos (pthread_t).

Modifique a join_threads() para que reciba la cantidad de hilos y la dirección al arreglo de registros privados (en lugar del arreglo de identificadores de hilos). Luego esperará por cada hilo con pthread_join() usando el identificador del hilo en los datos privados. Finalmente libera el arreglo de datos privados.

Con estos cambios, los hilos podrán saber su número de hilo (thread_number) y la cantidad de hilos que hay en el equipo (thread_count) accediendo a los campos del registro privado.

Aplique estas subrutinas a un ejemplo o ejercicio que utilice equipos de hilos, como Ejemplo 2 o [team_shot_put].

# como compilar

cc -g -Wall -Wextra create_thread_team.c -o create_thread_team -pthread(importante estar en la carpeta de src) o simplemente use el comando make ./bin/recursive_hello

# análisis

en este caso se probo el código con el de hello_w, así que las partes que se copiaron de ese código no se comentaron, cabe recalcar que se tuvo que modificar este código para que se adaptara a las funciones implementadas, en este caso la salida es exactamente igual que en el código original, sin embargo acá se está usando una estructura de datos diferentes y me parece interesante que con esta estructura se pueda sacar más información, por ejemplo, se puede sacar en cualquier momento el total de hilos en el equipo, o el número de hilo que se está ejecutando en el momento.
