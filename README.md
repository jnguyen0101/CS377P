# CS 377P: Programming for Performance

This repository contains the programming assignments completed for **CS 377P:
Programming for Performance** at the **University of Texas at Austin**.

The course explores how modern computer architectures impact program efficiency
and how to write high-performance software through optimization, benchmarking,
and low-level systems programming.

## Course Objectives

To obtain the high level of end-to-end performance needed in problem domains
like graphics, computer games, and machine learning, it is necessary for
programs to exploit many of the features of modern computer architectures.  In
this course, we will study the performance-critical features of modern computer
architectures, and discuss how applications can take advantage of them to obtain
high performance.  This is not a course on software tricks; rather, the emphasis
is on abstractions of computer architecture, understanding performance, and
obtaining performance when you need it.

## Course Topics

Assignments in this repository cover concepts such as:

- Analysis of applications that need high end-to-end performance
- Understanding performance: performance models, Amdahl's law
- Measurement and design of computer experiments
- Micro-benchmarks for abstracting performance-critical aspects of computer
systems
- Memory hierarchy: caches, virtual memory, exploiting spatial and temporal
locality
- Vectors and vectorization
- GPUs and GPU programming
- Multi-core processors and shared-memory programming, OpenMP
- Distributed-memory machines and message-passing programming, MPI
- Self-optimizing software

## Repository Structure

```
  CS377P/
  ├── assignment_1/          # Benchmarks six loop-order matrix multiplication variants using PAPI
  ├── assignment_2/          # Explores how compilers translate and optimize code
  └── README.md              # Repository overview and documentation
```