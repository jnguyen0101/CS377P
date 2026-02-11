# Assignment 1: Program Counters

## Overview

This project implements and benchmarks six loop-order variants of Matrix-Matrix
Multiplication using complex double arithmetic. Performance is measured using
PAPI hardware counters and POSIX `clock_gettime` timers.

## Files Included

- mmm.c: The driver containing the 6 MMM variants and instrumentation.
- Makefile: Automates the compilation process with `-O3` and `-lpapi`.

## How to Run

Use the Makefile by typing `make` to compile the program with the `-O3` and
`-lpapi` flags.

The executable runs the full benchmarking matrix sizes of 50x50, 100x100,
200x200, 400x400, 800x800, 1200x1200, 1600x1600 and 2000x2000 as well as a
single specified matrix size.

### Running PAPI Suite

To collect hardware counters for the entire range (50 to 2000), run:

```bash
mmm papi
```

### Running Timing Suite

To measure execution time using `CLOCK_THREAD_CPUTIME_ID` and `CLOCK_REALTIME`
for the entire range, run:

```bash
mmm clocktime
```

### Running Specific Sizes

You can also run PAPI/Clocktime measurements for a single matrix size (e.g. 
250x250):

```bash
mmm papi 250

mmm clocktime 250
```

## Output

For each variant and size, the program outputs the following information:

Example PAPI Output:

```
MMM Variant: ijk, Size: 50
Total Cycles: 1231089
Total Instructions: 2788145
Total Load/Store Instructions: 502656
Total Floating Point Instructions: 1005292
L1 Data Cache Accesses: 534417
L1 Data Cache Misses: 30658
L2 Data Cache Accesses: 30658
L2 Data Cache Misses: 351
```

Example Clocktime Output:

```
MMM Variant: ijk, Size: 50
CPU Time Used: 0.000464 seconds
```

## Directory Structure

```
  CS377P/assignment_1/
  ├── tables/                               # Collected data for each matrix variant and size
  │   ├── L1_Miss_Rates.csv
  │   ├── L2_Miss_Rates.csv
  │   ├── Total_Floating_Point_Ins.csv
  │   └── Total_Load_Store_Ins.csv
  ├── assignment_1.txt                      # Assignment instructions
  ├── clocktime.o                           # Output for clock_gettime
  ├── Makefile                              # Build script for compiling mmm.c
  ├── mmm.c                                 # Main source code for MMM and measurements
  ├── papi.o                                # Output for PAPI
  └── README.md                             # Project overview and documentation
```