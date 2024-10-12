# Enunciado

Modifique el programa de Ejemplo 35 para realizar tres etapas o fases de iteración. Es decir, tres ciclos for independientes uno tras otro. En cada ciclo se realiza la misma cantidad de iteraciones.

Optimice el programa para reutilizar los hilos. Es decir, en cada ciclo no cree ni destruya hilos, sino que créelos antes del primer ciclo y destrúyalos después del tercer ciclo. En cada ciclo los hilos son reutilizados.

El programa debe imprimir en la salida estándar la etapa en la que se encuentra, el número de hilo y las iteraciones que realizó. Las etapas siempre deben aparecer en orden y separadas entre ellas por un único cambio de línea. Por ejemplo, para repartir 5 iteraciones entre 2 hilos en las tres etapas, una salida podría ser:

$ ./several_for 2 5
stage 1: 0/2: iteration 0/5
stage 1: 1/2: iteration 3/5
stage 1: 1/2: iteration 4/5
stage 1: 0/2: iteration 1/5
stage 1: 0/2: iteration 2/5

stage 2: 1/2: iteration 3/5
stage 2: 1/2: iteration 4/5
stage 2: 0/2: iteration 0/5
stage 2: 0/2: iteration 1/5
stage 2: 0/2: iteration 2/5

stage 3: 1/2: iteration 3/5
stage 3: 1/2: iteration 4/5
stage 3: 0/2: iteration 0/5
stage 3: 0/2: iteration 1/5
stage 3: 0/2: iteration 2/5
