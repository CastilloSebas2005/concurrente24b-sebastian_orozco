# Enunciado 
 Escriba un programa que al correr cree dos hilos, el principal y uno secundario. Ambos hilos deben saludar en la salida estándar con el mensaje "Hello from main thread" y "Hello from secondary thread" respectivamente. Utilice un analizador dinámico de código como las herramientas memcheck y helgrind de Valgrind, o los Google Sanitizers, para asegurarse de que su programa no provoca condiciones de carrera.
# observaciones del código
Lo destacable y "nuevo" de éste código es el uso de la biblioteca pthreads.

# Función main:

pthread_t thread; con esto lo que se hace es el tipo de dato necesario para poder usar "pthread_create" y crear el hilo y asignarle su trabajo o trabajos, aparte que como se comentó en clase, esto se va al stack frame.

int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);
Con esta linea lo que se hace es pedirle al sistema operativo que cree el hilo y se crea otro stack frame según lo visto en clase. Lo que hace esta linea es crear el hilo y se le asigna la función greed ya que lo que haga esa función va a ser "el trabajo" de ese hilo, para este caso los argumentos quedan en NULL.

pthread_join(thread, /*value_ptr*/ NULL); esta función de acá lo que hace es pedirle al hilo padre que "mate" al hilo creado anteriormente con pthread_create.

# Función greed:
void* greet(void* data);
Debe de ser así, ya que según lo que he investigado esta es la firma de la función para usar pthread_create.
void* es simplemente un número como se comentó en clase o un puntero a "nada"
y luego esta subrutina lo que hace es simplemente mostrar un "hello from secundary thread" por la salida estándar.
