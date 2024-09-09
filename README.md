# concurrente24b-Sebastian_Orozco

## Repositorio para programación paralela y concurrente

## Glossary

Programación serial(secuencial): Secuencia de instrucciones ejecutadas una detrás de otra o en orden secuencial, en este mundo no se hacen cosas de forma simultánea. Por ejemplo si tengo 1,2,3,4,5(suponiendo que estas sean instrucciones o tareas de un programa) primero se ejecuta la instrucción 1, cuando esta termina se ejecuta la instrucción 2 y así sucesivamente hasta que se acaben los números(llegar al 5).

Programación concurrente: Es la programación que no es secuencial, hay varias formas de hacer programación concurrente, no necesariemente las tareas se tienen que ejecutar simultaneamente, por ejemplo: el sistema operativo ejecuta varias tareas al "mismo tiempo" pero esto no es del todo cierto, si no que el sistema operativo le dedica cierta cantidad de tiempo a cada tarea haciendo que parezca que se están ejecutando al mismo tiempo, el sistema operativo es el que gestiona esto.

Programación paralela: Es una forma de hacer programación concurrente, en este tipo de programación las instrucciones o tareas se ejecutan al mismo tiempo. Por ejemplo las tareas o instrcciones 1,2 y 3 se están ejecutando al mismo tiempo o simultaneamente, esto esta relacionado con los nucleos del procesador, generalmente se ejecutan en diferentes nucleos, sin embargo se están realizando al mismo tiempo.

Concurrencia de tareas:

Paralelismo de datos:

Hilo de ejecución:

Indeterminismo: No se sabe que va a pasar, por ejemplo la programación serial se ejecuta de una forma determinísta, se sabe que la instrucción 1 se ejecuta de primero siempre, luego la 2. Con el indeterminismo es lo contrario, no sigue ese orden, puede ser que se ejecute primero la instrucción 3 y luego la 2.

Memoria privada y compartida: memoria privada; espacio de memoria en que nadie puede acceder, sin mi permiso o el permiso del sistema operativo.

Espera activa: proceso donde repetidamente se verifica una condición, esto gasta el tiempo de CPU. Es como estar preguntando "¿ya puedo pasar?"

Condición de carrera: cuando dos o más hilos o procesos quieren acceder al mismo recurso compartido al mismo tiempo y están compitiendo por ese recurso, es como una carrera, el primero que llegue es el que gana, depende del SOS.
Requisitos para condiciones de carrera:

1. concurrencia

1. recursos compartidos

1. escritura en memoria

Control de concurrencia:

Seguridad condicional: Es usar memoria compartida sin embargo no todos pueden usarla al mismo tiempo, es como tener una piscina con carriles, todos pueden estar en ella, nadando en diferentes lugares, es útil porque es para evitar condiciones de carrera.

Exclusión mutua:

Semáforo:

Barrera:

Variable de condición:

Candado de lectura y escritura:

Descomposición:

Mapeo:

Incremento de velocidad:

Comunicación punto a punto entre procesos:

Comunicación colectiva entre procesos:

Reducción:
