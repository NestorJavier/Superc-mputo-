/*

-Materia: SuperComputo

-Semestre: 2019-2020/I

-Nombre del alumno: Méndez Gutiérrez Néstor Javier

-Clave del alumno: 250980

-Carrera: Informatica

-Nombre de tarea o programa: MPI: Funciones GatherV y ScatterV

-Periodo de evaluación: Parcial 2

-Descripción: Sumar elemento a elemento dos arreglos de igual tamaño y almacenar el resultado en un tercer arreglo del mismo tamaño

-Avance logrado (0 a 100%): 100%

-Comentarios adicionales: 
                            Para correr el programa se agrega un argumento al final del comando el cual corresponde al numero de datos que 
                            se van a procesar, es decir el tamaño de los 3 arreglos que se han de generar
                            Ejemplo"mpi run -np 10 exe 27"
*/

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
    int *arr1, *arr2, *irbuf;
    int *auxA, *auxB, *auxC;
    int *counts, *desplazamiento;
    int i;
    int iSem = time(NULL);     
    srand(iSem);  
    
    n = atoi(argv[1]);//Numero de datos a procesar
    MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    int nDatos = n/NumProc;
	int nDatosU = nDatos + n%(NumProc);

    counts = reservaMemoria(NumProc);
    desplazamiento = reservaMemoria(NumProc);

    if(idProc == (NumProc - 1))
    {
        auxA = reservaMemoria(nDatosU);
        auxB = reservaMemoria(nDatosU);
        auxC = reservaMemoria(nDatosU);
    }
    else
    {
        auxB = reservaMemoria(nDatos);
        auxC = reservaMemoria(nDatos);
        auxA = reservaMemoria(nDatos);
    }

	if(idProc == 0)
	{
        printf("nDatos: %d,  nDatosU: %d\n", nDatos, nDatosU);
        arr1 = reservaMemoria(n);
        arr2 = reservaMemoria(n);
        irbuf = reservaMemoria(n);
		
        llenaAleatorio(arr1, n);
        llenaAleatorio(arr2, n);
        
        ImprimeArreglos(arr1, arr2, n);

        for (i = 0; i < NumProc; i++)
        {
            counts[i] = nDatos;
            desplazamiento[i] = i*nDatos;
            if(i == (NumProc - 1))
            {
                counts[i] = nDatosU;
            }
        }
	}

    if(idProc == (NumProc - 1))
    {
        MPI_Scatterv(arr1, counts, desplazamiento, MPI_INT, auxA, nDatosU, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatterv(arr2, counts, desplazamiento, MPI_INT, auxB, nDatosU, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Scatterv(arr1, counts, desplazamiento, MPI_INT, auxA, nDatos, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatterv(arr2, counts, desplazamiento, MPI_INT, auxB, nDatos, MPI_INT, 0, MPI_COMM_WORLD);
    }

    

    if(idProc == (NumProc - 1))
    {
        for (i = 0; i < nDatosU; i++)
        {
            auxC[i] = auxA[i] + auxB[i];
        }
        MPI_Gatherv(auxC, nDatosU, MPI_INT, irbuf, counts, desplazamiento, MPI_INT, 0, MPI_COMM_WORLD);  
    }
    else
    {
        for (i = 0; i < nDatos; i++)
        {
            auxC[i] = auxA[i] + auxB[i];
        }
        MPI_Gatherv(auxC, nDatos, MPI_INT, irbuf, counts, desplazamiento, MPI_INT, 0, MPI_COMM_WORLD);  
    }
    
    if(idProc == 0)
    {
        printf("Arreglo C\n");
        ImprimeArreglo(irbuf, n);
    
        free(arr1);
        free(arr2);
        free(irbuf);
    }
    free(auxA);
    free(auxB);
    free(auxC);
    free(counts);
    free(desplazamiento);

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
        printf("Elemnto %d: %d\n", i, arreglo[i]);
    }
}

void ImprimeArreglos(int *arrA, int *arrB, int n)
{    
    int i;
        printf("            A + B = C\n");
    for(i = 0; i < n; i++) 
    {        
        printf("Elemnto %d: %d + %d = %d\n", i, arrA[i], arrB[i], arrA[i] + arrB[i]);
    }
}


int *reservaMemoria(int tam)
{
    int *array = (int *)malloc(tam * sizeof(int));
    return array;
}