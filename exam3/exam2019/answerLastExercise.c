// compiling & running
// mpicc -o MPI_main answerLastExercise.c
// mpirun -np 2 ./MPI_main

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

    for ( i = 0; i <= (lenStr1-lenStr2); i++ )
        if ( strncmp(&text_string[i], pattern, lenStr2) == 0 )
            count++;

    return count;
}

int main( int argc, char **argv ) {

    // last exercise
    int rank, num_procs;
    char *text_string = NULL, *pattern = NULL;
    size_t n1=0, n2=0;

    MPI_Init(&argc, &argv);                      // Initiating MPI
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);   // # of processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);        // currently rank.

    if ( rank == 0 )
    {
        text_string = "ATTTGCGCAGACCTAAGCA";
        pattern = "GCA";

        n1 = strlen(text_string);
        n2 = strlen(pattern);
    }

    MPI_Bcast(&n1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n2, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if ( rank > 0 )
        pattern = malloc( n2 * sizeof *pattern );

    MPI_Bcast(pattern, n2, MPI_CHAR, 0, MPI_COMM_WORLD);
    // printf("rank=%d     len1=%lu     len2=%lu    patter=%s\n", rank, n1, n2, pattern);

    int *sendcounts, *displs;
    sendcounts = (int*)malloc(num_procs*sizeof(int));
    displs = (int*)malloc(num_procs*sizeof(int));

    for ( int i=0; i<num_procs; i++)
        displs[i] = ( i*(n1-n2+1) ) / num_procs;

    for ( int i=0; i<num_procs-1; i++)
        sendcounts[i] = ( displs[i+1]-displs[i] ) + n2-1;

    sendcounts[num_procs-1] = n1-displs[num_procs-1];

    char *myText_string;
    myText_string = malloc( sendcounts[rank] * sizeof *myText_string );

    // Scatter A and x.
    MPI_Scatterv(
        text_string,
        sendcounts,
        displs,
        MPI_CHAR,
        myText_string,
        sendcounts[rank],
        MPI_CHAR,
        0,
        MPI_COMM_WORLD
    );

    int myCount;
    myCount = parallel_count_occurrence(myText_string, pattern);
    // printf("rank=%d     myText_string=%s     myCount=%d\n", rank, myText_string, myCount);

/* ########################## VERSION 1 - begin ########################## */

    int *out, globalCount_V1;
    out = (int*)malloc(num_procs*sizeof(int));

    MPI_Gather(
        &myCount,
        1,
        MPI_INT,
        out,
        1,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    if ( rank == 0 )
    {
        for ( int i = 0; i < num_procs; i++ )
        {
            globalCount_V1+=out[i];
            // printf("VERSION 1 >> out[%d]=%d\n", i, out[i]);
        }

        printf("VERSION 1 >> globalCount_V1=%d\n", globalCount_V1);
    }

/* ########################## VERSION 1 - end ########################## */

/* ########################## VERSION 2 - begin ########################## */

    int globalCount_V2;

    MPI_Reduce(
        &myCount,
        &globalCount_V2,
        myCount,
        MPI_INT,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    if ( rank == 0 )
        printf("VERSION 2 >> globalCount_V2=%d\n", globalCount_V2);

/* ########################## VERSION 2 - end ########################## */

    free(myText_string);
    free(out);
    MPI_Finalize();
}
