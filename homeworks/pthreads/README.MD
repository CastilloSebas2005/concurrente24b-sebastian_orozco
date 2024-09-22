# Pthreads homework

## Descripción

Este proyecto implementa una simulación de láminas para la transferencia del calor utilizando el lenguaje de programación C. La simulación incluye la lectura de archivos, manipulación de matrices, y la generación de reportes. Además, de la utilización argumentos de línea de comandos para personalizar la ejecución del programa.

El problema principal radica en la implementación de una simulación que no solo sea precisa, sino también eficiente en términos de tiempo y recursos. Además, es crucial que el programa sea capaz de manejar grandes volúmenes de datos y generar resultados que puedan ser fácilmente interpretados. Para este problema se usaron hilos para su solución, la parte de hilos se hacen a la hora de hacer simulaciones, se dividen la cantidad de simulaciones que hay entre la cantidad de hilos y a cada hilo se le asigna su trabajo.

## Requisitos para correr el programa

Se necesita un compilador de C (como gcc) y preferiblemente estar en un entorno de linux

## Instalación

Se debe de clonar el repositorio simplemente escriba en la terminal: git clone <https://github.com/CastilloSebas2005/concurrente24b-sebastian_orozco.git>

## Manual de uso del programa

Debido a un problema con el comando "make test", las pruebas se deben correr manualmente, a continuación se dará toda la explicación necesaria.

1. Compilación y ejecución del programa.

    1. Asegurese de estar en la siguiente ruta: concurrente24b-sebastian_orozco/homeworks/pthreads

    1. Escriba el comando: make

        1. Si desea probar la memoria del programa deberá hacerlo con los siguiente comandos...

        1. make clean tsan (para la memoria de los hilos) luego de eso, simplemente ejecute el programa.

        1. make clean asan (para la memoria serial) luego de eso, simplemente ejecute el programa.

        1. valgrind ./bin/pthreads job002.txt tests/job002b output cantidad_hilos, sin embargo valgrind es un poco más lento que los anteriores.

    1. Escriba en la terminal: ./bin/pthreads job002.txt tests/job002b output cantidad_hilos (en este caso se eligió el archivo de pruebas 2) esto para ejecutar el programa.

1. Cosas que recibe el programa como argumentos:

    1. Nombre del archivo job, si el archivo no se llama job, el programa fallará.

    1. Carpeta donde se encuentra el archivo job, esto para poder armar la ruta correctamente, si este argumento está mal, el programa fallará.

    1. Carpeta donde se va a poner el archivo de reporte, si esta carpeta no existe o está mal, el programa dará error.

    1. Cantidad de hilos a usar, si el programa recibe algo raro, fallará, favor solo ingresar números.

    1. Ejemplo de correr el programa: ./bin/pthreads job002.txt tests/job002b output cantidad_hilos

Es importante que siempre ingrese la cantidad de hilos, puesto que si no lo hace, el programa no funcionará, si ingresa 0 en la parte de hilos, el programa automaticamente asignará la cantidad de hilos que tenga su computadora por defecto.

Si ingresa solo un hilo, el programa ignorará la creacion de hilos y se ejecutará de forma serial.
Para manejar el indeterminismo en este programa se implemento la técnica de condición segura.

Este comando anterior porque el programa recibe un archivo jobxxx.txt, la carpeta donde está ese archivo(importante para que pueda encontrar la ruta) y la carpeta donde se va a guardar el archivo de salida. El programa ya se comparó y da la salida esperada, solo que el formato dependiendo de los decimales no lo acomoda tan bien. Asegurse siempre de que su archivo job no contenga espacios en blanco para evitar errores, sin embargo esta versión del programa puede soportar espacios en blanco.

## Créditos

Este proyecto fue desarrollado con la ayuda de Microsoft Copilot, específicamente en el proceso de depuración y mejora del código.
