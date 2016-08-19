/*
References:
- https://projecteuler.net/problem=92
- http://en.wikipedia.org/wiki/Happy_number

A number chain is created by continuously adding the square of the digits in a number to form a new number until it has been seen before.
For example,
- 44 → 32 → 13 → 10 → 1 → 1
- 85 → 89 → 145 → 42 → 20 → 4 → 16 → 37 → 58 → 89
Therefore any chain that arrives at 1 or 89 will become stuck in an endless loop. What is most amazing is that EVERY starting number will eventually arrive at 1 (called happy) or 89 (called unhappy).

How many starting numbers below ten million will arrive at 89?
Answer is: 8,581,146

To compile: mpicc -o unhappy -O2 -s unhappy.c # -O2 for optimization, -s for stripping
To run: mpiexec -n <NP> ./unhappy
*/

#include <stdlib.h> // atoi()
#include <mpi.h>

inline unsigned square(unsigned n) { return ( n * n ); }

unsigned sum_squares(unsigned n)
{
    switch ( n ) {
	case 0u :
	case 1u : // it's a happy number
	case 89u :
	    return ( n );

	default : {
	    unsigned sum = 0;

	    do {
		sum += square(n % 10u);
		n /= 10u;
	    } while ( n > 0 );

	    return ( sum_squares(sum) ); // tail recursive!
	}
    }
}

typedef enum { false, true } bool;
inline bool is_happy(unsigned n) { return ( sum_squares(n) == 1u ); }

int main(int argc, char *argv[])
{
    int np; // # of all the processes
    int rank; // process ID
    unsigned up = 10000000; // ten million by default
    unsigned n;  // runs from 1 to up

    int count = 0; // # of unhappy numbers for each MPI process

    if ( argv[1] ) {
	up = atoi(argv[1]); // read in from argv[1] if any
	if ( up <= 0 )
	    printf("n > 0 required\n");
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // All MPI tasks including the master compute the happiness.
    for ( n = 0 ; n++ < up ; )
    //for ( n = 1 ; n <= up ; n++ ) // alternatively
	if ( n % np == rank && !is_happy(n) )
	    count++;

    if ( rank == 0 ) {
	MPI_Reduce(MPI_IN_PLACE, &count, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);
	    // With MPI_IN_PLACE, the input data is taken at the root from the receive buffer, where it will
	    // be replaced by the output data.
	printf("number of unhappy numbers in 1..%u = %u\n", up, count);
    }
    else
	MPI_Reduce(&count, &count, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);
	    // The second &count for receive buffer is not significant here since we are not at root process.

    MPI_Finalize();
    return ( 0 );
}
