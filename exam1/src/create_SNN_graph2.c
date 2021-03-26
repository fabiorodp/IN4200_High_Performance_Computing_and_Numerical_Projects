// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf


/*
Creating the corresponding Shared Nearest Neighbors SNN graph - version 2 -
Compressed Row Storage:

Inputs
~~~~~~~~~~~~~~~~~~~~~~~~
int N: Number of Nodes.
int *row_ptr: 1D Array with the rows pointers.
int *col_idx: 1D Array with the columns indices.

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
an col_idx equals [ 1,2,3,    0,2,3,    0,1,3,4,    0,1,2,4,    2,3 ]
and row_ptr equals [ 0,3,6,10,14,16 ]

table2D_val equals [ 1,1,1,  1,1,1,  1,1,1,1,  1,1,1,1,  1,1 ]
SNN_val equals [ 2,2,2,  2,2,2,  2,2,3,1,  2,2,3,1,  1,1 ]
*/
void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val)
{
    // allocating SNN_val that has the same length of col_idx
    // row_ptr[N+1] is the length of col_idx
    (*SNN_val) = calloc(row_ptr[N+1], sizeof **SNN_val);

    // serialized loop construct to assign values in SNN_val
    size_t i, j, jj, ij, jjj;
    unsigned long count = 0;
    for ( i = 0; i < N; i++ )
        for ( j = row_ptr[i]; j < row_ptr[i + 1]; j++ )
            for ( jj = 0; jj < N; jj++ )
                if ( col_idx[j] == jj )  // if the nodes are connected
                {
                    for ( ij = row_ptr[i]; ij < row_ptr[i + 1]; ij++ )
                        for ( jjj = row_ptr[jj]; jjj < row_ptr[jj + 1]; jjj++ )
                            if ( col_idx[ij] == col_idx[jjj] )
                                (*SNN_val)[count] += 1;
                    count++;
                }
}
