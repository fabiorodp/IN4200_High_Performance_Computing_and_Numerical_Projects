// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


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
    (*SNN_val) = calloc(row_ptr[N], sizeof **SNN_val);

    // global variables
    size_t z, x, i, j, row_nr;
    unsigned long count;

    // loop construct to assign values for (*SNN_val)
    for ( z = 0; z < row_ptr[N]; z++ )
    {
        count = 0;

        for ( x = 0; x < N+1; x++ )  // getting row
        {
            if ( z < row_ptr[x] )
            {
                row_nr=x-1;
                break;
            }
        }

        for ( i = row_ptr[row_nr]; i < row_ptr[row_nr + 1]; i++ ) // in element/row
        {
            for ( j = row_ptr[col_idx[z]]; j < row_ptr[col_idx[z] + 1]; j++ )  // in element/col
                if ( col_idx[i] == col_idx[j] )
                    count += 1;
        }

        (*SNN_val)[z] = count;
    }
}

int main(int argc, char *argv[])
{
    // declaration for the num. of nodes, CRS matrices and SNN_val
    int N, *col_idx, *row_ptr, *SNN_val;

    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);

    for (size_t i = 0; i < row_ptr[N]; i++)
        printf("%d", col_idx[i]);

    printf("\n");

    for (size_t i = 0; i < N + 1; i++)
        printf("%d", row_ptr[i]);

    printf("\n");

    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);

    for (size_t i = 0; i < row_ptr[N]; i++)
        printf("%d", SNN_val[i]);

    free(SNN_val);
    free(row_ptr);
    free(col_idx);
}
