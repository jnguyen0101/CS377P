#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

typedef struct {
    int tid, num_threads;
    long *input, *output, pivot;
    size_t n, *counts, *offsets, *total_lt, *total_eq;
    pthread_barrier_t *barrier;
} thread_data_t;

long select_pivot(long* arr, size_t n) {
    long a = arr[0];
    long b = arr[n/2];
    long c = arr[n-1];
    
    if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
    return c;
}

void* two_filter_partition(void* arg) {
    thread_data_t* d = (thread_data_t*)arg;
    size_t start = d->tid * (d->n / d->num_threads);
    size_t end = (d->tid == d->num_threads - 1) ? d->n : (d->tid + 1) * (d->n / d->num_threads);

    // Filter 1, elements less than pivot
    size_t l_cnt = 0;
    for (size_t i = start; i < end; i++) {
        if (d->input[i] < d->pivot) l_cnt++;
    }
    d->counts[d->tid] = l_cnt;
    pthread_barrier_wait(d->barrier);

    // Scan
    if (d->tid == 0) {
        size_t sum = 0;
        for (int i = 0; i < d->num_threads; i++) {
            d->offsets[i] = sum;
            sum += d->counts[i];
        }
        *(d->total_lt) = sum;
    }
    pthread_barrier_wait(d->barrier);

    // Parallel write
    size_t lt_p = d->offsets[d->tid];
    for (size_t i = start; i < end; i++) {
        if (d->input[i] < d->pivot) d->output[lt_p++] = d->input[i];
    }
    pthread_barrier_wait(d->barrier);

    // Elements equal to pivot
    if (d->tid == 0) {
        size_t e_cnt = 0;
        for (size_t i = 0; i < d->n; i++) {
            if (d->input[i] == d->pivot) e_cnt++;
        }
        *(d->total_eq) = e_cnt;
        for (size_t i = 0; i < e_cnt; i++) {
            d->output[*(d->total_lt) + i] = d->pivot;
        }
    }
    pthread_barrier_wait(d->barrier);

    // Filter 2, elements greater than pivot
    size_t g_cnt = 0;
    for (size_t i = start; i < end; i++) {
        if (d->input[i] > d->pivot) g_cnt++;
    }
    d->counts[d->tid] = g_cnt;
    pthread_barrier_wait(d->barrier);

    // Scan
    if (d->tid == 0) {
        size_t sum = *(d->total_lt) + *(d->total_eq);
        for (int i = 0; i < d->num_threads; i++) {
            d->offsets[i] = sum;
            sum += d->counts[i];
        }
    }
    pthread_barrier_wait(d->barrier);

    // Parallel write
    size_t gt_p = d->offsets[d->tid];
    for (size_t i = start; i < end; i++) {
        if (d->input[i] > d->pivot) d->output[gt_p++] = d->input[i];
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    uint64_t execTime;
    struct timespec tick, tock;

    size_t sizes[] = {1024, 10240, 102400, 1048576, 10485760};
    int threads_sizes[] = {1, 2, 4, 8, 16, 32};

    for (int t = 0; t < 6; t++) {
        int n_threads = threads_sizes[t];
        for (int s = 0; s < 5; s++) {
            size_t n = sizes[s];
            long *in = (long*)malloc(n * sizeof(long));
            long *out = (long*)malloc(n * sizeof(long));

            // Create array element values
            for (size_t i = 0; i < n; i++) {
                long val = (long)(i + 1);
                in[i] = val * ((val % 2 == 0) ? -1 : 1);
            }
            
            pthread_t threads[n_threads];
            thread_data_t td[n_threads];
            size_t cnts[n_threads], offs[n_threads];
            size_t total_lt = 0, total_eq = 0;
            pthread_barrier_t b;
            pthread_barrier_init(&b, NULL, n_threads);

            clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

            long pivot = select_pivot(in, n);
            for (int i = 0; i < n_threads; i++) {
                td[i] = {i, n_threads, in, out, pivot, n, cnts, offs, &total_lt, &total_eq, &b};
                pthread_create(&threads[i], NULL, two_filter_partition, &td[i]);
            }
            for (int i = 0; i < n_threads; i++) {
                pthread_join(threads[i], NULL);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
            execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;

            printf("Threads: %d, Size: %zu, Time = %llu ns\n", n_threads, n, (long long unsigned int) execTime);

            pthread_barrier_destroy(&b);
            free(in);
            free(out);
        }
    }
}