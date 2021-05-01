// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// mpicc -o hello helloWorld.c
// mpirun -np 1 ./hello


#include <stdlib.h> // rand, malloc, calloc, realloc and free.
#include <stdio.h>  // printf, scanf
#include <time.h>  // clock
#include <mpi.h>
#include "helper.h"


#define idx(i,j) (i*N + j)  // to index a 2D matrix in 1D style
// Used to index local copies of the data in matrix A.
#define idx2(i,j) (i*n_cols[myrank] + j)

//  5x5 kernel:
// [[1 1 1 1 1][1 1 1 1 1][0 0 0 0 0][-1 -1 -1 -1 -1][-1 -1 -1 -1 -1]]

int main(int argc, char *argv[]) 
{
    MPI_Init(&argc, &argv);  // Initiating MPI

    int size, currently_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // the number of processors.
    MPI_Comm_rank(MPI_COMM_WORLD, &currently_rank);  // the rank of this instance.

    // doing something in parallel.


    // assuming that N is not necessarily divisible by the number of processes
    int N;
    if (currently_rank == 0) {
        if (argc>1) {
            N = atoi(argv[1]);
        } else {
            N = size*3 + 1;
        }
        printf("N = %d\n", N);
    }

    // |> use MPI_Scatterv because we assume that N is not divisible by the
    // number of processes, then the processes might get different number of
    // elements of the A matrix to work with. In MPI_Scatter, each process gets 
    // its own part of the A matrix.
    // vide w10_e3_matvec.c

    // passing mensages
    MPI_Status msg_status;
    if (currently_rank == 0) 
    {

    } 
    
    else 
    {
        MPI_Send(msg, msglen, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();  // Finalizing MPI
    return 0;
}
