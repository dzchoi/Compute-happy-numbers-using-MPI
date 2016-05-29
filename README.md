# Compute happy numbers using MPI

A number chain is created by continuously adding the square of the digits in a number to form a new number until it has been seen before.

For example,
- 44 → 32 → 13 → 10 → 1 → 1
- 85 → 89 → 145 → 42 → 20 → 4 → 16 → 37 → 58 → 89
Therefore any chain that arrives at 1 or 89 will become stuck in an endless loop. What is most amazing is that EVERY starting number will eventually arrive at 1 (called happy) or 89 (called unhappy).

So, How many numbers below ten million will be unhappy (arriving at 89)?
Answer is: 8,581,146

### Usage
- To compile: `mpicc -o unhappy -O2 -s unhappy.c` # `-O2` for optimization, `-s` for stripping
- To run: `mpiexec -n <NP> ./unhappy 10000000`

### References
- https://projecteuler.net/problem=92
- http://en.wikipedia.org/wiki/Happy_number

### Explanation
This program decomposes the domain (that is, the range of numbers, 1 .. 10,000,000) into NP subdomains (for example, if NP=4, each subdomain will have 2,500,000 numbers). Each MPI process computes how many unhappy numbers are there in its subdonamin and gives the count to the master MPI process using `MPI_Reduce()`. And, The master MPI process also participates in the computation for its own subdomain and combines all the results from other MPI processes with its own result using `MPI_Reduce()` to get the final result.

To simpify the computing algorithm for happy numbers, the function `sum_square()` uses a recursive call to itself. But, since this recursive call is tail-recursive and thanks to the compiler option `-O2` which performs the tail-call optimization, the recursive call is not compiled as the real recursive call, but as a simple jump.
