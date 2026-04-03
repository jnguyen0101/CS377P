#include <pthread.h> /*used in other parts of the assignment */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>  /* for uint64  */
#include <time.h>    /* for clock_gettime */
#include <atomic>    /*used in other parts of the assignment */

double f(double x) {
    return (4.0/(1+x*x));
}

double pi = 0.0;
int numPoints = 1000000000;
double step;
int num_threads;
double sum[8];

void *compute_pi(void* threadIdPtr) {
    int id = *(int*)threadIdPtr;
    
    for (int i = id; i < numPoints; i += num_threads) {
        double x = step * ((double) i);

        // Add to global array
        sum[id] = sum[id] + step*f(x);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    num_threads = (argc > 1) ? atoi(argv[1]) : 1;
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    step = 1.0 / numPoints;

    uint64_t execTime; /*time in nanoseconds */
    struct timespec tick, tock;

    clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, compute_pi, &thread_ids[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        pi += sum[i];
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &tock);

    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;

    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);

    printf("%.20f\n", pi);
    return 0;
}