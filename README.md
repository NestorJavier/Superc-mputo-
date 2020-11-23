# Supercómputo
Scripts desarrollados para la materia de Supercómputo, todos ellos 
pensados para ejecutarse sobre una maquina o maquina virtual con un 
sistema operativo Rocks Cluster Distribution https://www.rocksclusters.org/ 
usando MPI ("Message Passing Interface", Interfaz de Paso de Mensajes).
### MPI
- Es un estándar que define la sintaxis y la semántica de las funciones contenidas en una biblioteca
- Diseñada para ser usada en programas que exploten la existencia de múltiples procesadores.
- Con MPI el número de procesos requeridos se asigna antes de la ejecución del programa, y no se crean procesos adicionales mientras la aplicación se ejecuta.
- A cada proceso se le asigna una variable que se denomina rank, la cual identifica a cada proceso, en el rango de 0 a p-1, donde p es el número total de procesos. 
- El control de la ejecución del programa se realiza mediante la variable rank; la variable rank permite determinar que proceso ejecuta determinada porción de código.
- En MPI se define un comunicador como una colección de procesos,los cuales pueden enviar mensajes el uno al otro; el comunicadorbásico se denomina MPI_COMM_WORLD

## 1. Hola Mundo
[Hola Mundo](./HolaMundo) Usando MPI.
