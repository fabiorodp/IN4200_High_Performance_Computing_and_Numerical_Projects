// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc create_SNN_graph2.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>


/*
Creating the corresponding Shared Nearest Neighbors SNN graph - version 2 -
Compressed Row Storage:

Inputs
~~~~~~~~~~~~~~~~~~~~~~~~
int N: Number of Nodes.
int *row_ptr:
int *col_idx:

Output
~~~~~~~~~~~~~~~~~~~~~~~~
int **SNN_val: 1D array containing the SNN's values, allocated inside the
function, in a CRS format.

Specifies:
~~~~~~~~~~~~~~~~~~~~~~~~
- An 2D array for SNN:
    |-> unsigned int **SNN_val:
        |-> (*SNN_val) = malloc(2*N_edges * sizeof SNN_val)

Example:
~~~~~~~~~~~~~~~~~~~~~~~~
For example,
an col_idx equals [ 1,2,3,   0,2,3,  0,1,3,4,    0,1,2,4,    2,3 ]
and row_ptr equals [ 0,3,6,10,14,16 ]
generates a SNN_val equals [ ]
*/
void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val)
{

}


int main(int argc, char const *argv[])
{
    // declaration for the CRS matrix
    int *col_idx;
    int *row_ptr;

    // declaration for the num. of nodes
    int N;

    // calling the function to read file and return row_ptr, col_idx and N
    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);

    // number of the edges of the example we will test on
    int N_edges = 8;

    // testing returned values for col_idx
    for ( size_t i = 0; i < 2*N_edges; i++)
        printf("%d", col_idx[i]);

    printf("\n");

    // testing returned values for row_ptr
    for ( size_t i = 0; i < N+1; i++)
        printf("%d", row_ptr[i]);

    printf("\n");

    // calling the function to return SNN_val
    create_SNN_graph2(N, *row_ptr, *col_idx, **SNN_val)

    free(col_idx);
    free(row_ptr);

    return 0;
}
