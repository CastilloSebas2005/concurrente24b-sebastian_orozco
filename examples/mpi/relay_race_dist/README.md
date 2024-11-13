# Enunciado

Simule una carrera de relevos, donde los competidores son procesos en equipos. Cada equipo consta de dos corredores. Un corredor tomará la estafeta (en inglés, baton) y se posicionará en la línea de salida. El otro corredor esperará en la mitad de la pista a que el primer corredor llegue y le transfiera la estafeta. En tal momento, el segundo corredor seguirá a toda velocidad hasta alcanzar la meta.

Usted puede imponer restricciones sobre la cantidad de procesos con los que deberá ser ejecutada la simulación. Por ejemplo, un número mínimo de procesos que puede ser par o impar de acuerdo a sus decisiones de separación de asuntos. Idee un mecanismo para decidir qué procesos pertenecen al mismo equipo. Por ejemplo, los procesos 1 y 2 podrían conformar el equipo 1, los procesos 3 y 4 al equipo 2, y así sucesivamente. Si se invoca con una cantidad no adecuada de procesos, la simulación deberá detenerse y emitir un mensaje de error que ayude al usuario a escoger el número correcto.

Cada proceso recibirá dos parámetros: la duración en milisegundos que tarda el primer corredor atravesando la etapa 1 de la carrera (entre la salida y la mitad de la pista), y la duración en milisegundos del segundo corredor atravesando la etapa 2 de la carrera (de la mitad de la pista hasta la meta). Suponga que los corredores de una etapa tardan exactamente esta duración y no un valor generado al azar. Ejemplo de ejecución:

$ mpiexec -np 10 -f hosts.txt bin/relay_race_dist 1100 900

Place 1: team 3 in 2.35075s
Place 2: team 4 in 2.35085s
Place 3: team 5 in 2.35087s
Place 4: team 2 in 2.35089s
Place 5: team 1 in 2.35092s
