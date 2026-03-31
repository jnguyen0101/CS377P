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

## Directory Structure

```
  CS377P/assignment_4/
  ├── implementations/
  │   ├── pi_seq.cpp                        # Sequential implementation of pi estimation
  │   ├── pi_semicircle.cpp                 # Sequential estimation of a semicircle area to find pi
  │   ├── pi_parallel_race.cpp              # Pthread implementation with no synchronization
  │   ├── pi_parallel_mutex.cpp             # Pthread implementation using pthread_mutex_t
  │   ├── pi_parallel_atomic.cpp            # Lock-free implementation using std::atomic and compare_exchange_weak
  │   ├── pi_parallel_false.cpp             # Study of false-sharing using a global array
  │   ├── pi_parallel_optimized.cpp         # Implementation using local thread variables to eliminate sharing
  │   └── pi_parallel_barrier.cpp           # Implementation using pthread_barrier_t
  ├── assignment_4.txt                      # Assignment instructions
  └── README.md                             # Project overview and documentation
```