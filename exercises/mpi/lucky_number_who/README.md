# Enuciado

Modifique su programa de la Ejemplo 53 (lucky_number_reduce) para que cada proceso escoja al azar su número de la suerte entre 00 y 99 inclusive, y el mismo proceso reporta si su número es el menor o el mayor de los números escogidos por todos los procesos. El siguiente puede ser un ejemplo de ejecución:

$ mpiexec -np 5 -f hosts.txt bin/lucky_number_who
Process 0: my lucky number (83) is greater than the average (36.00)
Process 0: my lucky number (83) is the maximum (83)
Process 1: my lucky number (18) is less than the average (36.00)
Process 2: my lucky number (07) is less than the average (36.00)
Process 2: my lucky number (07) is the minimum (07)
Process 3: my lucky number (46) is greater than the average (36.00)
Process 4: my lucky number (26) is less than the average (36.00)
El reporte de mínimo, máximo y promedio es independiente. Por ejemplo, si sólo un proceso se ejecuta cumplirá los tres criterios:

$ mpiexec -np 1 -f hosts.txt bin/lucky_number_who
Process 0: my lucky number (31) is the minimum (31)
Process 0: my lucky number (31) is equals to the average (31.00)
Process 0: my lucky number (31) is the maximum (31)
