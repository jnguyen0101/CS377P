# Assignment 3: Numerical Methods for ODEs, PDEs, and Linear Systems

## Overview

This assignment focuses on the application of numerical discretization schemes
to solve ordinary and partial differential equations. It includes anazlying the
stability of the Forward-Euler method, implementing iterative solvers like
Jacobi and Gauss-Seidel for linear systems, and simulating physical phenomena
such as 2D heat conduction and vibrating membranes using the wave equation.

## Technical Specifications

### Discretization Schemes

The following numerical methods are explored in this assignment:

- **Forward-Euler:** Used for first-order ODEs and discretizing derivatives in
second-order equations.
- **Backward-Euler:** Applied to second-order differential equations with
specific initial condition discretization for $y(-h)$.
- **Centered Differences:** Utilized for both spatial and temporal
discretization in the 2D wave equation.
- **Iterative Solvers:** Implementation of Jacobi and Gauss-Seidel methods to
refine approximations of linear systems.

## Tools and Reference Materials

- **Linear Solver:** [MATLAB](https://www.mathworks.com/products/matlab.html)
- **Algorithmic References:** [Gauss-Seidel Method](https://mathworld.wolfram.com/Gauss-SeidelMethod.html)

## Directory Structure

```
  CS377P/assignment_3/
  ├── graphs/                               # Plots created for questions
  ├── assignment_3.pdf                      # Assignment instructions
  ├── figure_1_vibrating_membrane.png       # Diagram for Question 5
  └── README.md                             # Project overview and documentation
```