#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

void llenaAleatorio(int arreglo[], int n);
void llenaSubArreglo(int arreglo[],  int indiceArreglo, int nDatos, int subArreglo[]);

int main(int argc, char *argv[])
{
	int n, NumProc, idProc;
    MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);

    printf("Introduce el tamanio del arreglo: ");
    scanf("%d", &n);
	int nDatos = n/(NumProc - 1);
	int nDatosU = nDatos + n%(NumProc - 1);
    int suma = 0;
    	
	if(idProc == 0)
	{
		int arreglo[n];
        int subArreglo[nDatos], subArregloU[nDatosU];
        int iSum = 0, iSumTotal;

		llenaAleatorio(arreglo, n);
        int i;
        for(i = 1; i < (NumProc - 2); i++)
        {
            llenaSubArreglo(arreglo, nDatos*i, nDatos, subArreglo);
            MPI_Send(subArreglo, nDatos, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        llenaSubArreglo(arreglo, nDatos*(NumProc-1), nDatosU, subArregloU);
        MPI_Send(subArregloU, nDatosU, MPI_INT, NumProc-1, 0, MPI_COMM_WORLD);
        MPI_Status status;
        for(i = 1; i < (NumProc - 1); i++)
        {
            MPI_Recv(&iSum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            iSumTotal += iSum;
        }
        printf("La suma es: %d\n", iSumTotal);
	}
    else
    {
        if(idProc == (NumProc-1))
            nDatos = nDatosU;
            
        int arreglo[nDatos], iSum;
        MPI_Status status;
        MPI_Recv(arreglo, nDatos, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int i;
        for(i = 0; i < nDatos; i++)
        {
            iSum += arreglo[i];
        }
        MPI_Send(&iSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    	
   	return 0;
}

void llenaAleatorio(int arreglo[], int n)
{
    // Declaracion de variables  
    int iSem = time(NULL);  

    // Semilla de rand();  
    srand(iSem);  
    int i;
    for(i = 0; i < n; i++)  
    {        
        arreglo[i] = rand()%100;
        printf("             %d    \n", arreglo[i]); 
    }
        printf("         ----------\n");
}

//[0][1][2][3][4][5][6][7][8][9][10][11][12][13][14][15][16][17][18][19][20]
void llenaSubArreglo(int arreglo[],  int indiceArreglo, int nDatos, int subArreglo[])
{
    int j;
    for(j = 0; j < nDatos; j++)
    {
        subArreglo[j] = arreglo[indiceArreglo+j];
    }
}
