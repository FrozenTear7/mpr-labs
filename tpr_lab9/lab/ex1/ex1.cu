#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <windows.h>
#include "helper_timer.h"

__global__ void add (int *a, int *b, int *c, int n, int threads) {
    int tid = blockIdx.x * threads + threadIdx.x;

    if(tid < n) {
        c[tid] = a[tid] + b[tid];
    }
}

void addCpu(int *a, int *b, int *c, int n) {
    int i;

    for (i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

bool vectorCheck(int *a, int *b, int n) {
    int i;

    for (i = 0; i < n; i++) {
        if(a[i] != b[i]) {
            printf("UH OH mismatch %d, %d\n", a[i], b[i]);
            return false;
        }
    }

    return true;
}

int main(void) {
    // Time variables

    // GPU time
    StopWatchInterface *timer;

    // CPU time - for Windows
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double interval;

    while(true) {
        int n, blocks, threads;

        printf("Provide n:\n");
        scanf("%d", &n);

        if(n <= 0) {
            printf("n has to be bigger than 0\n");
            continue;
        }

        printf("Provide number of blocks:\n");
        scanf("%d", &blocks);

        if(n <= 1) {
            printf("number of blocks has to be bigger than 1\n");
            continue;
        }

        printf("Provide number of threads:\n");
        scanf("%d", &threads);

        if(n <= 1) {
            printf("number of threads has to be bigger than 1\n");
            continue;
        }

        if(blocks * threads != n) {
            printf("Blocks * threads have to be equal to n\n");
            continue;
        }

        // Other
        int *a = (int*) malloc(n * sizeof(int));
        int *b = (int*) malloc(n * sizeof(int));
        int *c = (int*) malloc(n * sizeof(int));
        int *d = (int*) malloc(n * sizeof(int));

        int *dev_a, *dev_b, *dev_c;

        cudaMalloc((void**) &dev_a, n * sizeof(int));
        cudaMalloc((void**) &dev_b, n * sizeof(int));
        cudaMalloc((void**) &dev_c, n * sizeof(int));
        
        for (int i = 0; i < n; i++) {
            a[i] = i;
            b[i] = i*2;
        }
        
        cudaMemcpy(dev_a, a, n * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_b, b, n * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(dev_c, c, n * sizeof(int), cudaMemcpyHostToDevice);
        
        // GPU part

        timer=NULL;
        sdkCreateTimer(&timer);
        sdkResetTimer(&timer);
        sdkStartTimer(&timer);

        add <<<blocks, threads>>> (dev_a, dev_b, dev_c, n, threads);

        cudaThreadSynchronize();
        sdkStopTimer(&timer);
        float time = sdkGetTimerValue(&timer);
        sdkDeleteTimer(&timer);

        cudaMemcpy(c, dev_c, n * sizeof(int), cudaMemcpyDeviceToHost);
        
        // CPU part

        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);

        addCpu(a, b, d, n);

        QueryPerformanceCounter(&end);
        interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart * 1000;

        for (int i = 0; i < n; i++) {
            // printf("%d+%d=%d, %d\n", a[i], b[i], c[i], d[i]);
        }

        bool sameVectors = vectorCheck(c, d, n);

        if(sameVectors) {
            printf("GPU and CPU vectors are properly added\n");
        } else {
            printf("GPU and CPU vectors are not properly added\n");
        }
        
        printf("Time for the kernel: %f ms\n", time);
        printf("CPU time: %f ms\n", interval);
        
        cudaFree(dev_a);
        cudaFree(dev_b);
        cudaFree(dev_c);
    }
    
    return 0;
}