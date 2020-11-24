#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

void RellenaArregloPrimos(int *arregloParcialPrimos,int *arrAux,int lim);
int CuentaPrimos(int *arrAux,int lim);
void RellenaArreglo(int *arrAux, int lim, int nDatos, int idProc);
void CalculaPrimos(int *arrAux, int desde, int hasta, int nDatos, int idProc, int n);
void ImprimeArreglo(int *arreglo, int n);
int *reservaMemoria(int tam);

int main(int argc, char *argv[])
{
	int n, NumProc, idProc, nDatos, nDatosU, iAux = 0, numeroTotalPrimos = 0;
    int *arregloParcialPrimos, *arrAux, *counts, *desplazamiento, *buffer, lim, desde, hasta, i, j, CantidadPrimos = 0;
    
    n = atoi(argv[1]);//Limite superior hasta el cual se han de calcular los numeros primos

    MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    nDatos = n/NumProc;
	nDatosU = nDatos + n%(NumProc);

    //Inicializamos o reservamos memoria para todas las variables necesarias para el calculo, todo esto en función del proceso que este ejecutando este codigo
    if(idProc == NumProc-1)
    {
        arrAux = reservaMemoria(nDatosU);   //Reservamos memoria para almacenar el bloque de numeros en el que ha de trabajar 
                                            //el ultimo proceso 
        lim = nDatosU;
        hasta = (idProc*nDatos)+1+nDatosU;  //la variable "hasta" contiene el numero superior hasta el cual se ha de trabajar en cada proceso
    }
    else
    {
        lim = nDatos;
        arrAux = reservaMemoria(nDatos);    //Reservamos memoria para almacenar el bloque de numeros en el que ha de trabajar 
                                            //el cualquier proceso que no sea el ultimo 
        hasta = (idProc*nDatos)+1+nDatos;   //la variable "hasta" contiene el numero superior hasta el cual se ha de trabajar en cada proceso
    }
    
    if(idProc == 0)
    {
        counts = reservaMemoria(NumProc);           //Reservamos memoria en el proceso cero para los arreglos cunts y desplazamiento
        desplazamiento = reservaMemoria(NumProc);
        desde = 2;                                  //la variable "desde" contiene el numero inferior desde el cual se ha de trabajar
                                                    //en cada proceso, en el caso del prceso cero se comienza a trabajar desde el numero 2
    }
    else
        desde = (idProc*nDatos)+1;                  //la variable "desde" contiene el numero inferior desde el cual se ha de trabajar
                                                    //en cada proceso

    RellenaArreglo(arrAux, lim, nDatos, idProc);
    CalculaPrimos(arrAux, desde, hasta, nDatos, idProc, n);

    CantidadPrimos = CuentaPrimos(arrAux, lim);
    arregloParcialPrimos = reservaMemoria(CantidadPrimos);
    RellenaArregloPrimos(arregloParcialPrimos, arrAux, lim);

    //Con MPI_Gather almacenamos en un arreglo en el proceso 0 cual fue la cantidad de numeros primos calculados por cada uno de los procesos
    MPI_Gather(&CantidadPrimos, 1, MPI_INT, counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(idProc == 0)
    {
        for(i = 0; i < NumProc; i++)
        {
            desplazamiento[i] = numeroTotalPrimos;  //Ya que sabemos cuantos numeros caclculo cada uno de los procesos
                                                    //llenamos el arreglo que contendra los desplazamientos generados para cada sub arreglo de numeros primos
            numeroTotalPrimos += counts[i]; //Calculamos el numero total de primos para poder reservar memoria para el arreglo
                                            //(buffer) que ha de contener a el total de numeros primos calculados por cada proceso
        }
        buffer = reservaMemoria(numeroTotalPrimos);//Reservamos memoria para el buffer
    }

    //Cada proceso envia su parte del total de primos para almacenar el el buffer que se encuentra en el proceso 0
    MPI_Gatherv(arregloParcialPrimos, CantidadPrimos, MPI_INT, buffer, counts, desplazamiento, MPI_INT, 0, MPI_COMM_WORLD);  
    
    if(idProc == 0)
    {
        printf("El total de numeros primos hasta %d es: %d\n", n, numeroTotalPrimos);
        ImprimeArreglo(buffer, numeroTotalPrimos); //imprimimos desde el proceso cero el arreglo con todos los numeros primos calculados
        free(counts);                               //Liberamos la memoria usada en todos los procesos
        free(desplazamiento);
        free(buffer);
    }
    free(arrAux);
    free(arregloParcialPrimos);

    MPI_Finalize();
   	return 0;
}

void ImprimeArreglo(int *arreglo, int n)
{    
    int i, j = 0;
    for(i = 0; i < n; i++) 
    {        
        printf("%d ", arreglo[i]);
        j++;
        if(j == 12)
        {
            j = 0;
            printf("\n");
        }
    }
    printf("\n");
}

int *reservaMemoria(int tam)
{
    int *array = (int *)malloc(tam * sizeof(int));
    return array;
}

void CalculaPrimos(int *arrAux, int desde, int hasta, int nDatos, int idProc, int n)
{    
    int despl = 1, i, j;
    for(i = 2; i*i <= n; i++)
    {
        for (j = desde; j < hasta; j+=despl)
        {
            if(j%i == 0 && j == i)  //en el caso de que esta condición sea cierta significa que i y j tienen el mismo valor
            {                       //entonces la variable despl se iguala al numero sospechoso de ser un primo, de esta forma
                despl = i;          //el desplazamiento dentro del arreglo nos permitira trabajar solo con los multiplos de ese numero
            }
            else if(j%i == 0 && j != i) //En el caso en el cual el residuo es cero y j e i son diferentes sognifica que es un multiplo del
            {                           //sospechoso y por tanto no es un numero primo, por lo que proseguimos a marcarlo en cero, lo cual 
                arrAux[j-((nDatos*idProc)+1)] = 0;  //dentro del arreglo indica que no es un numero primo.
            }
        }
        despl = 1;
    }
}

void RellenaArreglo(int *arrAux, int lim, int nDatos, int idProc)
{
    int i;
    for (i = 0; i < lim; i++)                       //Se rellena el arreglo con todos los numeros sobre los cuales va a trabajar
    {                                               //Por ejemplo si son 4 procesos y el limite superior dado es de 100
        if(i == 0 && idProc == 0)                   //entonces a cada proceso le toca trabajar con 25 datos, que en el caso de el proceso 0
            arrAux[i] = 0;                          //serian un rango de 1 a 25, en el proceso 1 de 26 a 50 etc...
        else
        {
            arrAux[i] = (nDatos*idProc) + i + 1;
        }
    }
}

int CuentaPrimos(int *arrAux,int lim)
{
    int cant = 0, i;            //Cuenta la cantidad de numeros primos obtenidos pafra reservar memoria para el arreglo que ha de
                                //guardar unicamente los numeros primos obtenidos
    for (i = 0; i < lim; i++)
    {
        if (arrAux[i] != 0)
        {
            cant++;
        }
    }
    return cant;
}

void RellenaArregloPrimos(int *arregloParcialPrimos,int *arrAux,int lim)
{
    int i, j = 0;
    for (i = 0; i < lim; i++)       //Rellenamos el arreglo solo con los numero que resultaron ser numeros primos
    {
        if (arrAux[i] != 0)
        {
            arregloParcialPrimos[j] = arrAux[i];
            j++;
        }
    }
}