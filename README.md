# Compute happy numbers using MPI

A happy number is a number defined by the following process: Starting with any positive integer, replace the number by the sum of the squares of its digits, and repeat the process until the number either equals 1 (where it will stay), or it loops endlessly in a cycle which does not include 1. Those numbers for which this process ends in 1 are happy numbers, while those that do not end in 1 are unhappy numbers (or sad numbers).

For example,
- 44 → 32 → 13 → 10 → 1 → 1 → ... (happy)
- 85 → 89 → 145 → 42 → 20 → 4 → 16 → 37 → 58 → 89 → ... (unhappy)

It is proven for all positive integers that:
- numbers that are happy, follow a sequence that ends in 1, and
- all non-happy numbers follow sequences that reach the cycle: 4, 16, 37, 58, 89, 145, 42, 20, 4, ...

Therefore, we can tell the happiness of a number by checking if its chain of process arrives at either 1 or 4.

So, How many numbers below ten million will be unhappy (arriving at 89)?  
Answer is: 8,581,146

### Usage
- To compile: `mpicc -o unhappy -O2 -s unhappy.c` # `-O2` for optimization, `-s` for stripping
- To run: `mpiexec -n <NP> ./unhappy 10000000`

### References
- https://projecteuler.net/problem=92
- http://en.wikipedia.org/wiki/Happy_number

### Explanation
This program decomposes the domain (that is, the range of numbers, 1 .. 10,000,000) into NP subdomains (for example, if NP=4, each subdomain will have 2,500,000 numbers). Each MPI process computes how many unhappy numbers are there in its subdonamin and gives the count to the master MPI process using `MPI_Reduce()`. And, The master MPI process also participates in the computation for its own subdomain (with `MPI_IN_PLACE` as the first argument of `MPI_Reduce()`) and combines all the results from other MPI processes with its own result using `MPI_Reduce()` to get the final result.

To simpify the computing algorithm for happy numbers, the function `sum_square()` uses a recursive call to itself. But, since this recursive call is tail-recursive and thanks to the compiler option `-O2` which performs the tail-call optimization, the recursive call is not compiled as the real recursive call, but as a simple jump.

If we apply the optimization technique of memoization (https://en.wikipedia.org/wiki/Memoization), we could solve this problem without resorting to the parallel computing and might have the same or even better performance in speed. This is for a demonstration of MPI programming!
