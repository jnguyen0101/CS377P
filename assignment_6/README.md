# Assignment 6: Matrix Multiplication with CUDA

## Overview

This assignment explores GPU-accelerated computing by implementing and optimizing matrix multiplication kernels using the CUDA programming model. The primary goal is to understand how memory hierarchy and thread organization impact performance on massive datasets (up to $10,000 \times 10,000$ matrices). We transition from a baseline single-threaded implementation to a massively parallel tiled approach that leverages shared memory to minimize global memory bottlenecks.

Key concepts explored include:
- **Massive Parallelism:** Transitioning from single-threaded execution to a "one thread per element" model to utilize the thousands of cores available on NVIDIA GPUs.
- **Shared Memory Tiling:** Implementing memory-efficient kernels that load sub-blocks of matrices into on-chip shared memory to reduce global memory latency and bandwidth pressure.
- **Performance Profiling:** Utilizing NVIDIA Nsight Compute (NCU) to analyze hardware utilization, specifically comparing Compute (SM) [%] and Memory [%] throughput.
- **Hardware Bottlenecks:** Identifying whether a kernel is "compute-bound" or "memory-bound" and how tile size affects the "Speed of Light" metrics on the TACC Lonestar GPU nodes.
- **Scaling Analysis:** Measuring speedup across varying matrix dimensions and tile sizes (8, 16, 32) to determine optimal configurations for large-scale linear algebra.

## Directory Structure

```
  CS377P/assignment_6/
  ├── graphs/                               # Visual comparisons of execution times and tile size scaling
  ├── matmul_sample.cu                      # Code to implement the kernels and record execution times
  ├── assignment_6.txt                      # Assignment instructions
  ├── tacc_user_guide.txt                   # Assignment instructions
  ├── assignment_6_report.pdf               # Final summary of implementation, profiling, and conclusions
  └── README.md                             # Project overview and documentation
```
