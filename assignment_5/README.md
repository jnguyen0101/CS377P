# Assignment 4: Shared-memory Parallel Programming

## Overview

This assignment focuses on the implementation and performance analysis of parallel partitioning within the Quicksort algorithm using pThreads. While the recursive calls in Quicksort are easily parallelized, the partitioning step often remains a sequential bottleneck. To address this, we implement the partitioning step using high-level data-parallel operators. Key concepts explored include:
- **Parallel Partitioning:** Moving away from the sequential Hoare or Lomuto schemes to a parallelizable approach using auxiliary memory.
- **Filter and Scan Operators:** Implementing the filter operator, which relies on a parallel scan (prefix sum) to determine the output indices for elements in parallel.
- **Median-of-Three Rule:** Utilizing a robust pivot selection strategy to avoid worst-case $O(n^2)$ behavior.
- **Performance Scaling:** Analyzing the efficiency of parallel filters by measuring speedup across varying thread counts (1–32) and large-scale data sets (up to 10M elements)
- **Algorithmic Optimization:** Comparing the overhead and performance of a dual-filter approach versus a refined single-filter implementation.

## Directory Structure

```
  CS377P/assignment_5/
  ├── compiled/                             # Compiled code
  ├── graphs/                               # Graphs to visualize execution times and speedup based on array sizes
  ├── implementations/
  │   ├── partition_seq.cpp                 # Sequential partitioning using median-of-three
  │   ├── partition_two.cpp                 # Parallel partitioning using two filter operations
  │   └── partition_single.cpp              # Optimized parallel partitioning using a single filter
  ├── assignment_4.txt                      # Assignment instructions
  └── README.md                             # Project overview and documentation
```