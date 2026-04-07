#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

typedef struct {
    int tid, num_threads;
    long *input, *output, pivot;
    size_t n;
    size_t *lt_counts, *gt_counts;
    size_t *lt_offsets, *gt_offsets;
    size_t *total_lt, *total_gt;
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

void* single_filter_partition(void* arg) {
    thread_data_t* d = (thread_data_t*)arg;
    size_t start = d->tid * (d->n / d->num_threads);
    size_t end = (d->tid == d->num_threads - 1) ? d->n : (d->tid + 1) * (d->n / d->num_threads);

    // Count elements less than pivot and greater than pivot
    size_t l_cnt = 0, g_cnt = 0;
    for (size_t i = start; i < end; i++) {
        if (d->input[i] < d->pivot) l_cnt++;
        else if (d->input[i] > d->pivot) g_cnt++;
    }
    d->lt_counts[d->tid] = l_cnt;
    d->gt_counts[d->tid] = g_cnt;

    pthread_barrier_wait(d->barrier);

    // Scan
    if (d->tid == 0) {
        // Calculate total counts and offsets for less than pivot
        size_t lt_sum = 0;
        for (int i = 0; i < d->num_threads; i++) {
            d->lt_offsets[i] = lt_sum;
            lt_sum += d->lt_counts[i];
        }
        *(d->total_lt) = lt_sum;

        // Calculate total counts and offsets for greater than pivot
        size_t gt_sum = 0;
        for (int i = 0; i < d->num_threads; i++) {
            gt_sum += d->gt_counts[i];
        }
        *(d->total_gt) = gt_sum;

        size_t eq_cnt = d->n - (lt_sum + gt_sum);
        size_t gt_start_idx = lt_sum + eq_cnt;
        
        size_t current_gt_offset = gt_start_idx;
        for (int i = 0; i < d->num_threads; i++) {
            d->gt_offsets[i] = current_gt_offset;
            current_gt_offset += d->gt_counts[i];
        }

        // Write pivot elements in the middle
        for (size_t i = 0; i < eq_cnt; i++) {
            d->output[lt_sum + i] = d->pivot;
        }
    }

    pthread_barrier_wait(d->barrier);

    // Parallel write
    size_t l_pos = d->lt_offsets[d->tid];
    size_t g_pos = d->gt_offsets[d->tid];

    for (size_t i = start; i < end; i++) {
        if (d->input[i] < d->pivot) {
            d->output[l_pos++] = d->input[i];
        } else if (d->input[i] > d->pivot) {
            d->output[g_pos++] = d->input[i];
        }
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
            size_t lt_counts[n_threads], gt_counts[n_threads];
            size_t lt_offs[n_threads], gt_offs[n_threads];
            size_t total_lt = 0, total_gt = 0;
            pthread_barrier_t b;
            pthread_barrier_init(&b, NULL, n_threads);

            clock_gettime(CLOCK_MONOTONIC_RAW, &tick);
            
            long pivot = select_pivot(in, n);
            for (int i = 0; i < n_threads; i++) {
                td[i] = {i, n_threads, in, out, pivot, n, 
                         lt_counts, gt_counts, lt_offs, gt_offs, 
                         &total_lt, &total_gt, &b};
                pthread_create(&threads[i], NULL, single_filter_partition, &td[i]);
            }
            for (int i = 0; i < n_threads; i++) {
                pthread_join(threads[i], NULL);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
            execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + (tock.tv_nsec - tick.tv_nsec);

            printf("Threads: %d, Size: %zu, Time = %llu ns\n", n_threads, n, (long long unsigned int) execTime);

            pthread_barrier_destroy(&b);
            free(in);
            free(out);
        }
    }
    return 0;
}