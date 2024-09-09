# Enunciado

Cree una simulación del productor-consumidor. Su programa recibirá seis argumentos en la línea de comandos:

El tamaño del buffer.

La cantidad de rondas, vueltas, o veces que se debe llenar y vaciar el buffer.

La duración mínima en milisegundos que el productor tarda en generar un producto.

La duración máxima en milisegundos que el productor tarda en generar un producto.

La duración mínima en milisegundos que el consumidor tarda en consumir un producto.

La duración máxima en milisegundos que el consumidor tarda en consumir un producto.

El programa debe crear un buffer para almacenar productos identificados por números secuenciales iniciando en 1. Por ejemplo, si se piden 2 rondas con un buffer de tamaño 3, el 4 identificará el primer producto de la segunda ronda.

Generar un producto no es inmediato. El tiempo que el productor tarda depende de la naturaleza del contexto en que se aplique el problema del consumidor-productor. Para efectos de la simulación, los argumentos 3 y 4 sirven de rango para generar números pseudoaleatorios. Cada vez que el productor tiene que construir un producto, se generará una duración pseudoaletoria y el productor esperará esta cantidad de milisegundos, simulando hasta que el producto esté acabado. Luego el productor agrega el producto al buffer e imprime en la salida estándar el texto i generated, donde i es el número de producto.

Una vez que el consumidor extrae un producto del buffer imprime en la salida estándar el texto Consuming i y se dispone a consumir. Consumir un producto no es inmediato. El tiempo que el consumidor tarda se genera también en forma pseudoaletoria con los argumentos 5 y 6. Sugerencia: indente la salida del consumidor para distinguirla de la salida del productor.

En el siguiente ejemplo de ejecución, los entes hacen dos rondas llenando y consumiendo un buffer de 3 elementos. El productor es rápido y tarda máximo 100ms creando un producto. El consumidor es más lento y podría tardar máximo 750ms consumiendo un producto. Como se puede ver en la salida, el productor rápidamente llena el buffer y debe esperar a que el consumidor libere espacio en el buffer para continuar produciendo. En la salida se comprueba que el consumidor procesa todos los datos generados por el productor y en el mismo orden.

$ ./producer_consumer 3 2 0 100 0 750
Produced 1
Produced 2
Produced 3
        Consuming 1
Produced 4
        Consuming 2
Produced 5
        Consuming 3
        Consuming 4
        Consuming 5
Produced 6
        Consuming 6
Puede usar el siguiente código no modularizado como punto de partida para su solución:
