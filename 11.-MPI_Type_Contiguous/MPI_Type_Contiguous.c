#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

typedef struct {
        int x;
        int y;
        int z;
}point;

void ImprimeArreglo(int *arreglo, int n);
void ImprimeArreglo_Puntos(point *arrP, int n);
int *reservaMemoria(int tam);
point *reservaMemoriaPuntos(int tam);

int main(int argc, char *argv[])
{
	int n, NumProc, idProc;
    point *sendBuff_Puntos;//Arreglo de puntos en el proceso 0
    point *subArreglo_Puntos;//Arreglo auxiliar que se ha de usar en todos los procesos
    int *recvBuff_Suma;//Arreglo de enteros para guardar el resultado
    int *sendSubArregloBuff_Suma;//Sub arreglo donde se ha de almacenar el resultados de la suma de los subArreglos de puntos
    int *counts, *desplazamiento;
    int i;
    int iSem = time(NULL);     
    srand(iSem);  
    n = atoi(argv[1]);//Numero de Estructuras
    
    MPI_Datatype myType;
    MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Type_contiguous(3,MPI_INT,&myType);
    MPI_Type_commit(&myType);

    int nDatos = n/NumProc;
	int nDatosU = nDatos + n%(NumProc);

    counts = reservaMemoria(NumProc);
    desplazamiento = reservaMemoria(NumProc);
    
    if(idProc == (NumProc - 1))
    {
        subArreglo_Puntos = reservaMemoriaPuntos(nDatosU);
        sendSubArregloBuff_Suma = reservaMemoria(nDatosU);
    }
    else
    {
        subArreglo_Puntos = reservaMemoriaPuntos(nDatos);
        sendSubArregloBuff_Suma = reservaMemoria(nDatos);
    }
    
    if(idProc == 0)
	{
        sendBuff_Puntos = reservaMemoriaPuntos(n);    
	    recvBuff_Suma = reservaMemoria(n);
               
        //Llenar de datos las estructuras
        for (i = 0; i < n; i++)
        {
            sendBuff_Puntos[i].x = i;
            sendBuff_Puntos[i].y = i+1;
            sendBuff_Puntos[i].z = i+2;
        }
        ImprimeArreglo_Puntos(sendBuff_Puntos, n);
	}

    for (i = 0; i < NumProc; i++)
    {
        counts[i] = nDatos;
        desplazamiento[i] = i*nDatos;
        if(i == (NumProc - 1))
        {
            counts[i] = nDatosU;
        }
    }

    if(idProc == (NumProc - 1))
    {
        MPI_Scatterv(sendBuff_Puntos, counts, desplazamiento, myType, subArreglo_Puntos, nDatosU, myType, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Scatterv(sendBuff_Puntos, counts, desplazamiento, myType, subArreglo_Puntos, nDatos, myType, 0, MPI_COMM_WORLD);
    }

    

    if(idProc == (NumProc - 1))
    {
        for (i = 0; i < nDatosU; i++)
        {
            sendSubArregloBuff_Suma[i] = subArreglo_Puntos[i].x + subArreglo_Puntos[i].y + subArreglo_Puntos[i].z;
        }
        MPI_Gatherv(sendSubArregloBuff_Suma, nDatosU, MPI_INT, recvBuff_Suma, counts, desplazamiento, MPI_INT, 0, MPI_COMM_WORLD);  
    }
    else
    {
        for (i = 0; i < nDatos; i++)
        {
            sendSubArregloBuff_Suma[i] = subArreglo_Puntos[i].x + subArreglo_Puntos[i].y + subArreglo_Puntos[i].z;
        }
        MPI_Gatherv(sendSubArregloBuff_Suma, nDatos, MPI_INT, recvBuff_Suma, counts, desplazamiento, MPI_INT, 0, MPI_COMM_WORLD);  
    }
    
    if(idProc == 0)
    {
        printf("Arreglo Resultante\n");
        ImprimeArreglo(recvBuff_Suma, n);
        free(sendBuff_Puntos);
        free(recvBuff_Suma);
    }

    free(counts);
    free(desplazamiento);
    free(subArreglo_Puntos);
    free(sendSubArregloBuff_Suma);
    
    MPI_Finalize();
   	return 0;
}

void ImprimeArreglo(int *arreglo, int n)
{    
    int i;
    for(i = 0; i < n; i++) 
    {        
        printf("    %d\n", arreglo[i]);
    }
}

void ImprimeArreglo_Puntos(point *arrP, int n)
{    
    int i;
    printf("Arreglo Original de Coordenadas\n");
    for (i = 0; i < n; i++)
    {
        printf("Coor: %d, %d, %d\n", arrP[i].x, arrP[i].y, arrP[i].z);
    }
}


int *reservaMemoria(int tam)
{
    int *array = (int *)malloc(tam * sizeof(int));
    return array;
}

point *reservaMemoriaPuntos(int tam)
{
    point *array = (point *)malloc(tam * sizeof(point));
    return array;
}