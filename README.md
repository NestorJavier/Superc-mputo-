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
[Hola Mundo](./1.-HolaMundo) Usando MPI.

## 2. Suma el contenido de un arreglo estatico de enteros
[Suma el contenido de un arreglo](./2.-SumArray_MPI) distribuyendo el contenido del arreglo en los distintos procesos
para que cada uno realice una suma parcial del contenido.

## 3. Suma el contenido de un arreglo dinamico de enteros
[Suma Arreglo Dinamico](./3.-SumaArregloDinamico) Suma de un arreglo dinamico de enteros, distribuyendo el contenido del arreglo en los distintos procesos para que cada uno realice una suma parcial del contenido.

## 4. Imprime el contenido de un arreglo usando la función MPI_Bcast
[Imprime el contenido de un arreglo](./4.-PrintArray_MPI_Bcast) distribuyendo el contenido del arreglo en los distintos procesos
para que cada uno imprima parte del contenido.

## 5. Multiplicación de Matrices
[Multiplica dos matrices](./5.-Mult_Matri_MPI) Distribuye la operación en distintos procesos para realizarla en paralelo.

## 6. Suma de dos arreglos
[Suma de dos arreglos](./6.-Sum2Arrays_MPI) Distribuye la operación en distintos procesos para realizarla en paralelo,
cada proceso suma partes iguales de ambos arreglos.

## 7.- Suma de dos arreglos (Gather-Scatter)
[Suma de dos arreglos usando las funciones (Gather-Scatter) de MPI](./7.-Sum2Arrays(Gather-Scatter)V_MPI)

## 8. Multiplicación de Matrices
[Multiplica dos matrices usando las funciones (Gather-Scatter) de MPI](./8.-Mult_Matri_(Gather-Scatter)V_MPI)

## 9. Ejemplo de MPI_Reduce
[MPI Reduce operaciones MPI_SUM y MPI_PROD](./9.-MPI_Reduce)

## 10. Criba Eratostenes
[Criba de eratostenes:](./10.-Criba_Eratostenes) Implementación del algoritmo de la Criba de Eratóstenes
que consiste en obtener todos los numeros primos anteriores a un numero dado.

## 11. MPI_Type_Contiguous
[11.-MPI_Type_Contiguous](./11.-MPI_Type_Contiguous) ejercicio

## 12. MPI_Type_Struct
[MPI_Type_Struct](./12.-MPI_Type_Struct) ejercicio

## 13. Cuda
[Cuda ejercicios](./13.-Cuda)