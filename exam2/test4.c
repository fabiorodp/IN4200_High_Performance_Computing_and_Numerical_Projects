// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc test3.c
// ./a.out


#include <stdio.h>
#include <stdlib.h>


#define NUM_OF_RANKS 4
#define maxNeededRanks 6


int main( int argc, char **argv )
{
    int M=6, N=5, K1=3, K2=2;
    int ndims, order; 
    
    // output row and col sizes
    int M_out = M - K1 - K2 + 2;  // 3
    int N_out = N - K1 - K2 + 2;  // 2

    // MPI_Type_create_subarray
    if ( rank == 0)
    {
        int matrix[6][5] = {
            {1,2,3,4,5},
            {6,7,8,9,10},
            {11,12,13,14,15},
            {16,17,18,19,20},
            {21,22,23,24,25},
            {26,27,28,29,30}
        };
   
        for ( int receiver = 1; receiver < NUM_OF_RANKS; receiver++ )
        {
            ndims=2;
            int array_of_sizes[2]={M,N};
            int array_of_subsizes[2]={M_out,N_out};
            int array_of_starts[2];
            order = MPI_ORDER_C;
            MPI_Datatype oldtype = MPI_INT;
            MPI_Datatype *mysubarray;

            MPI_Type_create_subarray(
                ndims,              // num of dimensions
                array_of_sizes,     // sizes of buffer's matrix 
                array_of_subsizes,  // sizes of subarray
                array_of_starts,    // start value
                order,              // MPI_ORDER_C
                oldtype,            // MPI_INT
                &mysubarray         // MPI_Datatype *mysubarray
            );

            MPI_Type_commit(&mysubarray);

            MPI_Send(
                &(matrix[0][0]),    // buffer
                1,                  // qtd of buffers
                mysubarray,         // MPI_Datatype *subarray
                receiver,           // receiver's rank
                receiver,           // tag
                MPI_COMM_WORLD      // communicator
            );

            MPI_Type_free(&mysubarray);
            free(matrix[0]);
            free(matrix);
        }
    } 
    
    else if ( rank > 0 )
    {
        int *subarray = malloc( M_out * N_out * sizeof *subarray);
        
        // receiving subarray
        MPI_Recv(
            &(subarray[0][0]),
            subsize*subsize,
            MPI_INT,
            sender,
            ourtag,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
    };

    return 0;
}