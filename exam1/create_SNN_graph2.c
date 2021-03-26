// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc create_SNN_graph2.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include "read_graph_from_file2.c"


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


//int main(int argc, char *argv[])
//{
//    // declaration for the CRS matrix
//    int *col_idx;
//    int *row_ptr;
//
//    // declaration for the num. of nodes
//    int N;
//
//    // calling the function to read file and return N, row_ptr and col_idx
//    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);
//
//    // declaration for the SNN_val
//    int *SNN_val;
//
//    // calling the function to return SNN_val
//    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);
//
//    // testing returned values for SNN_val
//    // row_ptr[N+1] is the length of col_idx
//    for ( size_t i = 0; i < row_ptr[N+1]; i++)
//        printf("%d", SNN_val[i]);
//
//    free(col_idx);
//    free(row_ptr);
//    free(SNN_val);
//
//    return 0;
//}
