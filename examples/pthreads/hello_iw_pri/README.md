# enunciado
Cree una nueva versión de su programa "hola mundo" donde cada thread imprime "Hello from secondary thread i of w", donde i es el número de thread y w la cantidad total de hilos creada. Está prohibido usar variables en el segmento de datos (globales, estáticas, externas). Cree un registro de memoria (estructura) privado para cada thread.

# análisis
No pude adivinar el resultado debido al indeterminismo que hay al usar hilos, me parece interesante como la estructuras de datos pueden ayudarnos a usar hilos con memoria privada.

Me parece interesante el uso de calloc para generar la memoria ya "limpia" para usar las estructuras de datos y no usar malloc ya que se pueden generar errores