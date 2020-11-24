#include <stdio.h>
#include <cuda_runtime.h>

__global__ void matMul(float* A, float* B, float* C, int nFil, int nCol)
{
	int idx = blockDim.x * blockIdx.x + threadIdx.x;
	int idy = blockDim.y * blockIdx.y + threadIdx.y;
	int i = idx + idy * nCol;
	if(idx < nCol && idy < nFil)
	{
		float sum = 0;
		for(int k = 0; k < nCol; k++)
		{
			sum += A[idy * nCol + k] * B[k * nCol + idx];
		}
	C[i] = sum;
	}
}

int main()
{
	int nFil = 5;
	int nCol = 5;
	int N = nFil * nCol;
	size_t size = N * sizeof(float);
	
	float* h_A = (float*)malloc(size);
	float* h_B = (float*)malloc(size);
	float* h_C = (float*)malloc(size);

	//Initialialize input vectors

	for(int i = 0; i < nFil; i++)
	{
		for(int j = 0; j < nCol; j++)
		{
			h_A[i * nCol + j] = 1;
			h_B[i * nCol + j] = 2;
		}
	}

	
	float* d_A;
	cudaMalloc(&d_A, size);
	float* d_B;
	cudaMalloc(&d_B, size);
	float* d_C;
	cudaMalloc(&d_C, size);

	cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);	


	int threadsPerBlock = 256;
	int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
	matMul<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, nFil, nCol);
	
	cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

	printf("\n\nMatriz resultante:\n");
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			printf("%.2f", h_C[i*nCol+j]);
		}
		printf("\n");	
	}
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);

	free(h_A);
	free(h_B);
	free(h_C);

	return(0);
}
