// Matrix multiplication by parts
// Elements stored in row-major order

using namespace std;
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cuda.h>
#include <stdlib.h>
#include <windows.h>
#include "helper_timer.h"

#define GRID_SIZE 18
#define BLOCK_SIZE 32

typedef struct
{	
	int width;
	int height;
	float *elements;
} Matrix;

// Forward declaration of matrix mult
__global__ void MatMulKernel (const Matrix, const Matrix, Matrix);

// Host code
void MatMul(const Matrix A, const Matrix B, Matrix C)
{
	// Load matrices A and B to device memory
	Matrix d_A;
	d_A.width = A.width; 
	d_A.height = A.height;
	size_t size = A.width * A.height * sizeof(float);

	cudaMalloc((void**) &d_A.elements, size);
	cudaMemcpy(d_A.elements, A.elements, size, cudaMemcpyHostToDevice);
	
	Matrix d_B;
	d_B.width = B.width; 
	d_B.height = B.height;
	size = B.width * B.height * sizeof(float);

	cudaMalloc((void**) &d_B.elements, size);
	cudaMemcpy(d_B.elements, B.elements, size, cudaMemcpyHostToDevice);
	
	// allocate C in device
	Matrix d_C;
	d_C.width = C.width;
	d_C.height = C.height;	
	size = d_C.width * d_C.height * sizeof(float);

	cudaMalloc((void**) &d_C.elements, size);
	
	// call kernel
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE); // define the block size (what is the best value?) 
    dim3 dimGrid(GRID_SIZE, GRID_SIZE); //  choose grid size depending on problem size 
        
	MatMulKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C);
	
	// copy C to host
	cudaMemcpy(C.elements, d_C.elements, size, cudaMemcpyDeviceToHost);
	
	// free device memory
	cudaFree(d_A.elements);
	cudaFree(d_B.elements);
	cudaFree(d_C.elements);
}

//matrix multiplication kernel
__global__ void MatMulKernel(Matrix A, Matrix B, Matrix C)
{	
	// each thread computes one element of C and acumulates results to Cvalue

	float Cvalue = 0; 
	int row = blockIdx.y * blockDim.y + threadIdx.y; 
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	// printf("%d - %d\n", row, col);

	if ((row >= A.height) || (col >= B.width))
	{
		return;
	}

	for (int e = 0; e < A.width; e++) 
	{
		Cvalue += A.elements[row * A.width + e] * B.elements[e * B.width + col];	
	}

	C.elements[row * C.width + col] = Cvalue;
}

//matrix multiplication CPU
void MatMulCPU(Matrix A, Matrix B, Matrix C)
{	
	for (int i = 0; i < A.height; i++) 
	{
		for (int j = 0; j < B.width; j++) 
		{
			printf("%d - %d\n", i, j);
			C.elements[i * C.width + j] = 0;
			for (int k = 0; k < A.width; k++) 
			{
				C.elements[i * C.width + j] += A.elements[i * A.width + k] * B.elements[k * B.width + j];
				// printf("%d, %d, %d, %d, %d, %d\n", k, i * C.width + j, i * A.width + k, k * B.width + j, C.width, C.height);
			}
			printf("xd\n");
		}
	}
}

bool checkMatrices(Matrix C, Matrix D)
{
	for (int i = 0; i < C.width; i++) 
	{
		for (int j = 0; j < D.width; j++) 
		{
			if (C.elements[i * C.width + j] != D.elements[i * D.width + j])
				return false;
		}
	}

	return true;
}

int main(int argc, char * const argv[])
{	
	if (argc != 3)
	{
		printf("Provide matrix files \n");
		return -1;
	}

	char *filename1 = argv[1];
	char *filename2 = argv[2];

	// Time variables

    // GPU time
    StopWatchInterface *timer;

    // CPU time - for Windows
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double interval;

	Matrix A;
	Matrix B;
	Matrix C;
	Matrix D;
	
	A.width = 640;
	B.width = 480;
	C.width = 640;
	D.width = 640;
	
	A.height = 480;
	B.height = 320;
	C.height = 320;
	D.height = 320;
	
	A.elements = new float[A.width * A.height * 2];
	B.elements = new float[B.width * B.height * 2];
	C.elements = new float[C.width * C.height * 2];
	D.elements = new float[D.width * D.height * 2];
	
	//fill matrices
	std::ifstream A_input;
	std::ifstream B_input;
	A_input.open(filename1);
	B_input.open(filename2);
	
	float a, b;
	A_input >> a;	
	B_input >> b;	
	int i = 0;

	while (!A_input.eof())
	{	
		A.elements[i] = a;
		A_input >> a;	
		i += 1;
	}

	i = 0;

	while (!B_input.eof())
	{	
		B.elements[i] = b;
		B_input >> b;	
		i += 1;
	}

	A_input.close();
	B_input.close();

	// GPU part

	timer=NULL;
	sdkCreateTimer(&timer);
	sdkResetTimer(&timer);
	sdkStartTimer(&timer);

	MatMul(A, B, C);

	cudaThreadSynchronize();
	sdkStopTimer(&timer);
	float time = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);

	// CPU part

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	MatMulCPU(A, B, D);

	QueryPerformanceCounter(&end);
	interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart * 1000;
	
	if(!checkMatrices(C, D))
	{
		printf("CPU and GPU mul didn't return the same results!\n");
	}
	
	std::ofstream C_output;
	std::ofstream D_output;
	C_output.open("C.txt");
	D_output.open("D.txt");

	for (int i = 0; i < C.width; i++)
	{	
		for (int j = 0; j < C.height; j++)
		{
			C_output << C.elements[i * C.width + j] << "\t";
			D_output << D.elements[i * C.width + j] << "\t";
		}

		C_output << endl;
		D_output << endl;
	}

	printf("Time for the kernel: %f ms\n", time);
	printf("CPU time: %f ms\n", interval);

	return 0;
}
	 