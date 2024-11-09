# Enunciado

Adapte su solución al ejercicio anterior para que sea más realista. En el primer argumento de línea de comandos, el usuario indicará el marcador de victoria de la partida. Por ejemplo, si se indica 10, el primer jugador que anote 10 goles ganará la partida y el juego termina.

Los jugadores en esta versión no son perfectos. El segundo y tercer argumento de línea de comandos indica la probabilidad de acierto en cada servicio (tiro o pase) del primer y segundo jugador respectivamente. Por ejemplo, la siguiente ejecución indica que el jugador 1 (proceso 0) tiene una tasa de acierto del 85.5% de los tiros y el jugador 2 de un 88%.

mpiexec -n 2 ping_pong 3 85.5 88
1: 24 1
2: 53 0
3: 11 0
0 wins 2 to 1
En la salida estándar indique una línea por cada ronda y una línea final con el resultado. En cada ronda se indica la cantidad de servicios (pases) jugados y el proceso que la ganó. La última línea indica el marcador final o si hubo un empate.

## Como correr

Use el comando make para compilar.

Luego escriba en la terminal, debe asegurarse de estar en la ruta de esta carpeta

mpiexec -n 2 ./bin/ping_pong_realistic 8 88 88
