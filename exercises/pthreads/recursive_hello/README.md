# Enunciado del ejercicio

Copie su ejemplos/pthreads/hello a ejercicios/pthreads/recursive_hello. Puede renombrar ejercicios/pthreads/recursive_hello/src/hello.c a recursive_hello.c o si lo prefiere, main.c.

Modifique a greet() para recibir un número de tipo size_t. Si este número es 0, greet() imprime una despedida y el número recibido. Luego termina su ejecución.

Si el número que greet() recibe es mayor que cero, imprime un saludo y el número recibido. Luego crea un hilo para que invoque a greet() pero con un número menor que el recibido por parámetro.

Modifique el procedimiento main() para crear una variable local de tipo size_t inicializada en 2. Envíe este número como parámetro de la subrutina greet() que correrá en el hilo secundario.

Recuerde copiar este enunciado en una sección de su documento de análisis ejercicios/pthreads/recursive_hello/readme.md y darle el formato apropiado. En otra sección haga una predicción de la salida de su programa antes de corerlo. Compílelo y córralo con el Makefile. Copie la salida de su programa y péguela en un bloque de código debajo de su predicción. Indique si acertó o no la predicción.

# Como correr

cc -g -Wall -Wextra recursive_hello.c -o recursive_hello -pthread(Importante: estar en la carpeta de src) o usar comando make y escribir ./bin/recursive_hello

# Análisis y predicción

Predije que la salida iba a ser: 
"Hello from main thread"
"Hello from thread 1"
"Hello from thread 2"
"Goodbye from other thread"

y acerté, sin embargo se me dificultó pasar un size_t como parámetro, ya que desconocía como funcionaba pthreads y su firma, por lo que tuve que ir a la documentación de pthreads e investigar esto para poder hacerlo.
Me parece valioso como puede llamarse varios hilos de forma recursiva para que realicen diferentes tareas, pero me parece algo costoso en términos de memoria.