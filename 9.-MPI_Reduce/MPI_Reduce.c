#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

void llenaAleatorio(int *arreglo, int n);
void ImprimeArreglo(int *arreglo, int n);
void ImprimeArreglos(int *arrA, int *arrB, int n);
int *reservaMemoria(int tam);

int main(int argc, char *argv[])
{
	int n, NumProc, idProc;
    int *arr, *irbufSUM, *irbufPROD;
    int i;
    int iSem = time(NULL);     
    srand(iSem);

    n = atoi(argv[1]);//Numero de datos a procesar
    MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);

    arr = reservaMemoria(n);
    llenaAleatorio(arr, n);
    if(idProc == 0)
    {
        irbufSUM = reservaMemoria(n);
        irbufPROD = reservaMemoria(n);
    }

    MPI_Reduce(arr, irbufSUM, n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(arr, irbufPROD, n, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);


    ImprimeArreglo(arr, n);
    if(idProc == 0)
    {
        printf("MPI_SUM\n");
        ImprimeArreglo(irbufSUM, n);
        printf("MPI_PROD\n");
        ImprimeArreglo(irbufPROD, n);
        free(irbufSUM);
        free(irbufPROD);
    }


    free(arr);
    MPI_Finalize();
   	return 0;
}
void llenaAleatorio(int *arreglo, int n)
{
    // Declaracion de variables  
    int sum = 0;
    // Semilla de rand();  
    int i;
    for(i = 0; i < n; i++)  
    {        
        arreglo[i] = rand()%100;
    }
}

void ImprimeArreglo(int *arreglo, int n)
{    
    int i;
    for(i = 0; i < n; i++) 
    {        
        printf("%d ", arreglo[i]);
    }
    printf("\n");
}

int *reservaMemoria(int tam)
{
    int *array = (int *)malloc(tam * sizeof(int));
    return array;
}