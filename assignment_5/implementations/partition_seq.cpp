#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

long select_pivot(long* arr, size_t n) {
    long a = arr[0];
    long b = arr[n / 2];
    long c = arr[n - 1];

    if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
    return c;
}

int main(int argc, char *argv[]) {
    uint64_t execTime;
    struct timespec tick, tock;

    size_t sizes[] = {1024, 10240, 102400, 1048576, 10485760};

    for (int s = 0; s < 5; s++) {
        size_t n = sizes[s];
        long* input = (long*)malloc(n * sizeof(long));
        long* output = (long*)malloc(n * sizeof(long));
        
        // Create array element values
        for (size_t i = 0; i < n; i++) {
            long val = (long)(i + 1);
            input[i] = val * ((val % 2 == 0) ? -1 : 1);
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

        long pivot = select_pivot(input, n);
        size_t eq_ptr = 0;
    
        // Count elements less than pivot and calculate position of equal to pivot
        for (size_t i = 0; i < n; i++) {
            if (input[i] < pivot) eq_ptr++;
        }
        
        // Count elements equal to pivot and calculate position of greater than pivot
        size_t gt_ptr = eq_ptr;
        for (size_t i = 0; i < n; i++) {
            if (input[i] == pivot) gt_ptr++;
        }

        // Fill auxiliary array
        size_t curr_lt = 0, curr_eq = eq_ptr, curr_gt = gt_ptr;
        for (size_t i = 0; i < n; i++) {
            if (input[i] < pivot) {
                output[curr_lt++] = input[i];
            } else if (input[i] == pivot) {
                output[curr_eq++] = input[i];
            } else {
                output[curr_gt++] = input[i];
            }
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
        execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;

        printf("Size: %zu, Time = %llu ns\n", n, (long long unsigned int) execTime);

        free(input);
        free(output);
    }
}