# Serial homework

## Descripción

Este proyecto implementa una simulación de láminas para la transferencia del calor utilizando el lenguaje de programación C. La simulación incluye la lectura de archivos, manipulación de matrices, y la generación de reportes. Además, de la utilización argumentos de línea de comandos para personalizar la ejecución del programa.

El problema principal radica en la implementación de una simulación que no solo sea precisa, sino también eficiente en términos de tiempo y recursos. Además, es crucial que el programa sea capaz de manejar grandes volúmenes de datos y generar resultados que puedan ser fácilmente interpretados.

## Requisitos para correr el programa

Se necesita un compilador de C (como gcc) y preferiblemente estar en un entorno de linux

## Instalación

Se debe de clonar el repositorio simplemente escriba en la terminal: git clone <https://github.com/CastilloSebas2005/concurrente24b-sebastian_orozco.git>

## Manual de uso del programa

Debido a un problema con el comando "make test", las pruebas se deben correr manualmente, a continuación se dará toda la explicación necesaria.

1. Asegurese de estar en la siguiente ruta: concurrente24b-sebastian_orozco/homeworks/serial
2. Escriba el comando: make
3. Escriba en la terminal: ./bin/serial job002.txt tests output (en este caso se eligió el archivo de pruebas 2)

Este comando anterior porque el programa recibe un archivo jobxxx.txt, la carpeta donde está ese archivo(importante para que pueda encontrar la ruta) y la carpeta donde se va a guardar el archivo de salida. Asegurese de borrar o limpiar el contenido del archivo job002.tsv cada vez que quiera correr el programa, no porque se vaya a sobreescribir, si no que se usa el argumento "a+" que lo que hace es escribir al final del archivo. Por lo tanto debe asegurarse de limpiarlo. El programa ya se comparó y da la salida esperada, solo que el formato dependiendo de los decimales no lo acomoda tan bien. También asegúrese que su archivo job esté bien escrito y que no tenga espacios en blanco, debido a que si pasa esto, el programa lo detectará como un error y va a reportar en el error estándar Error: line of jobfile is blank or corrupt (Está hecho así para evitar archivos dañados o con contenido extraño).

## Créditos

Este proyecto fue desarrollado con la ayuda de Microsoft Copilot, específicamente en el proceso de depuración y mejora del código.
