#include <stdio.h>
#include <cuda_runtime.h>
#include <time.h>

void llenaAleatorio(float arreglo[], int n);
void ImprimeArreglo(float arreglo[], float arreglo2[], float arreglo3[], int n);

__global__ void VecAdd(float* A, float* B, float* C, int N){
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if(i < N)
        C[i] = A[i] + B[i];
}

int main(void)
{
    int N = 1024;
    size_t size = N * sizeof(float);
    float * h_A = (float*)malloc(size);
    float * h_B = (float*)malloc(size);
    float * h_C = (float*)malloc(size);
    
    //Inicializar vectores de entrada
    llenaAleatorio(h_A, N);
    llenaAleatorio(h_B, N);

    //Allocate vectors in device memory
    float* d_A;
    cudaMalloc(&d_A, size);
    float* d_B;
    cudaMalloc(&d_B, size);
    float* d_C;
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock -1)/threadsPerBlock;
    VecAdd<<<blocksPerGrid,threadsPerBlock>>>(d_A, d_B, d_C, N);

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    ImprimeArreglo(h_A, h_B,h_C, N);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    free(h_A);
    free(h_B);
    free(h_C);
}

void llenaAleatorio(float arreglo[], int n)
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


void ImprimeArreglo(float arreglo[], float arreglo2[], float arreglo3[], int n)
{    
    int i;
    for(i = 0; i < n; i++) 
    {        
        printf("%f + %f = %f\n", arreglo[i], arreglo2[i], arreglo3[i]);
    }
}
