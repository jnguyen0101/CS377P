# Assignment 2: Compiler Optimizations and x86-64 ISA

## Overview

This assignment focuses on understanding how compilers translate high-level C
code into x86-64 assembly and how different optimization levels ($O1$ vs. $O3$)
affect the resulting machine instructions. Compiler Explorer (Godbolt) is used
to analyze the effects of loop vectorization, register usage, and instruction
selection.

## Technical Specifications

### Input Code

The study is based on a standard array-summation function:

```c
int testFunction(int* input, int length) {
  int sum = 0;
  for (int i = 0; i < length; ++i) {
    sum += input[i];
  }
  return sum;
}
```

### Compiler Settings

The following settings are used in Compiler Explorer:

- Language: C
- Compiler: `x86-64 gcc 9.2`
- Syntax: Uncheck "Intel" and display in AT&T
- Flags: `-O1` (for scalar analysis) or `-O3` (for vectorization analysis)

## Tools and Reference Materials

- **Compiler Tool:** [Compiler Explorer](https://godbolt.org/)
- **Instruction Set Manual:** [Felix Cloutier x86 Reference](https://www.felixcloutier.com/x86/).

## Directory Structure

```
  CS377P/assignment_2
  ├── assignment_2.txt                      # Assignment instructions
  ├── test_function.c                       # Function used for this study
  └── README.md                             # Project overview and documentation
```