#include <complex.h>
#include <papi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NUM_SIZES 8
#define NUM_VARIANTS 6
#define CACHE_FLUSH_SIZE (64 * 1024 * 1024)

int sizes[NUM_SIZES] = {50, 100, 200, 400, 800, 1200, 1600, 2000};
char *variants[NUM_VARIANTS] = {"ijk", "jik", "jki", "kji", "ikj", "kij"};

// Function type for MMM variants
typedef void (*mmm_fun)(int, complex double*, complex double*, complex double*);

void ijk(int n, complex double *A, complex double *B, complex double *C);
void jik(int n, complex double *A, complex double *B, complex double *C);
void jki(int n, complex double *A, complex double *B, complex double *C);
void kji(int n, complex double *A, complex double *B, complex double *C);
void ikj(int n, complex double *A, complex double *B, complex double *C);
void kij(int n, complex double *A, complex double *B, complex double *C);
void run_mmm_PAPI(mmm_fun fun, int n, char* variant);
void populate_matrices(int n, complex double *A, complex double *B,
                       complex double *C);
void flush_cache();


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

    // Initialize matrices
    complex double *A = (complex double*)malloc(n * n * sizeof(complex double));
    complex double *B = (complex double*)malloc(n * n * sizeof(complex double));
    complex double *C = (complex double*)malloc(n * n * sizeof(complex double));

    // Events to measure
    int events_1[4] = {
        PAPI_TOT_CYC,        // Total cycles
        PAPI_TOT_INS,        // Total instructions
        PAPI_LST_INS,        // Total load store instructions
        PAPI_FP_INS,         // Total floating point instructions
    };
    int events_2[4] = {
        PAPI_L1_DCA,         // L1 data cache accesses
        PAPI_L1_DCM,         // L1 data cache misses
        PAPI_L2_DCA,         // L2 data cache accesses
        PAPI_L2_DCM          // L2 data cache misses
    };

    // Create Event Sets
    if (PAPI_create_eventset(&EventSet1) != PAPI_OK)
        printf("Error creating PAPI Event Set 1\n");
    if (PAPI_add_events(EventSet1, events_1, 4) != PAPI_OK)
        printf("Error adding events to PAPI Event Set 1\n");
    if (PAPI_create_eventset(&EventSet2) != PAPI_OK)
        printf("Error creating PAPI Event Set 2\n");
    if (PAPI_add_events(EventSet2, events_2, 4) != PAPI_OK)
        printf("Error adding events to PAPI Event Set 2\n");
    long long counter_values_1[4] = {0};
    long long counter_values_2[4] = {0};

    // Run MMM variant with PAPI measurements
    populate_matrices(n, A, B, C);
    flush_cache();
    if (PAPI_start(EventSet1) != PAPI_OK)
        printf("Error starting PAPI Event Set 1\n");
    fun(n, A, B, C);
    if (PAPI_stop(EventSet1, counter_values_1) != PAPI_OK)
        printf("Error stopping PAPI Event Set 1\n");

    populate_matrices(n, A, B, C);
    flush_cache();
    if (PAPI_start(EventSet2) != PAPI_OK)
        printf("Error starting PAPI Event Set 2\n");
    fun(n, A, B, C);
    if (PAPI_stop(EventSet2, counter_values_2) != PAPI_OK)
        printf("Error stopping PAPI Event Set 2\n");

    // Print results
    printf("MMM Variant: %s, Size: %d\n", variant, n);
    printf("Total Cycles: %lld\n", counter_values_1[0]);
    printf("Total Instructions: %lld\n", counter_values_1[1]);
    printf("Total Load/Store Instructions: %lld\n", counter_values_1[2]);
    printf("Total Floating Point Instructions: %lld\n", counter_values_1[3]);
    printf("L1 Data Cache Accesses: %lld\n", counter_values_2[0]);
    printf("L1 Data Cache Misses: %lld\n", counter_values_2[1]);
    printf("L2 Data Cache Accesses: %lld\n", counter_values_2[2]);
    printf("L2 Data Cache Misses: %lld\n", counter_values_2[3]);
    printf("--------------------------------------------------\n");

    // Clean up
    if (PAPI_cleanup_eventset(EventSet1) != PAPI_OK)
        printf("Error cleaning up PAPI Event Set 1\n");
    if (PAPI_cleanup_eventset(EventSet2) != PAPI_OK)
        printf("Error cleaning up PAPI Event Set 2\n");
    if (PAPI_destroy_eventset(&EventSet1) != PAPI_OK)
        printf("Error destroying PAPI Event Set 1\n");
    if (PAPI_destroy_eventset(&EventSet2) != PAPI_OK)
        printf("Error destroying PAPI Event Set 2\n");
    free(A); free(B); free(C);
}

/**
 * Runs a specific MMM variant with clock time measurement
 */
void run_mmm_clocktime(mmm_fun fun, int n, char* variant) {
    // Initialize matrices
    complex double *A = (complex double*)malloc(n * n * sizeof(complex double));
    complex double *B = (complex double*)malloc(n * n * sizeof(complex double));
    complex double *C = (complex double*)malloc(n * n * sizeof(complex double));

    // Run MMM variant with clock cputime measurement
    struct timespec start_c, end_c;
    populate_matrices(n, A, B, C);
    flush_cache();
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_c);
    fun(n, A, B, C);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_c);

    // Calculate CPU time used
    double cpu_time_used = (end_c.tv_sec - start_c.tv_sec) +
                           (end_c.tv_nsec - start_c.tv_nsec) / 1e9;

    // Print results
    printf("MMM Variant: %s, Size: %d\n", variant, n);
    printf("CPU Time Used: %f seconds\n", cpu_time_used);
    printf("--------------------------------------------------\n");

    // Run MMM variant with clock realtime measurement
    struct timespec start_r, end_r;
    populate_matrices(n, A, B, C);
    flush_cache();
    clock_gettime(CLOCK_REALTIME, &start_r);
    fun(n, A, B, C);
    clock_gettime(CLOCK_REALTIME, &end_r);

    // Calculate CPU time used
    double real_time_used = (end_r.tv_sec - start_r.tv_sec) +
                           (end_r.tv_nsec - start_r.tv_nsec) / 1e9;

    // Print results
    printf("MMM Variant: %s, Size: %d\n", variant, n);
    printf("Real Time Used: %f seconds\n", real_time_used);
    printf("--------------------------------------------------\n");

    // Clean up
    free(A); free(B); free(C);
}

/**
 * Flushes the CPU cache by allocating and accessing a large buffer
 */
void flush_cache() {
    volatile char *buffer = (char *)malloc(CACHE_FLUSH_SIZE);
    for (size_t i = 0; i < CACHE_FLUSH_SIZE; i++) {
        buffer[i] = (char)i;
    }
    free((void *)buffer);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <papi|clocktime>\n", argv[0]);
        return 1;
    }

    // Initialize PAPI
    int retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        printf("PAPI library init error!\n");
        exit(1);
    }

    mmm_fun mmm_variants[NUM_VARIANTS] = {ijk, jik, jki, kji, ikj, kij};

    if (strcmp(argv[1], "papi") == 0) {
        // Run PAPI measurements
        printf("PAPI Results:\n");
        for (int i = 0; i < NUM_SIZES; i++) {
            for (int j = 0; j < NUM_VARIANTS; j++) {
                run_mmm_PAPI(mmm_variants[j], sizes[i], variants[j]);
            }
        }
    } else if (strcmp(argv[1], "clocktime") == 0) {
        // Run clocktime measurements
        printf("Clock Time Results:\n");
        for (int i = 0; i < NUM_SIZES; i++) {
            for (int j = 0; j < NUM_VARIANTS; j++) {
                run_mmm_clocktime(mmm_variants[j], sizes[i], variants[j]);
            }
        }
    } else {
        printf("Error: Invalid option '%s'\n", argv[1]);
        printf("Usage: %s <papi|clocktime>\n", argv[0]);
        return 1;
    }

    return 0;
}