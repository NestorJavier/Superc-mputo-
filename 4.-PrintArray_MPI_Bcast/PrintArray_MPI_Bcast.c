#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

void llenaArreglo(int *arreglo, int n);
void imprimeArreglo(int *arreglo, int n);

int main(int argc, char *argv[])
{
	int n = 10;//elementos en del arreglo
    int NumProc, idProc;
    MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    int *arreglo;
    arreglo = (int*)malloc(n*sizeof(int));

	if(idProc == 0)
	{
        llenaArreglo(arreglo, n);
    }

    MPI_Bcast(arreglo, n, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Desde el proceso %d de %d\n",idProc,NumProc);
    imprimeArreglo(arreglo, n);
    
    free(arreglo);
    MPI_Finalize();
   	return 0;
}

void llenaArreglo(int *arreglo, int n)
{
    int iSem = time(NULL);  
    srand(iSem); 
    int i;
    for(i = 0; i < n; i++)  
    {        
        arreglo[i] = rand()%100;
    }
}

void imprimeArreglo(int *arreglo, int n)
{
    int i;
    for(i = 0; i < n; i++)  
    {        
        printf("%d\n",arreglo[i]);
    }
}
