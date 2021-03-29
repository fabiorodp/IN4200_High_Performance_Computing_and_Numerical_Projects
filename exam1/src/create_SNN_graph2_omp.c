// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <omp.h>


void create_SNN_graph2_omp(int N, int *row_ptr, int *col_idx, int **SNN_val)
{
    // allocating SNN_val that has the same length of col_idx
    (*SNN_val) = calloc(row_ptr[N + 1], sizeof **SNN_val);

    // global variables are shared by default in the parallel region
    size_t z, x, i, j, row_nr;
    unsigned long count;

    #pragma omp parallel for private(z, x, i, j, row_nr) reduction(+:count)
    for ( z = 0; z < row_ptr[N + 1]; z++ )
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
