#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

void llenaAleatorio(int arreglo[], int n);
void ImprimeArreglo(int arreglo[], int n);

int main(int argc, char *argv[])
{
	int n = 10, NumProc, idProc;
    int arr1[n], arr2[n], irbuf[n];
     
    MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
	int nDatos = n/NumProc;
    int auxA[nDatos], auxB[nDatos], auxC[nDatos];

	if((n%NumProc) != 0)
    {
        MPI_Finalize();
   	    return 0;
    }

	if(idProc == 0)
	{
		llenaAleatorio(arr1, n);
        llenaAleatorio(arr2, n);

        printf("Arreglo A\n");
        ImprimeArreglo(arr1, n);



        printf("Arreglo B\n");
        ImprimeArreglo(arr2, n);
	}

    MPI_Scatter(&arr1, nDatos, MPI_INT, &auxA, nDatos, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&arr2, nDatos, MPI_INT, &auxB, nDatos, MPI_INT, 0, MPI_COMM_WORLD);
    
    int i;
    for (i = 0; i < nDatos; i++)
    {
        auxC[i] = auxA[i] + auxB[i];
    }
    
    MPI_Gather(&auxC, nDatos, MPI_INT, &irbuf, nDatos, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(idProc == 0)
    {
        printf("Arreglo C\n");
        ImprimeArreglo(irbuf, n);
    }
    MPI_Finalize();
   	return 0;
}

void llenaAleatorio(int arreglo[], int n)
{
    // Declaracion de variables  
    int iSem = time(NULL);
    int sum = 0;
    // Semilla de rand();  
    srand(iSem);  
    int i;
    for(i = 0; i < n; i++)  
    {        
        arreglo[i] = rand()%100;
    }
}


void ImprimeArreglo(int arreglo[], int n)
{    
    int i;
    for(i = 0; i < n; i++) 
    {        
        printf("Elemnto %d: %d\n", i, arreglo[i]);
    }
}
