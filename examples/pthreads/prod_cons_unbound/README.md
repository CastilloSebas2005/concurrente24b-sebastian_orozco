# Enunciado

Modifique la simulación del problema productor-consumidor generalizando las siguientes restricciones.

El buffer no es acotado, sino de tamaño arbitrario o dinámico, definido por los límites de memoria del sistema.

No existe el concepto de ronda. El primer argumento de línea de comandos indica la cantidad de productos a simular. Identifique los productos en orden secuencial.

Varios productores, indicados por el usuario en el segundo argumento de línea de comandos.

Varios consumidores, indicados por el usuario en el tercer argumento de línea de comandos.

Cada productor y consumidor indica su número en la salida estándar.

El siguiente ejemplo de ejecución simula la construcción de 5 unidades de trabajo por 2 productores que tardan entre 0 y 100 milisegundos en producir, y 3 consumidores que tardan entre 0 y 750 milisegundos en consumir.

$ ./producer_consumer 5 2 3 0 100 0 750
0 produced 1
1 produced 3
                1 consuming 1
0 produced 2
                2 consuming 3
0 produced 5
                0 consuming 5
                1 consuming 2
1 produced 4
                2 consuming 4
Note que no necesariamente los productos se generan en orden secuencial, debido al indeterminismo de los productores. Recuerde, el problema está resuelto si todos los consumidores procesan todos los productos en el mismo orden en que los productores los generan. Puede usar el siguiente código modularizado como punto de partida para su solución:
