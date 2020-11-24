/*

-Materia: Supercómputo
-Semestre: 2019-2020/I
-Nombre del alumno: Bernal Cabrera José Bernardo    
-Clave del alumno: 248062
-Carrera: Ing. Informática
-Nombre del alumno: Méndez Gutiérrez    
-Clave del alumno: 250980
-Carrera: Ing. Informática
-Nombre de tarea o programa: MPI: Funciones Gather y Scatter
-Periodo de evaluación (parcial 1, 2, 3, 4 o 5): 2
-Descripción: Programa en MPI que muestra el funcionamiento de estas funciones
-Avance logrado (0 a 100%): 100%
-Comentarios adicionales: Ninguno
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "mpi.h"

int getNumeroRandom();
int *creaArreglo(int tam);
void llenaMatriz(int *matriz, int fil, int col);
void imprime(int *matriz, int fil, int col);
int validaArgumentos(int argc, char *argv[], int idProc, int *datos);
int filaXColumna(int *fil, int *col, int datos);
int *llenaFila(int *A, int tam, int indice);
int *llenaColumna(int *A, int tam, int indice, int col);
int *getArrayDeProcesos(int numProc, int nDatos, int nDatosU);
int *getArrayDesplazamiento(int numProc, int nDatos);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    setlocale(LC_ALL, "es_MX");
    int idProc, numProc, nDatos, nDatosU;
    int *CBuf, *procesos, *direccionamiento, *A, *B, *C;
    int datos[4]; // Indices 0: #Fil A 1: #Col A 2: #Fil B, 3: #Col B
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Status estado;
    if (!validaArgumentos(argc, argv, idProc, datos))
    {
        MPI_Finalize();
        return 0;
    }
    A = creaArreglo(datos[0] * datos[1]);
    B = creaArreglo(datos[2] * datos[3]);
    nDatos = (datos[0] * datos[3]) / (numProc);
    nDatosU = nDatos + (datos[0] * datos[3]) % (numProc);
    procesos = getArrayDeProcesos(numProc, nDatos, nDatosU);
    direccionamiento = getArrayDesplazamiento(numProc, nDatos);
    CBuf = creaArreglo(nDatos);
    if (idProc == 0)
    {
        llenaMatriz(A, datos[0], datos[1]);
        llenaMatriz(B, datos[2], datos[3]);
        imprime(A, datos[0], datos[1]);
        imprime(B, datos[2], datos[3]);
    }
    MPI_Bcast(A, datos[0] * datos[1], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, datos[2] * datos[3], MPI_INT, 0, MPI_COMM_WORLD);
    if (idProc == 0)
    {
        C = creaArreglo(datos[0] * datos[3]); //Matriz resultante
    }
    int *fil, *col, i, j, k;
    fil = creaArreglo(datos[0]);
    col = creaArreglo(datos[3]);
    i = (idProc * nDatos) / datos[3];
    j = (idProc * nDatos) % datos[3];
    if(idProc == numProc - 1){
        nDatos = nDatosU;
    }
    for (k = 0; k < nDatos; k++)
    {
        fil = llenaFila(A, datos[1], i);
        col = llenaColumna(B, datos[2], j, datos[3]);
        CBuf[k] = filaXColumna(fil, col, datos[0]);
        if (j + 1 < datos[3])
        {
            j++;
        }else{
            j = 0;
            i++;
        }
    }
    MPI_Gatherv(CBuf, nDatos, MPI_INT, C, procesos, direccionamiento, MPI_INT, 0, MPI_COMM_WORLD);
    free(A);
    free(B);
    if (idProc == 0)
    {
        imprime(C, datos[0], datos[3]);
        free(C);
    }
    MPI_Finalize();
    return 0;
}

int *getArrayDeProcesos(int numProc, int nDatos, int nDatosU)
{
    int *procesos = creaArreglo(numProc);
    int i;
    for (i = 0; i < numProc - 1; i++)
    {
        *(procesos + i) = nDatos;
    }
    *(procesos + i) = nDatosU;
    return procesos;
}

int *getArrayDesplazamiento(int numProc, int nDatos)
{
    int *desplazamiento = creaArreglo(numProc);
    int i;
    for (i = 0; i < numProc; i++)
    {
        *(desplazamiento + i) = i * nDatos;
    }
    return desplazamiento;
}

int *llenaFila(int *A, int tam, int indice)
{
    int i, *fila;
    fila = creaArreglo(tam);
    for (i = 0; i < tam; i++)
    {
        *(fila + i) = *(A + (indice * tam) + i);
    }

    return fila;
}

int *llenaColumna(int *A, int tam, int indice, int col)
{
    int i, *columna;
    columna = creaArreglo(tam);
    for (i = 0; i < tam; i++)
    {
        *(columna + i) = *(A + indice + (i * col));
    }
    return columna;
}

int validaArgumentos(int argc, char *argv[], int idProc, int *datos)
{
    if (argc < 5)
    {
        if (!idProc)
        {
            printf("Te faltan argumentos\n");
        }
        return 0;
    }
    if (!atoi(argv[1]) || atoi(argv[1]) < 0)
    {
        if (!idProc)
        {
            printf("La cantidad de filas de la matriz A no es válida\n");
        }
        return 0;
    }
    if (!atoi(argv[2]) || atoi(argv[2]) < 0)
    {
        if (!idProc)
        {
            printf("La cantidad de columnas de la matriz A no es válida\n");
        }
        return 0;
    }
    if (!atoi(argv[3]) || atoi(argv[3]) < 0)
    {
        if (!idProc)
        {
            printf("La cantidad de filas de la matriz B no es válida\n");
        }
        return 0;
    }
    if (!atoi(argv[4]) || atoi(argv[4]) < 0)
    {
        if (!idProc)
        {
            printf("La cantidad de columnas de la matriz B no es válida\n");
        }
        return 0;
    }
    if (!multiplicacionValida(atoi(argv[2]), atoi(argv[3])))
    {
        if (!idProc)
        {
            printf("La cantidad de filas de la matriz A no es igual a la cantidad de columnas de la matriz B\n");
        }
        return 0;
    }
    int i = 0;
    for (; i < 4; i++)
    {
        datos[i] = atoi(argv[i + 1]);
    }
    return 1;
}

int getNumeroRandom(int limInf, int limSup)
{

    return rand() % limSup + limInf;
}

int *creaArreglo(int tam)
{
    int *matriz = (int *)malloc(tam * sizeof(int));
    return matriz;
}

void llenaMatriz(int *matriz, int fil, int col)
{
    int i, j;
    for (i = 0; i < fil * col; i++)
    {
        int num = getNumeroRandom(1, 10);
        matriz[i] = num;
    }
}

void imprime(int *matriz, int fil, int col)
{
    int i;
    for (i = 0; i < fil * col; i++)
    {
        printf("%d ", matriz[i]);
        if ((i + 1) % col == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

int multiplicacionValida(int filA, int colB)
{
    if (filA == colB)
    {
        return 1;
    }
    return 0;
}

int filaXColumna(int *fil, int *col, int datos)
{
    int i, resultado;
    resultado = 0;
    for (i = 0; i < datos; i++)
    {
        resultado += *(fil + i) * *(col + i);
    }
    return resultado;
}
