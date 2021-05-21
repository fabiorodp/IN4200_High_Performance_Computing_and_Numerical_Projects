// compiling & running
// mpicc -o answer.c
// mpirun -np 2 ./a.out

#include <stdlib.h> // rand, malloc, calloc, realloc and free.
#include <stdio.h>  // printf, scanf
#include <mpi.h>
#include <math.h>
#include <omp.h>
#include "string.h"
#include <mpi.h>

/*
EXERCISE 9:
*/
int parallel_count_occurrence(const char *text_string, const char *pattern)
{
    int i, count=0;
    size_t lenStr1 = strlen(text_string);
    size_t lenStr2 = strlen(pattern);

#pragma omp parallel for reduction(+ : count)
    for ( i = 0; i <= (lenStr1-lenStr2); i++ )
        if ( strncmp(&text_string[i], pattern, lenStr2) == 0 )
            count++;

    return count;
}

int main( int argc, char **argv ) {

    // last exercise
    int u, i, j;
    int M = 0, N = 0, K1 = 0, K2 = 0, rank, size;
    float *input = NULL, *output = NULL, *kernel1 = NULL, *kernel2 = NULL;
    double t0, t1, t2, t3;

    MPI_Init(&argc, &argv);                 // Initiating MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // # of processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // currently rank.

    if (rank == 0) {
        const char text_string[] = "ATTTGCGCAGACCTAAGCA";
        const char pattern[] = "GCA";
    }
}
