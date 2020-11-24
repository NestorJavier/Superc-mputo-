#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

typedef struct {
        int a;
        float b;
        char str[100];
}EST;

void imprimeCadena(char arr[], int tam);
void Concatena(char arrA[], char arrB[], char arrC[]);
void llenaCadena(char arr[]);
int RandomNumber(int inicio, int fin);
void ImprimeArreglo(int *arreglo, int n);
int *reservaMemoria(int tam);
EST *reservaMemoriaEST(int tam);

int main(int argc, char *argv[])
{
	int n, NumProc, idProc;
    EST *sendBuff_A, *sendBuff_B, *recvBuff_C;
    EST *subArray_A, *subArray_B, *subArray_C;
    int *counts, *desplazamiento;
    int i;
    float r = 13.7, r2 = 34.3, r3 = 57.2;

    int iSem = time(NULL);     
    srand(iSem);  
    n = atoi(argv[1]);//Numero de Estructuras
    ////////////////////////////
    int num_Elem_Bloque[3];
    num_Elem_Bloque[0] = 1;
    num_Elem_Bloque[1] = 1;
    num_Elem_Bloque[2] = 100;
    
    ////////////////////////////
    MPI_Aint desplazamientoTipo[3];
    desplazamientoTipo[0] = 0;
    desplazamientoTipo[1] = sizeof(int);
    desplazamientoTipo[2] = sizeof(int) + sizeof(float);

    //////////////////////////
    MPI_Datatype tipos[3];
    tipos[0] = MPI_INT;
    tipos[1] = MPI_FLOAT;
    tipos[2] = MPI_CHAR;
    
    MPI_Datatype myType;

    MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Type_struct(3, num_Elem_Bloque, desplazamientoTipo, tipos, &myType);
    MPI_Type_commit(&myType);

    int nDatos = n/NumProc;
	int nDatosU = nDatos + n%(NumProc);

    counts = reservaMemoria(NumProc);
    desplazamiento = reservaMemoria(NumProc);
    
    if(idProc == (NumProc -1))
    {
        subArray_A = reservaMemoriaEST(nDatosU);
        subArray_B = reservaMemoriaEST(nDatosU);
        subArray_C = reservaMemoriaEST(nDatosU);
    }
    else
    {
        subArray_A = reservaMemoriaEST(nDatos);
        subArray_B = reservaMemoriaEST(nDatos);
        subArray_C = reservaMemoriaEST(nDatos);
    }
    
    if(idProc == 0)
	{
        sendBuff_A = reservaMemoriaEST(n);
        sendBuff_B = reservaMemoriaEST(n);
        recvBuff_C = reservaMemoriaEST(n);
               
        //Llenar de datos las estructuras
        for (i = 0; i < n; i++)
        {
            sendBuff_A[i].a = i;
            sendBuff_B[i].a = i+3;
            sendBuff_A[i].b = (r2/r)+i;
            sendBuff_B[i].b = (r3/r)+i;
            llenaCadena(sendBuff_A[i].str);
            llenaCadena(sendBuff_B[i].str);
        }
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
        MPI_Scatterv(sendBuff_A, counts, desplazamiento, myType, subArray_A, nDatosU, myType, 0, MPI_COMM_WORLD);
        MPI_Scatterv(sendBuff_B, counts, desplazamiento, myType, subArray_B, nDatosU, myType, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Scatterv(sendBuff_A, counts, desplazamiento, myType, subArray_A, nDatos, myType, 0, MPI_COMM_WORLD);
        MPI_Scatterv(sendBuff_B, counts, desplazamiento, myType, subArray_B, nDatos, myType, 0, MPI_COMM_WORLD);
    }

    

    if(idProc == (NumProc - 1))
    {
        for (i = 0; i < nDatosU; i++)
        {
            subArray_C[i].a = subArray_A[i].a + subArray_B[i].a;
            subArray_C[i].b = subArray_A[i].b + subArray_B[i].b;
            Concatena(subArray_A->str, subArray_B->str, subArray_C->str);
        }
        MPI_Gatherv(subArray_C, nDatosU, myType, recvBuff_C, counts, desplazamiento, myType, 0, MPI_COMM_WORLD);  
    }
    else
    {
        for (i = 0; i < nDatos; i++)
        {
            subArray_C[i].a = subArray_A[i].a + subArray_B[i].a;
            subArray_C[i].b = subArray_A[i].b + subArray_B[i].b;
            Concatena(subArray_A->str, subArray_B->str, subArray_C->str);
        }
        MPI_Gatherv(subArray_C, nDatos, myType, recvBuff_C, counts, desplazamiento, myType, 0, MPI_COMM_WORLD);  
    }
    
    if(idProc == 0)
    {
        printf("Arreglo Resultante\n");

        for (i = 0; i < n; i++)
        {
            printf("suma de %d + %d = %d\n",sendBuff_A[i].a, sendBuff_B[i].a, recvBuff_C[i].a);
            printf("suma de %f + %f = %f\n",sendBuff_A[i].b, sendBuff_B[i].b, recvBuff_C[i].b);
            /*printf("Cad A\n");
            imprimeCadena(sendBuff_A[i].str, 50);
            printf("Cad B\n");
            imprimeCadena(sendBuff_B[i].str, 50);
            printf("Cad C\n");
            imprimeCadena(recvBuff_C[i].str, 100);*/
        }
        
        free(sendBuff_A);
        free(sendBuff_B);
        free(recvBuff_C);
    }

    free(counts);
    free(desplazamiento);
    free(subArray_A);
    free(subArray_B);
    free(subArray_C);    
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



int *reservaMemoria(int tam)
{
    int *array = (int *)malloc(tam * sizeof(int));
    return array;
}

EST *reservaMemoriaEST(int tam)
{
    EST *array = (EST *)malloc(tam * sizeof(EST));
    return array;
}

void llenaCadena(char arr[])
{
    int i;
    for (i = 0; i < 50; i++)
    {
        arr[i] = (char)RandomNumber(65, 122);
    }
}

void imprimeCadena(char arr[], int tam)
{
    int i;
    for (i = 0; i < tam; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


void Concatena(char arrA[], char arrB[], char arrC[])
{
    int i;
    for (i = 0; i < 50; i++)
    {
        arrC[i] = arrA[i];
        arrC[i+50] = arrB[i];
    }
}


int RandomNumber(int inicio, int fin)
{
   int resultado = 0;
   int margen = fin - inicio;
   int comienzo = inicio;
   if( inicio > fin )
   {
       comienzo = fin;
       margen = margen * -1;
   }
   margen++;
   resultado = comienzo + (rand() % margen);
   return resultado;
}