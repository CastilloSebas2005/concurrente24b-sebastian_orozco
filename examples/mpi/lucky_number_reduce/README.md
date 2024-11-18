# Enunciado

Escriba un programa donde cada proceso escoge al azar su número de la suerte entre 00 y 99 inclusive. Haga que el proceso 0 reporte el mínimo, el promedio, y el máximo de los números de la suerte escogidos entre todos los procesos. El siguiente puede ser un ejemplo de ejecución:

$ mpiexec -np 5 -f hosts.txt bin/lucky_number_reduce
Process 0: my lucky number is 83
Process 0: all minimum: 10
Process 0: all average: 44.20
Process 0: all maximum: 83
Process 1: my lucky number is 56
Process 2: my lucky number is 10
Process 3: my lucky number is 10
Process 4: my lucky number is 62
Para su solución y generar números pseudoaleatorios, puede usar el siguiente código base en C++.
