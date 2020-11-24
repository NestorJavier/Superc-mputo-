#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "mpi.h"

int getNumeroRandom();
int *generaMatriz(int tam);
void llenaMatriz(int *matriz, int fil, int col);
void imprime(int *matriz, int fil, int col);
int validaArgumentos(int argc, char *argv[], int idProc);
int filaXColumna(int *fil, int *col, int datos);
int *llenaFila(int *A, int tam, int indice);
int *llenaColumna(int *A, int tam, int indice, int col);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    setlocale(LC_ALL, "es_MX");
    int idProc, numProc, nDatos, nDatosU, filA, colA, filB, colB;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Status estado;
    if (!validaArgumentos(argc, argv, idProc))
    {
        MPI_Finalize();
        return 0;
    }
    filA = atoi(argv[1]);
    colA = atoi(argv[2]);
    filB = atoi(argv[3]);
    colB = atoi(argv[4]);
    int *A = generaMatriz(filA * colA);
    int *B = generaMatriz(filB * colB);
    nDatos = (filA * colB) / (numProc - 1);
    nDatosU = nDatos + (filA * colB) % (numProc - 1);
    if (idProc == 0)
    {
        llenaMatriz(A, filA, colA);
        llenaMatriz(B, filB, colB);
        imprime(A, filA, colA);
        imprime(B, filB, colB);
    }
    MPI_Bcast(A, filA * colA, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, filB * colB, MPI_INT, 0, MPI_COMM_WORLD);
    if (idProc == 0)
    {
        int i, j, *parcial, cont;
        cont = 0;
        int *C = generaMatriz(filA * colB); //Matriz resultante
        for (i = 1; i < numProc - 1; i++)
        {
            parcial = generaMatriz(nDatos);
            MPI_Recv(parcial, nDatos, MPI_INT, i, 0, MPI_COMM_WORLD, &estado);
            for (j = 0; j < nDatos; j++)
            {
                *(C + cont) = *(parcial + j);
                cont++;
            }
        }
        parcial = generaMatriz(nDatosU);
        MPI_Recv(parcial, nDatosU, MPI_INT, i, 0, MPI_COMM_WORLD, &estado);
        for (j = 0; j < nDatosU; j++)
        {
            *(C + cont) = *(parcial + j);
            cont++;
        }
        imprime(C, filA, colB);
        free(C);
    }
    else
    {
        int *fil, *col, *resultado, i, j, cont, n;
        fil = generaMatriz(filA);
        col = generaMatriz(colB);
        cont = n = 0;
        if (idProc != numProc - 1)
        {
            resultado = generaMatriz(nDatos);
        }
        else
        {
            resultado = generaMatriz(nDatosU);
        }
        for (i = 0; i < filA; i++)
        {
            for (j = 0; j < colB; j++)
            {
                if (cont >= nDatos * (idProc - 1))
                {
                    if (n < nDatos)
                    {
                        fil = llenaFila(A, colA, i);
                        col = llenaColumna(B, filB, j, colB);
                        resultado[n] = filaXColumna(fil, col, filA);
                        n++;
                    }
                    else{
                        i = filA;
                        break;
                    }
                }
                cont++;
            }
        }
        if (idProc == numProc - 1)
        {
            nDatos = nDatosU;
        }
        MPI_Send(resultado, nDatos, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    free(A);
    free(B);
    MPI_Finalize();
    return 0;
}

int *llenaFila(int *A, int tam, int indice)
{
    int i, *fila;
    fila = generaMatriz(tam);
    for (i = 0; i < tam; i++)
    {
        *(fila + i) = *(A + (indice * tam) + i);
    }

    return fila;
}

int *llenaColumna(int *A, int tam, int indice, int col)
{
    int i, *columna;
    columna = generaMatriz(tam);
    for (i = 0; i < tam; i++)
    {
        *(columna + i) = *(A + indice + (i * col));
    }
    return columna;
}

int validaArgumentos(int argc, char *argv[], int idProc)
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
    return 1;
}

int getNumeroRandom(int limInf, int limSup)
{

    return rand() % limSup + limInf;
}

int *generaMatriz(int tam)
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
