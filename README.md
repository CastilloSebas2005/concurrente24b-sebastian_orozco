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

### 5. Paralelismo de datos

Subdividir el conjunto de datos de entrada a un programa, de manera que a cada hilo le corresponda un subconjunto de esos datos.
Se busca dividir el trabajo en porciones más pequeñas y ejecutar esas porciones al mismo tiempo utilizando múltiples recursos de procesamiento.

### 6. Hilo de ejecución

Es un arreglo de valores que el SO carga en los registros del CPU y permiten ejecutar código en ella. También se les llama trabajadores o hilos.
Mientras que un programa puede tener un solo hilo (proceso único), la implementación de múltiples hilos permite la ejecución concurrente de diferentes partes del código.

### 7. Indeterminismo

No se sabe que va a pasar, por ejemplo la programación serial se ejecuta de una forma determinísta, se sabe que la instrucción 1 se ejecuta de primero siempre, luego la 2. Con el indeterminismo es lo contrario, no sigue ese orden, puede ser que se ejecute primero la instrucción 3 y luego la 2.
Como los hilos trabajan de manera independiente, no se sabe cuando cada hilo va a comenzar su trabajo o cuando va a terminarlo.

### 8. Memoria privada y compartida

memoria privada: espacio de memoria en que nadie puede acceder, sin mi permiso o el permiso del sistema operativo. Esta se le asigna a los hilos como un struct y cada hilo tiene su memoria privada.

memoria compartida: espacio de memoria que todos pueden acceder, en este caso, todos los hilos tienen acceso a ella, se define como un atributo dentro de la memoria privada, pero todos los hilos pueden acceder a esta memoria compartida y es la misma para cada hilo.

### 9. Espera activa

Proceso donde repetidamente se verifica una condición, esto gasta el tiempo de CPU. Es como estar preguntando "¿ya puedo pasar?"
Ciclo que hace a un hilo de ejecución esperar repetitivamente hasta que una condición se haga falsa.

### 10. Condición de carrera

Cuando dos o más hilos o procesos quieren acceder al mismo recurso compartido al mismo tiempo y están compitiendo por ese recurso, es como una carrera, el primero que llegue es el que gana, depende del SOS.
Requisitos para condiciones de carrera:

1. concurrencia

1. recursos compartidos

1. escritura en memoria

### 11. Control de concurrencia

Mecanismos que permiten controlar la concurrencia, controlando el tiempo de ejecución, un ejemplo de esto es un semáforo, este evita que un hilo haga su trabajo a menos que tenga su respectiva señal de que ya puede pasar a hacerlo.

### 12. Seguridad condicional

Es usar memoria compartida sin embargo no todos pueden usarla al mismo tiempo, es como tener una piscina con carriles, todos pueden estar en ella, nadando en diferentes lugares, es útil porque es para evitar condiciones de carrera.

Es evitar el control de la concurrencia, utilizando memoria por ejemplo matrices, arreglos, etc...

### 13. Exclusión mutua

Actividad que realiza el SO para evitar que dos o más procesos accedan al mismo tiempo a un área de datos, esto es basicamente lo que hace un mutex o candado, el mutex evita que dos o más hilos accedan a la memoria compartida al mismo tiempo. En nuestro caso, son herramientas para evitar que los hilos accedan a regiones de datos al mismo tiempo.

### 14. Semáforo

Un semáforo es un entero con signo que se puede incrementar (signal) y decrementar (wait). Si el signo es negativo, significa que tiene que esperar para poder seguir ejecutando su tarea (el número indica cuantos están esperando) y si es positivo, indica que puede seguir ejecutando sin ninguna espera (el número indica cuantos hilos pueden pasar o seguir hasta que se "ponga en rojo" el semáforo).

### 15. Barrera

Una barrera es una sincronización entre múltiples hilos o procesos en la que todos deben llegar a un punto específico antes de que cualquiera de ellos pueda continuar. Es útil para coordinar el progreso de tareas concurrentes.

### 16. Variable de condición

Una variable de condición es una herramienta de sincronización que permite a los hilos esperar hasta que una condición particular se cumpla. Se utiliza junto con un mutex para evitar condiciones de carrera.

### 17. Candado de lectura y escritura

Un candado de lectura y escritura es un mecanismo de sincronización que permite a múltiples hilos leer un recurso compartido simultáneamente, pero solo permite a un hilo escribir en el recurso a la vez. Esto mejora la concurrencia al permitir más operaciones de lectura concurrentes.

### 18. Descomposición

Proceso de dividir una tarea o conjunto de datos en unidades más pequeñas y manejables para su procesamiento concurrente. Esto implica particionar conjuntos de datos entre múltiples hilos o procesos para que cada uno maneje una porción independiente. Se puede ver como si se tuviera una tarea principal que se puede dividir en subtareas más y más pequeñas, es como armar un rompecabezas, se tienen varias piezas que se unen hasta armar una imagen o forma completa. "Divide y vencerás".

### 19. Mapeo

Asignación de tareas o datos a recursos de cómputo para aprovechar al máximo el paralelismo disponible. Implica una planificación cuidadosa de la distribución de la carga de trabajo entre los diferentes procesadores, núcleos o nodos en un sistema paralelo o distribuido.

### 20. Incremento de velocidad

Beneficio obtenido al dividir una tarea entre múltiples recursos de procesamiento. Se mide mediante el cociente entre el tiempo que tarda un programa en ejecutarse secuencialmente y el tiempo que toma ejecutar la versión paralela del mismo programa.

### 21. Eficiencia

La eficiencia en el contexto de la computación paralela y distribuida se refiere a la medida de cómo se utiliza el tiempo de procesamiento disponible en un sistema. Se calcula como el cociente entre la aceleración (speedup) obtenida al ejecutar un programa en paralelo y el número de recursos de procesamiento utilizados (como hilos o procesos). La eficiencia indica qué tan bien se están utilizando los recursos adicionales para reducir el tiempo de ejecución. Una eficiencia del 100% significa que los recursos adicionales están siendo utilizados de manera óptima, mientras que una eficiencia menor indica que hay sobrecarga o ineficiencia en la paralelización.

### 22. Comunicación punto a punto entre procesos

La comunicación punto a punto entre procesos es un método de intercambio de datos directamente entre dos procesos en un sistema paralelo o distribuido. Esto puede implicar el uso de mensajes o buffers compartidos.

### 23. Comunicación colectiva entre procesos

La comunicación colectiva entre procesos implica la participación de múltiples procesos en una operación de comunicación, como la difusión de datos desde un proceso a todos los demás (broadcast) o la recolección de datos de todos los procesos en uno solo (gather).

### 24. Reducción

La reducción es una operación colectiva en la que se combinan los datos de todos los procesos en un solo resultado utilizando una operación específica, como la suma, el máximo o el mínimo. Es común en aplicaciones de computación paralela para resumir resultados distribuidos.

Importante: Los conceptos son vistos durante las clases con el profesor Daniel Alvarado, también me apoye en los videos de Jeisson y google.
