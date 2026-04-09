# Assignment 5: Shared-memory parallel programming (II)

## Files Included

- partition_seq.cpp: Sequential partitioning implementation
- partition_single.cpp: Parallel partitioning using a single filter
- partition_two.cpp: Parallel partitioning using two filters
- Makefile: Compiles all of the partitioning implementations (sequential, two filters, and single filter)

## How to Run

Use the Makefile by typing `make` to compile all of the programs.

### Running the sequential partitioning

To measure run times for all array sizes, run:

```bash
partition_seq
```

### Running the parallel partitioning using two filters

To measure run times for all array sizes and thread counts, run:

```bash
partition_two
```

### Running the parallel partitioning using a single filter

To measure run times for all array sizes and thread counts, run:

```bash
partition_single
```

## Output

For each array size and thread count (for parallel implementations), the program outputs the following information:

Example sequential output:

```
Size: 1024, Time = 34451 ns
Size: 10240, Time = 373910 ns
Size: 102400, Time = 1633890 ns
Size: 1048576, Time = 7084856 ns
Size: 10485760, Time = 71681727 ns
```

Example parallel output:

```
Threads: 1, Size: 1024, Time = 421021 ns
Threads: 1, Size: 10240, Time = 871434 ns
Threads: 1, Size: 102400, Time = 5304387 ns
Threads: 1, Size: 1048576, Time = 10837395 ns
Threads: 1, Size: 10485760, Time = 106640935 ns
Threads: 2, Size: 1024, Time = 324113 ns
Threads: 2, Size: 10240, Time = 325483 ns
Threads: 2, Size: 102400, Time = 886899 ns
Threads: 2, Size: 1048576, Time = 6439406 ns
Threads: 2, Size: 10485760, Time = 63935286 ns
Threads: 4, Size: 1024, Time = 146738 ns
Threads: 4, Size: 10240, Time = 377094 ns
Threads: 4, Size: 102400, Time = 543693 ns
Threads: 4, Size: 1048576, Time = 4291322 ns
Threads: 4, Size: 10485760, Time = 41266287 ns
Threads: 8, Size: 1024, Time = 507386 ns
Threads: 8, Size: 10240, Time = 293945 ns
Threads: 8, Size: 102400, Time = 492373 ns
Threads: 8, Size: 1048576, Time = 3130218 ns
Threads: 8, Size: 10485760, Time = 31431775 ns
Threads: 16, Size: 1024, Time = 356704 ns
Threads: 16, Size: 10240, Time = 349949 ns
Threads: 16, Size: 102400, Time = 566229 ns
Threads: 16, Size: 1048576, Time = 3045742 ns
Threads: 16, Size: 10485760, Time = 30551052 ns
Threads: 32, Size: 1024, Time = 682040 ns
Threads: 32, Size: 10240, Time = 675161 ns
Threads: 32, Size: 102400, Time = 897907 ns
Threads: 32, Size: 1048576, Time = 3536214 ns
Threads: 32, Size: 10485760, Time = 32913864 ns
```