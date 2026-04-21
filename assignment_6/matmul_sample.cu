#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define DEFAULT_N 4
#define TILE_SIZE 8

void cudaCheckError(cudaError_t err, const char *file, int line) {
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA error at %s:%d: %s\n", file, line, cudaGetErrorString(err));
        exit(1);
    }
}

#define cudaCheck(call) cudaCheckError((call), __FILE__, __LINE__)

__global__ void matmul_kernel_single(const float *A, const float *B, float *C, int width) {
    // Your codes will be here for single thread MM...
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            float c_val = 0;
            for (int k = 0; k < width; k++) {
                c_val += A[i * width + k] * B[k * width + j];
            }
            C[i * width + j] = c_val;
        }
    }
}

__global__ void matmul_kernel_naive(const float *A, const float *B, float *C, int width) {
    // Your codes will be here for naive MM...
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    // Check bounds
    if (row < width && col < width) {
        float c_val = 0;

        // Each thread computes one element
        for (int k = 0; k < width; k++) {
            c_val += A[row * width + k] * B[k * width + col];
        }
        C[row * width + col] = c_val;
    }
}

__global__ void matmul_kernel_tiled(const float *A, const float *B, float *C, int width) {
    // Your code will be here for tiled MM...

    // Code from lecture slide 52
    __shared__ float As[TILE_SIZE][TILE_SIZE];
    __shared__ float Bs[TILE_SIZE][TILE_SIZE];

    int bx = blockIdx.x; int by = blockIdx.y;
    int tx = threadIdx.x; int ty = threadIdx.y;

    int row = by * TILE_SIZE + ty;
    int col = bx * TILE_SIZE + tx;

    // Loop to compute C
    float c_val = 0;
    for (int m = 0; m < width / TILE_SIZE; ++m) {
        // Load A and B tiles into shared memory
        As[ty][tx] = A[row * width + (m * TILE_SIZE + tx)];
        Bs[ty][tx] = B[col + (m * TILE_SIZE + ty) * width];
        __syncthreads();

        for (int k = 0; k < TILE_SIZE; ++k) {
            c_val += As[ty][k] * Bs[k][tx];
        }

        __syncthreads();
        C[row * width + col] = c_val;
    }
}

int main(int argc, char**argv) {
    int N = DEFAULT_N;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [matrix_size]\n", argv[0]);
        return 1;
    }

    if (argc == 2) {
        N = atoi(argv[1]);
        if (N <= 0) {
            fprintf(stderr, "Matrix size must be a positive integer.\n");
            return 1;
        }
    }

    size_t bytes = N * N * sizeof(float);

    // Allocate matrices on CPU memory
    float *h_A = (float *)malloc(bytes);
    float *h_B = (float *)malloc(bytes);
    float *h_C = (float *)malloc(bytes);

    for (int i = 0; i < N * N; i++) {
        h_A[i] = (float)(i + 1);
        h_B[i] = 1.0f;
    }

    float *d_A, *d_B, *d_C;
    // Allocate matrices on GPU memory
    cudaCheck(cudaMalloc((void **)&d_A, bytes));
    cudaCheck(cudaMalloc((void **)&d_B, bytes));
    cudaCheck(cudaMalloc((void **)&d_C, bytes));

    // Copy matrices from CPU to GPU memory
    cudaCheck(cudaMemcpy(d_A, h_A, bytes, cudaMemcpyHostToDevice));
    cudaCheck(cudaMemcpy(d_B, h_B, bytes, cudaMemcpyHostToDevice));

    // Define the number of threads per threadblock, and number of threadblocks...
    dim3 blockDim(TILE_SIZE, TILE_SIZE);
    dim3 gridDim((N + blockDim.x - 1) / blockDim.x, (N + blockDim.y - 1) / blockDim.y);

    // Time measurement for CUDA kernel
    cudaEvent_t start, stop;
    float elapsed_ms;
    cudaCheck(cudaEventCreate(&start));
    cudaCheck(cudaEventCreate(&stop));

    cudaCheck(cudaEventRecord(start));

    // Single kernel
    // matmul_kernel_single<<<1, 1>>>(d_A, d_B, d_C, N);

    // Naive kernel
    // matmul_kernel_naive<<<gridDim, blockDim>>>(d_A, d_B, d_C, N);

    // Tiled kernel
    // matmul_kernel_tiled<<<gridDim, blockDim>>>(d_A, d_B, d_C, N);

    cudaCheck(cudaGetLastError());
    cudaCheck(cudaEventRecord(stop));
    cudaCheck(cudaEventSynchronize(stop));
    cudaCheck(cudaEventElapsedTime(&elapsed_ms, start, stop));

    // Copy matrices from GPU to CPU memory
    cudaCheck(cudaMemcpy(h_C, d_C, bytes, cudaMemcpyDeviceToHost));
    printf("Kernel time: %.3f ms\n", elapsed_ms);

    cudaCheck(cudaFree(d_A));
    cudaCheck(cudaFree(d_B));
    cudaCheck(cudaFree(d_C));
    cudaCheck(cudaEventDestroy(start));
    cudaCheck(cudaEventDestroy(stop));

    free(h_A);
    free(h_B);
    free(h_C);

    return 0;
}