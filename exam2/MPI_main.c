// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// mpicc -o hello helloWorld.c
// mpirun -np 1 ./hello


#include <stdlib.h> // rand, malloc, calloc, realloc and free.
#include <stdio.h>  // printf, scanf
#include <time.h>  // clock
#include <mpi.h>
#include "MPI_double_layer_convolution.c"

//  5x5 kernel:
// [[1 1 1 1 1][1 1 1 1 1][0 0 0 0 0][-1 -1 -1 -1 -1][-1 -1 -1 -1 -1]]

/*
Arguments:
~~~~~~~~~~~~~~~~~~~~~
*/
int main(int argc, char *argv[]) 
{
    int M=0, N=0, K1=0, K2=0, my_rank, size;
    float **input=NULL, **output=NULL, **kernel1=NULL, **kernel2=NULL;
    
    MPI_Init(&argc, &argv);  // Initiating MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // # of processors.
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);  // currently rank.

    if ( my_rank == 0 )
    {
    // read from command line the values of M, N, K1 and K2;
    // allocate 2D array 'input' with M rows and N columns;
    // allocate 2D array 'output' with M-K1-K2+2 rows and N-K1-K2+2 columns;
    // allocate the convolutional kernel with K1 rows and K1 columns;
    // allocate the convolutional kernel with K2 rows and K2 columns;
    // fill 2D array 'input' with some values;
    // fill kernel1 with some values;
    // fill kernel2 with some values;
    }

    // process 0 broadcasts values of M, N, K1, K2 to all the other processes
    // ...

    if ( my_rank > 0 )
    {
        // allocate the convolutional kernel1 with K1 rows and K1 columns
        // allocate the convolutional kernel2 with K2 rows and K2 columns
    }

    // process 0 broadcasts the content of kernels to all the other processes
    // ...

    // parallel computation of a multi-layer convolution
    // MPI_double_layer_convolution(M, N, input, K1, kernel1, K2, kernel2, output);

    if ( my_rank > 0 )
    {
        // for example, compare the content of array 'output' with that is
        // produced by the sequential function double_layer_convolution
    }

    MPI_Finalize();
    return 0;
}
