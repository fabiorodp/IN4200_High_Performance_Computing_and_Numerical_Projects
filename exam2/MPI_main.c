// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// mpicc -o MPI_main MPI_main.c
// mpirun -np 2 ./MPI_main 6 5 3 2


#include <stdlib.h> // rand, malloc, calloc, realloc and free.
#include <stdio.h>  // printf, scanf
#include <mpi.h>
#include "MPI_double_layer_convolution.c"


int main(int argc, char *argv[]) 
{
    size_t u, i, j;
    int M=0, N=0, K1=0, K2=0, rank, size;
    float *input=NULL, *output=NULL, *kernel1=NULL, *kernel2=NULL;
    
    MPI_Init(&argc, &argv);                 // Initiating MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // # of processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // currently rank.

    if ( rank == 0 )
    {
        // read from command line the values of M, N, K1 and K2;
        M = atoi(argv[1]); N = atoi(argv[2]); 
        K1 = atoi(argv[3]); K2 = atoi(argv[4]);

        // allocate 2D array 'input' with M rows and N columns;
        input = malloc( M * N * sizeof *input );

        // allocate 2D array 'output' with M-K1-K2+2 rows and N-K1-K2+2 columns;
        output = malloc( (M-K1-K2+2) * (N-K1-K2+2) * sizeof *output );
        
        // allocate the convolutional kernel1 with K1 rows and K1 columns;
        kernel1 = malloc( K1 * K1 * sizeof *kernel1 );

        // allocate the convolutional kernel2 with K2 rows and K2 columns;
        kernel2 = malloc( K2 * K2 * sizeof *kernel2 );

        // fill 2D array 'input' with some values;
        int count = 0;
        for ( i = 0; i < M; i++ )
            for ( j=0; j < N; j++ )
                input[i*N + j] = ++count;

        // fill kernel1 with some values;
        int bi=0;
        for ( i = 0; i < K1; i++ )
            for ( j=0; j < K1; j++ )
                bi = kernel1[i*K1 + j] = (bi==0) ? 1 : 0;
        
        // fill kernel2 with some values;
        bi=1;
        for ( i = 0; i < K2; i++ )
            for ( j=0; j < K2; j++ )
                bi = kernel2[i*K2 + j] = (bi==0) ? 1 : 0;
    }

    // process 0 broadcasts values of M, N, K1, K2 to all the other processes
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K2, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if ( rank > 0 )
    {
        // allocate the convolutional kernel1 with K1 rows and K1 columns
        kernel1 = malloc( K1 * K1 * sizeof *kernel1 );
        // allocate the convolutional kernel2 with K2 rows and K2 columns
        kernel2 = malloc( K2 * K2 * sizeof *kernel2 );
    }

    // process 0 broadcasts the content of kernels to all the other processes
    MPI_Bcast(kernel1, K1*K1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(kernel2, K2*K2, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // parallel computation of a multi-layer convolution
    MPI_double_layer_convolution(M, N, input, K1, kernel1, K2, kernel2, &output);

    // if ( rank == 0 )
    // {
    //     // for example, compare the content of array 'output' with that is
    //     // produced by the sequential function double_layer_convolution
    //     float *seq_output1, *seq_output2;
    //     single_layer_convolution(M, N, input, K1, kernel1, &seq_output1);
    //     single_layer_convolution(M-K1+1, N-K1+1, seq_output1, K2, kernel2, &seq_output2);
        
    //     // freeing unnecessary array
    //     free(seq_output1);
        
    //     // computing if both techniques return equal values
    //     int is_equal=0;
    //     for ( u = 0; u < (M-K1-K2+2)*(N-K1-K2+2); u++ )
    //     {
    //         is_equal += seq_output2[u]==output[u] ? 0 : 1;
    //         printf("sequential_output[%lu]=%f and parallel_output[%lu]=%f\n", u, seq_output2[u], u, output[u]);
    //     }
        
    //     if ( is_equal==0 ) printf("The serialized and MPI paralelized outputs are equal.\n");
    //     else printf("The serialized and MPI paralelized outputs are NOT equal.\n");
        
    //     free(seq_output2);
    // }
    MPI_Finalize();
    return 0;
}
