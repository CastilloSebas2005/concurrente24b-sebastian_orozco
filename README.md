# concurrente24b-Sebastian_Orozco

## Repositorio para programación paralela y concurrente

## Glossary

### 1. Programación serial(secuencial)

Secuencia de instrucciones ejecutadas una detrás de otra o en orden secuencial, en este mundo no se hacen cosas de forma simultánea. Por ejemplo si tengo 1,2,3,4,5(suponiendo que estas sean instrucciones o tareas de un programa) primero se ejecuta la instrucción 1, cuando esta termina se ejecuta la instrucción 2 y así sucesivamente hasta que se acaben los números(llegar al 5).

### 2. Programación concurrente

Es la programación que no es secuencial, hay varias formas de hacer programación concurrente, no necesariemente las tareas se tienen que ejecutar simultaneamente, por ejemplo: el sistema operativo ejecuta varias tareas al "mismo tiempo" pero esto no es del todo cierto, si no que el sistema operativo le dedica cierta cantidad de tiempo a cada tarea haciendo que parezca que se están ejecutando al mismo tiempo, el sistema operativo es el que gestiona esto.

### 3. Programación paralela

Es una forma de hacer programación concurrente, en este tipo de programación las instrucciones o tareas se ejecutan al mismo tiempo. Por ejemplo las tareas o instrcciones 1,2 y 3 se están ejecutando al mismo tiempo o simultaneamente, esto esta relacionado con los nucleos del procesador, generalmente se ejecutan en diferentes nucleos, sin embargo se están realizando al mismo tiempo.

### 4. Concurrencia de tareas

Separación de asuntos o tareas, hilos realizando trabajos distintos.
Capacidad de gestionar varias actividades simultáneamente en un entorno computacional.
Diversas tareas pueden progresar aparentemente al mismo tiempo, aunque no necesariamente de manera simultánea.

### Paralelismo de datos

Subdividir el conjunto de datos de entrada a un programa, de manera que a cada hilo le corresponda un subconjunto de esos datos.
Se busca dividir el trabajo en porciones más pequeñas y ejecutar esas porciones al mismo tiempo utilizando múltiples recursos de procesamiento.

### Hilo de ejecución

Es un arreglo de valores que el SO carga en los registros del CPU y permiten ejecutar código en ella. También se les llama trabajadores o hilos.
Mientras que un programa puede tener un solo hilo (proceso único), la implementación de múltiples hilos permite la ejecución concurrente de diferentes partes del código.

### Indeterminismo

No se sabe que va a pasar, por ejemplo la programación serial se ejecuta de una forma determinísta, se sabe que la instrucción 1 se ejecuta de primero siempre, luego la 2. Con el indeterminismo es lo contrario, no sigue ese orden, puede ser que se ejecute primero la instrucción 3 y luego la 2.
Como los hilos trabajan de manera independiente, no se sabe cuando cada hilo va a comenzar su trabajo o cuando va a terminarlo.

### Memoria privada y compartida

memoria privada: espacio de memoria en que nadie puede acceder, sin mi permiso o el permiso del sistema operativo. Esta se le asigna a los hilos como un struct y cada hilo tiene su memoria privada.

memoria compartida: espacio de memoria que todos pueden acceder, en este caso, todos los hilos tienen acceso a ella, se define como un atributo dentro de la memoria privada, pero todos los hilos pueden acceder a esta memoria compartida y es la misma para cada hilo.

### Espera activa

Proceso donde repetidamente se verifica una condición, esto gasta el tiempo de CPU. Es como estar preguntando "¿ya puedo pasar?"
Ciclo que hace a un hilo de ejecución esperar repetitivamente hasta que una condición se haga falsa.

### Condición de carrera

Cuando dos o más hilos o procesos quieren acceder al mismo recurso compartido al mismo tiempo y están compitiendo por ese recurso, es como una carrera, el primero que llegue es el que gana, depende del SOS.
Requisitos para condiciones de carrera:

1. concurrencia

1. recursos compartidos

1. escritura en memoria

### Control de concurrencia

Mecanismos que permiten controlar la concurrencia, controlando el tiempo de ejecución, un ejemplo de esto es un semáforo, este evita que un hilo haga su trabajo a menos que tenga su respectiva señal de que ya puede pasar a hacerlo.

### Seguridad condicional

Es usar memoria compartida sin embargo no todos pueden usarla al mismo tiempo, es como tener una piscina con carriles, todos pueden estar en ella, nadando en diferentes lugares, es útil porque es para evitar condiciones de carrera.

Es evitar el control de la concurrencia, utilizando memoria por ejemplo matrices, arreglos, etc...

### Exclusión mutua

Actividad que realiza el SO para evitar que dos o más procesos accedan al mismo tiempo a un área de datos, esto es basicamente lo que hace un mutex o candado, el mutex evita que dos o más hilos accedan a la memoria compartida al mismo tiempo. En nuestro caso, son herramientas para evitar que los hilos accedan a regiones de datos al mismo tiempo.

### Semáforo

Un semáforo es un entero con signo que se puede incrementar (signal) y decrementar (wait). Si el signo es negativo, significa que tiene que esperar para poder seguir ejecutando su tarea (el número indica cuantos están esperando) y si es positivo, indica que puede seguir ejecutando sin ninguna espera (el número indica cuantos hilos pueden pasar o seguir hasta que se "ponga en rojo" el semáforo).

### Barrera

No visto formalmente.

### Variable de condición

No visto formalmente

### Candado de lectura y escritura

No visto formalmente

### Descomposición

Proceso de dividir una tarea o conjunto de datos en unidades más pequeñas y manejables para su procesamiento concurrente. Esto implica particionar conjuntos de datos entre múltiples hilos o procesos para que cada uno maneje una porción independiente. Se puede ver como si se tuviera una tarea principal que se puede dividir en subtareas más y más pequeñas, es como armar un rompecabezas, se tienen varias piezas que se unen hasta armar una imagen o forma completa. "Divide y vencerás".

### Mapeo

Asignación de tareas o datos a recursos de cómputo para aprovechar al máximo el paralelismo disponible. Implica una planificación cuidadosa de la distribución de la carga de trabajo entre los diferentes procesadores, núcleos o nodos en un sistema paralelo o distribuido.

### Incremento de velocidad

Beneficio obtenido al dividir una tarea entre múltiples recursos de procesamiento. Se mide mediante el cociente entre el tiempo que tarda un programa en ejecutarse secuencialmente y el tiempo que toma ejecutar la versión paralela del mismo programa.

### Comunicación punto a punto entre procesos

No visto formalmente

### Comunicación colectiva entre procesos

No visto formalmente

### Reducción

No visto formalmente

Importante: Los conceptos son vistos durante las clases con el profesor Daniel Alvarado, también me apoye en los videos de Jeisson y google.
