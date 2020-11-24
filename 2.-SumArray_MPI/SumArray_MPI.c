#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

void llenaAleatorio(int arreglo[], int n);

int main(int argc, char *argv[])
{
	int n = 110, NumProc, idProc;
    
    MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);

    	
	int nDatos = n/(NumProc - 1);
	int nDatosU = nDatos + n%(NumProc - 1);
	
    	
	if(idProc == 0)
	{
		printf("nDatos: %d\n", nDatos);
		printf("nDatosU: %d\n", nDatosU);
		int arreglo[n];
        int subArreglo[nDatos], subArregloU[nDatosU];
        int iSum = 0, iSumTotal = 0;

		llenaAleatorio(arreglo, n);
        int index, i;
        int lim = NumProc-2;
        for(i = 1; i < (NumProc - 1); i++)
        {
            index = (i-1)*nDatos;
            MPI_Send(&arreglo[index], nDatos, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        index = (i-1)*nDatos;

        MPI_Send(&arreglo[index], nDatosU, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Status status;
        for(i = 1; i < NumProc; i++)
        {
            MPI_Recv(&iSum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            iSumTotal += iSum;
        }
        printf("              ------------\n");
        printf("La suma total es:  %d\n", iSumTotal);
	}
    else
    {
	//MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
        if(idProc == (NumProc-1))
            nDatos = nDatosU;
            
        int arreglo[nDatos], iSum = 0;
        MPI_Status status;
        MPI_Recv(&arreglo, nDatos, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int i;
        for(i = 0; i < nDatos; i++)
        {
            iSum += arreglo[i];
        }
	    printf("La suma Parcial es: %d\n", iSum);
        MPI_Send(&iSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
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
        printf("             %d    \n", arreglo[i]);
        sum +=  arreglo[i];
    }
        printf("Suma total elemento a elemento en el proceso 0: %d\n\n", sum);
}
