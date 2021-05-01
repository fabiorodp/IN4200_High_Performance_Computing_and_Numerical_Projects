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


void print2Darray(int row_len, int col_len)
{
    printf("\n");
    for (int i=0; i<row_len; i++)
    {
        for (int j=0; j<col_len; j++)
            printf("%f ", input[i*col_len + j]);
        
        printf("\n");
    }
}


//  5x5 kernel:
// [[1 1 1 1 1][1 1 1 1 1][0 0 0 0 0][-1 -1 -1 -1 -1][-1 -1 -1 -1 -1]]

/*
Arguments:
~~~~~~~~~~~~~~~~~~~~~
M for # of rows
N for # of columns

*/
int main(int argc, char *argv[]) 
{
    int M=0, N=0, K1=0, K2=0, rank, size;
    float *input=NULL, *output=NULL, *kernel1=NULL, *kernel2=NULL;
    
    MPI_Init(&argc, &argv);  // Initiating MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // # of processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // currently rank.

    if ( rank == 0 )
    {
    // read from command line the values of M, N, K1 and K2;
    M = atoi(argv[1]); N = atoi(argv[2]); 
    K1 = atoi(argv[3]); K2 = atoi(argv[4]);

    if (size > N) 
    {
        printf("Error: N has to be larger than the amount of MPI processes!");
        MPI_Finalize();
        return 1;
    }

    // allocate 2D array 'input' with M rows and N columns;
    *input = malloc( M * N * sizeof *input );

    // allocate 2D array 'output' with M-K1-K2+2 rows and N-K1-K2+2 columns;
    *output = malloc( (M-K1-K2+2) * (N-K1-K2+2) * sizeof *output );
    
    // allocate the convolutional kernel1 with K1 rows and K1 columns;
    *kernel1 = malloc( K1 * K1 * sizeof *input );

    // allocate the convolutional kernel2 with K2 rows and K2 columns;
    *kernel2 = malloc( K2 * K2 * sizeof *input );

    // fill 2D array 'input' with some values;
    for ( int i = 0; i < M; i++ )
        for ( int j=0; j < N; j++ )
            input[i*N + j] = ++count;
    
    // print2Darray(M, N);

    // fill kernel1 with some values;
    int bi=0;
    float *kernel1 = malloc(K1 * K1 * sizeof *input);
    for ( int i = 0; i < K1; i++ )
        for ( int j=0; j < K1; j++ )
            bi = kernel1[i*K1 + j] = (bi==0) ? 1 : 0;
    
    // print2Darray(K1, K1);
    
    // fill kernel2 with some values;
    bi=1;
    float *kernel2 = malloc(K2 * K2 * sizeof *input);
    for ( int i = 0; i < K2; i++ )
        for ( int j=0; j < K2; j++ )
            bi = kernel2[i*K2 + j] = (bi==0) ? 1 : 0;
    }

    // print2Darray(K2, K2);

    // process 0 broadcasts values of M, N, K1, K2 to all the other processes
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K2, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if ( myRank > 0 )
    {
        // allocate the convolutional kernel1 with K1 rows and K1 columns
        *kernel1 = malloc( K1 * K1 * sizeof *input );
        // allocate the convolutional kernel2 with K2 rows and K2 columns
        *kernel2 = malloc( K2 * K2 * sizeof *input );
    }

    // process 0 broadcasts the content of kernels to all the other processes
    MPI_Bcast(&kernel1, (K1*K1), MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&kernel2, (K2*K2), MPI_FLOAT, 0, MPI_COMM_WORLD);

    // parallel computation of a multi-layer convolution
    MPI_double_layer_convolution(M, N, input, K1, kernel1, K2, kernel2, output);

    if ( rank == 0 )
    {
        // for example, compare the content of array 'output' with that is
        // produced by the sequential function double_layer_convolution
        float *seq_output1, *seq_output2;
        single_layer_convolution(M, N, input, K1, kernel1, &seq_output1);
        single_layer_convolution(M-K1+1, N-K1+1, output1, K2, 
                                 kernel2, &seq_output2);
        
        int is_equal;
        for ( int i = 0; i < (M-K1-K2+2); i++ )
            for ( int j = 0; j < (N-K1-K2+2); j++ )
                is_equal = ( seq_output2[i*(M-K1-K2+2) +j]==output[i*(M-K1-K2+2) +j] ) ? 1 : 0;
        
        if ( is_equal==1 )
            printf("The serialized and MPI paralelized outputs are equal.")
        else
            printf("The serialized and MPI paralelized outputs are NOT equal.")
        
        free(seq_output1)
        free(seq_output2)
    }

    MPI_Finalize();
    return 0;
}
