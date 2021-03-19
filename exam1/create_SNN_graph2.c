// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc create_SNN_graph2.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>
#include "read_graph_from_file2.c"


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
an col_idx equals [ 1,2,3,    0,2,3,    0,1,3,4,    0,1,2,4,    2,3 ]
and row_ptr equals [ 0,3,6,10,14,16 ]

table2D_val equals [ 1,1,1,  1,1,1,  1,1,1,1,  1,1,1,1,  1,1 ]
SNN_val equals [ 2,2,2,  2,2,2,  2,2,3,1,  2,2,3,1,  1,1 ]
*/
void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val)
{
    // allocating SNN_val
    // row_ptr[N+1] is the length of col_idx
    (*SNN_val) = calloc(row_ptr[N+1], sizeof **SNN_val);

    // sparse to dense matrix


    for ( size_t e = 0; e < row_ptr[N+1]; e++ )
    {
        for ( size_t i = 1; i < N+1; i++ )  // getting 1st batch
        {
            size_t start_b1 = row_ptr[i-1];
            size_t end_b1 = row_ptr[i];
            // printf("\n %lu %lu\n", start, end);  // -> ok

            for ( size_t j = start_b1; j < end_b1; j++)
            {

                int row_idx = 0;
                for ( size_t ii = 1; ii < N+1; ii++ )  // getting 2nd batch
                {
                    if ( col_idx[j] == row_idx )
                    {
                        size_t start_b2 = row_ptr[ii-1];
                        size_t end_b2 = row_ptr[ii];

                        for ( size_t jj = start_b2; jj < end_b2; jj++)
                        {
                            if ( col_idx[j] == col_idx[jj] )
                            {
                                (*SNN_val)[e] += 1;
                            }
                        }
                    }
                    row_idx += 1;
                }
            }
        }
    }

}


int main(int argc, char *argv[])
{
    // declaration for the CRS matrix
    int *col_idx;
    int *row_ptr;

    // declaration for the num. of nodes
    int N;

    // calling the function to read file and return N, row_ptr and col_idx
    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);

    // declaration for the SNN_val
    int *SNN_val;

    // calling the function to return SNN_val
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);

    // testing returned values for SNN_val
    // row_ptr[N+1] is the length of col_idx
    for ( size_t i = 0; i < row_ptr[N+1]; i++)
        printf("%d", SNN_val[i]);

    free(col_idx);
    free(row_ptr);
    free(SNN_val);

    return 0;
}
