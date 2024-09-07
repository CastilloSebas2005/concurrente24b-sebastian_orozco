# Enunciado

Ejecute al menos tres veces los códigos de Ejemplo 7 (imprimir en orden con semáforos) y Ejemplo 8 (imprimir en orden con seguridad condicional) con tantos hilos como su sistema operativo permite.

Anote las tres duraciones de cada solución. Tome la mayor duración de las tres corridas de cada versión. Agregue los resultados a sus cuadros y gráficos del Ejercicio 14 [delayed_busy_wait].

Si en un proyecto con sus clientes usted tuviere que implementar una de las cuatro soluciones para hacer un trabajo en orden ¿Cuál solución escogería? Agregue además un párrafo a la discusión de Ejercicio 14 [delayed_busy_wait] explicando la potencial causa de este comportamiento.

## Duraciones de delayed_busy_wait

Nota: Se tomaron los tiempos más pequeños para hacer la comparación.l
1µs: pseudoaleatorio(Execution time: 0.001210494s) y constante(Execution time: 0.001560078s)

## Duraciones del ejemplo 7

1. Tiempo 1: 0.895485400
1. Tiempo 2: 0.876772600
1. Tiempo 3: 0.844265000

## Duraciones del ejemplo 8

1. Tiempo 1: 1.767811900
1. Tiempo 2: 1.493220700
1. Tiempo 3: 1.395148000

## Duraciones del ejemplo delayed_busy_wait tiempo constante

1. Tiempo 1: 1.694622100
1. Tiempo 2: 1.550308700
1. Timepo 3: 1.360802800

## Duraciones del ejemplo delayed_busy_wait tiempo pseudoaleatorio

1. Tiempo 1: 1.497360700
1. Tiempo 2: 1.354566400
1. Tiempo 3: 1.330442700
