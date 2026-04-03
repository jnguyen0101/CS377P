# Assignment 4: Shared-memory Parallel Programming

## Overview

This assignment explores the implementation of shared-memory parallel programs
using Pthreads and C++ Atomics. The core task is to approximate the value of
$\pi$ via numerical integration. By incrementally modifying the implementation,
we study critical performance factors in parallel computing, including:
- Synchronization Overhead: The cost of mutexes vs. lock-free programming.
- True-Sharing: Performance degradation when multiple threads update the same
memory location.
- False-Sharing: Performance degradation when threads update independent
variables that reside on the same cache line.
- Synchronization Primitives: Comparing `pthread_join` with barriers.

## How to Run

### Compilation

Use the provided Makefile to compile all versions of the program:

```bash
make
```

### Execution

Each parallel program accepts the number of threads (1/2/4/8) as a command-line
argument. For example, to run the atomic version with 4 threads:

```bash
./pi_parallel_atomic 4
```

To run the sequential versions:
- `pi_seq` is run without any arguments
- `pi_semicircle` can be run with an integer argument that denotes the number of points to be used

```bash
./pi_seq
./pi_semicircle
./pi_semicircle 20
```

## Performance Measurements

The programs measure the execution time of the computation phase (excluding
thread creation/setup) in nanoseconds.

### Speedup Calculation

Speedup is calculated as:

$$Speedup(n) = \frac{T_{serial}}{T_{parallel}(n)}$$

Note: $T_{serial}$ refers to the pure sequential code, while $T_{parallel}(1)$
includes the overhead of parallel primitives even when running on a single core.

## Directory Structure

```
  CS377P/assignment_4/
  ├── compiled/                             # Compiled code
  ├── programs/
  │   ├── pi_seq.cpp                        # Sequential implementation of pi estimation
  │   ├── pi_semicircle.cpp                 # Sequential estimation of a semicircle area to find pi
  │   ├── pi_parallel_race.cpp              # Pthread implementation with no synchronization
  │   ├── pi_parallel_mutex.cpp             # Pthread implementation using pthread_mutex_t
  │   ├── pi_parallel_atomic.cpp            # Lock-free implementation using std::atomic and compare_exchange_weak
  │   ├── pi_parallel_false.cpp             # Study of false-sharing using a global array
  │   ├── pi_parallel_optimized.cpp         # Implementation using local thread variables to eliminate sharing
  │   └── pi_parallel_barrier.cpp           # Implementation using pthread_barrier_t
  ├── Makefile                              # Script to compile all variants with -O3 and -lpthread flags
  ├── assignment_4.txt                      # Assignment instructions
  └── README.md                             # Project overview and documentation
```