// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <omp.h>


void create_SNN_graph2_omp(int N, int *row_ptr, int *col_idx, int **SNN_val)
{
    // allocating SNN_val that has the same length of col_idx
    (*SNN_val) = calloc(row_ptr[N + 1], sizeof **SNN_val);

    // row_ptr equals [ 0, 3, 6, 10, 14, 16 ]
    // col_idx equals [ 1,2,3,    0,2,3,    0,1,3,4,    0,1,2,4,    2,3 ]
    // SNN_val equals [ 2,2,2,    2,2,2,    2,2,3,1,    2,2,3,1,    1,1 ]

    size_t k, i, j;
    for (k = 0; k < N; k++)
    {
        int len = row_ptr[k+1]-row_ptr[k];

        if (len>1)
            for (j = row_ptr[k]; j < row_ptr[k+1]; j++)
                for (i = row_ptr[col_idx[i]]; i < row_ptr[col_idx[i]]+len; i++)
                    (*SNN_val)[col_idx[i]] += 1;
    }
}
