# Espera activa con retraso. ¿Se corrige el problema de la espera activa si en lugar de tener un ciclo vacío, se espera un cierto tiempo? Copie su carpeta ejemplos/pthreads/hello_order_busywait a ejercicios/pthreads/delayed_busy_wait. Permita que el usuario pueda invocar su programa dos argumentos de línea de comandos: la cantidad de hilos a crear, y la cantidad de microsegundos a esperar cuando no es el turno del hilo de ejecución

Se corrige, pero no es una solución optima, sigue siendo una espera activa disfrazada

## Recuerde probar la calidad de su código (sanitizers, linter). Luego ejecute al menos tres veces su solución (sin sanitizers) con la cantidad máxima de hilos de ejecución que su sistema operativo le permite crear y una espera de 50µs. Anote la mayor de las duraciones que obtuvo de sus corridas, . ¿Hubo una mejora de la espera constante respecto a la espera activa pura?

Execution time sin uslep: 0.001406400s cantidad de tiempo sin la mejora en la espera activa(sin usar usleep) con 12 hilos que es los que tiene mi pc

Execution time con uslep de 50 microsegundos: 0.001220100s, 0.001393132s, 0.001497957, el tiempo no mejoró mucho con esta implementación, sin embargo al aumentar la cantidad de hilos, sí se ve una mejora notable, ya no se queda tanto en cada iteración.

## Ejecute al menos tres veces su solución pseudoaleatoria con la cantidad máxima de hilos y un máximo de espera de 50µs. Tome la mayor de las duraciones. ¿Hubo una mejora de la espera pseudoaleatoria respecto a la espera constante?

Execution time: 0.001397034s este fue el tiempo mayor, como se puede ver, sí hubo una mejoría respecto a la espera constante

## Comparación de las esperas. ¿Mejora el tiempo de ejecución de los dos tipos de esperas (constante y pseudoaleatoria) si disminuye o incrementa la espera máxima de los hilos?

Haga al menos un par de ejecuciones con al menos los siguientes tiempos de espera:

Nota: voy a tomar el mayor tiempo de espera luego de 3 ejecuciones

1µs: pseudoaleatorio(Execution time: 0.001210494s) y constante(Execution time: 0.001560078s)

5µs: pseudoaleatorio(Execution time: 0.001334711s) y constante(Execution time: 0.001625875s)

25µs: pseudoaleatorio(Execution time: 0.001682210s) y constante(Execution time: 0.001723518s)

50µs: pseudoaleatorio(Execution time: 0.001756862s) y constante(Execution time: 0.001773143s)

75µs: pseudoaleatorio(Execution time: 0.001734027s) y constante(Execution time: 0.001862913s)

100µ: pseudoaleatorio(Execution time: 0.001936062s) y constante(Execution time: 0.002068963s)

como se puede ver en las comparaciones, el tiempo de espera pseudoaleatorio es ligeramente mejor, no es mucho mejor, pero mejora un poco los tiempos de espera.

## Cree una gráfica donde el eje-x son las duraciones dadas por argumento al programa. El eje-y son los tiempos de ejecución de los programas. El gráfico tendrá dos series, una para la espera constante y otra para la espera pseudoaleatoria

![Tiempo Constante y Tiempo Pseudoaleatorio](design/Tiempo%20Constante%20y%20Tiempo%20Pseudoaleatorio.png)

## Agregue la gráfica al readme.md del ejercicio y una discusión de a lo sumo dos párrafos. Indique cuál fue el mejor tiempo máximo de espera obtenido y los efectos de disminuir o incrementarlo. Conjeture y trate de explicar por qué ocurre este comportamiento. Finalmente indique si la espera activa con retraso es una solución óptima, y en caso negativo, provea una idea que pueda lograr este ideal

El mejor tiempo máximo de espera obtenido fue de 0.001936062s y este se hizo con la parte pseudoaleatoria, sin embargo si se ve la gráfica, no es demasiada la diferencia al disminuir el tiempo de los microsegundos, tampoco hay demasiada diferencia entre los metodos constante y pseudoaleatorio, incluso sin estos métodos el tiempo no cambia demasiado, usando una espera activa sin estos, si bien el programa va más rápido, no es mucha la diferencia, entiendo que con este método se quieran mitigar las condiciones de carrera, sin embargo no es el método más optimo, pienso que usar un mutex es mejor opción y creo que es más rápido que estar en un ciclo que puede volverse casi infinito y hacer el programa mucho más lento. Pienso que hay mejores formas de esperar a los hilos que pueden ser menos riesgosas, pero también pienso que esperar hilos se use el método que se use va a significar algún sacrificio de cara al tiempo.

Nota: yo hice el programa con las dos técnicas indicadas, sin embargo lo dejé con la primera debido a que fue la última que probé de cara a los tiempos, por eso la que aparece en el código es la espera activa con la técnica de constate. También está el doxyfile, sin embargo este es el mismo que el del ejemplo hello_order_busywait.
