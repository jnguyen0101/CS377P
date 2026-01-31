#include <complex.h>
#include <papi.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_SIZES 8
#define NUM_VARIANTS 6

int sizes[NUM_SIZES] = {50, 100, 200, 400, 800, 1200, 1600, 2000};
char *variants[NUM_VARIANTS] = {"ijk", "jik", "jki", "kji", "ikj", "kij"};

// Function type for MMM variants
typedef void (*mmm_fun)(int, complex double*, complex double*, complex double*);

// Function prototypes
void ijk(int n, complex double *A, complex double *B, complex double *C);
void jik(int n, complex double *A, complex double *B, complex double *C);
void jki(int n, complex double *A, complex double *B, complex double *C);
void kji(int n, complex double *A, complex double *B, complex double *C);
void ikj(int n, complex double *A, complex double *B, complex double *C);
void kij(int n, complex double *A, complex double *B, complex double *C);
void run_mmm_PAPI(mmm_fun fun, int n, char* variant);
void populate_matrices(int n, complex double *A, complex double *B,
                       complex double *C);

// Matrix multiplication implementations with different loop orderings
void ijk(int n, complex double *A, complex double *B, complex double *C) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            complex double sum = 0.0 + 0.0 * I;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

void jik(int n, complex double *A, complex double *B, complex double *C) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            complex double sum = 0.0 + 0.0 * I;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

void jki(int n, complex double *A, complex double *B, complex double *C) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            complex double r = B[k * n + j];
            for (int i = 0; i < n; i++) {
                C[i * n + j] += A[i * n + k] * r;
            }
        }
    }
}

void kji(int n, complex double *A, complex double *B, complex double *C) {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            complex double r = B[k * n + j];
            for (int i = 0; i < n; i++) {
                C[i * n + j] += A[i * n + k] * r;
            }
        }
    }
}

void ikj(int n, complex double *A, complex double *B, complex double *C) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            complex double r = A[i * n + k];
            for (int j = 0; j < n; j++) {
                C[i * n + j] += r * B[k * n + j];
            }
        }
    }
}

void kij(int n, complex double *A, complex double *B, complex double *C) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            complex double r = A[i * n + k];
            for (int j = 0; j < n; j++) {
                C[i * n + j] += r * B[k * n + j];
            }
        }
    }
}

/**
 * Initializes matrices A and B to sample complex doubles and matrix C to zero
 */
void populate_matrices(int n, complex double *A, complex double *B,
                       complex double *C) {
    for (int i = 0; i < n * n; i++) {
        A[i] = 1.0 + 1.0 * I;
        B[i] = 2.0 + 2.0 * I;
        C[i] = 0.0 + 0.0 * I;
    }
}

/**
 * Runs a specific MMM variant with PAPI performance measurement
 */
void run_mmm_PAPI(mmm_fun fun, int n, char* variant) {
    int EventSet1 = PAPI_NULL;
    int EventSet2 = PAPI_NULL;
    int EventSet3 = PAPI_NULL;

    // Initialize matrices
    complex double *A = (complex double*)malloc(n * n * sizeof(complex double));
    complex double *B = (complex double*)malloc(n * n * sizeof(complex double));
    complex double *C = (complex double*)malloc(n * n * sizeof(complex double));

    // Events to measure
    int events_1[4] = {
        PAPI_TOT_CYC,        // Total cycles
        PAPI_TOT_INS,        // Total instructions
        PAPI_LD_INS,         // Total load store instructions
        PAPI_FP_INS,         // Total floating point instructions
    };
    int events_2[4] = {
        PAPI_L1_DCA,         // L1 data cache accesses
        PAPI_L1_DCM,         // L1 data cache misses
    };
    int events_3[4] = {
        PAPI_L2_DCA,         // L2 data cache accesses
        PAPI_L2_DCM          // L2 data cache misses
    };

    // Create Event Sets
    PAPI_create_eventset(&EventSet1);
    PAPI_add_events(EventSet1, events_1, 4);
    PAPI_create_eventset(&EventSet2);
    PAPI_add_events(EventSet2, events_2, 2);
    PAPI_create_eventset(&EventSet3);
    PAPI_add_events(EventSet3, events_3, 2);
    long long counter_values_1[4];
    long long counter_values_2[2];
    long long counter_values_3[2];

    // Run MMM variant with PAPI measurements
    populate_matrices(n, A, B, C);
    PAPI_start(EventSet1);
    fun(n, A, B, C);
    PAPI_stop(EventSet1, counter_values_1);

    populate_matrices(n, A, B, C);
    PAPI_start(EventSet2);
    fun(n, A, B, C);
    PAPI_stop(EventSet2, counter_values_2);

    populate_matrices(n, A, B, C);
    PAPI_start(EventSet3);
    fun(n, A, B, C);
    PAPI_stop(EventSet3, counter_values_3);

    // Print results
    printf("MMM Variant: %s, Size: %d\n", variant, n);
    printf("Total Cycles: %lld\n", counter_values_1[0]);
    printf("Total Instructions: %lld\n", counter_values_1[1]);
    printf("Total Load/Store Instructions: %lld\n", counter_values_1[2]);
    printf("Total Floating Point Instructions: %lld\n", counter_values_1[3]);
    printf("L1 Data Cache Accesses: %lld\n", counter_values_2[0]);
    printf("L1 Data Cache Misses: %lld\n", counter_values_2[1]);
    printf("L2 Data Cache Accesses: %lld\n", counter_values_3[0]);
    printf("L2 Data Cache Misses: %lld\n", counter_values_3[1]);
    printf("--------------------------------------------------\n");

    // Clean up
    PAPI_cleanup_eventset(EventSet1);
    PAPI_cleanup_eventset(EventSet2);
    PAPI_cleanup_eventset(EventSet3);
    PAPI_destroy_eventset(&EventSet1);
    PAPI_destroy_eventset(&EventSet2);
    PAPI_destroy_eventset(&EventSet3);
    free(A); free(B); free(C);
}

int main(int argc, char *argv[]) {
    // Initialize PAPI
    int retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        printf("PAPI library init error!\n");
        exit(1);
    }

    mmm_fun mmm_variants[NUM_VARIANTS] = {ijk, jik, jki, kji, ikj, kij};

    // Run each MMM variant for each matrix size
    for (int i = 0; i < NUM_SIZES; i++) {
        for (int j = 0; j < NUM_VARIANTS; j++) {
            run_mmm_PAPI(mmm_variants[j], sizes[i], variants[j]);
        }
    }

    return 0;
}