#include <complex.h>

int sizes[8] = {50, 100, 200, 400, 800, 1200, 1600, 2000};

void ijk (int n, complex double *A, complex double *B, complex double *C) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            complex double sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

void jik (int n, complex double *A, complex double *B, complex double *C) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            complex double sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

void jki (int n, complex double *A, complex double *B, complex double *C) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            complex double r = B[k * n + j];
            for (int i = 0; i < n; i++) {
                C[i * n + j] += A[i * n + k] * r;
            }
        }
    }
}

void kji (int n, complex double *A, complex double *B, complex double *C) {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            complex double r = B[k * n + j];
            for (int i = 0; i < n; i++) {
                C[i * n + j] += A[i * n + k] * r;
            }
        }
    }
}

void ikj (int n, complex double *A, complex double *B, complex double *C) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            complex double r = A[i * n + k];
            for (int j = 0; j < n; j++) {
                C[i * n + j] += r * B[k * n + j];
            }
        }
    }
}

void kij (int n, complex double *A, complex double *B, complex double *C) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            complex double r = A[i * n + k];
            for (int j = 0; j < n; j++) {
                C[i * n + j] += r * B[k * n + j];
            }
        }
    }
}

int main() {
    
}