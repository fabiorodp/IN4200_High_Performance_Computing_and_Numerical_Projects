// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc create_SNN_graph2.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>
#include <omp.h>
//#include "read_graph_from_file2.c"

void create_SNN_graph2_omp(int N, int *row_ptr, int *col_idx, int **SNN_val)
{
    // allocating SNN_val that has the same length of col_idx
    (*SNN_val) = calloc(row_ptr[N + 1], sizeof **SNN_val);

    // col_idx equals [ 1,2,3,    0,2,3,    0,1,3,4,    0,1,2,4,    2,3 ]
    // row_ptr equals [ 0,3,6,10,14,16 ]
    // SNN_val equals [ 2,2,2,  2,2,2,  2,2,3,1,  2,2,3,1,  1,1 ]

    for (size_t k=0; k<N; k++)
    {
        int length = row_ptr[k+1]-row_ptr[k];

        if (length>1)
        {
            for (size_t i=row_ptr[k]; i<row_ptr[k+1]; i++)
                (*SNN_val)[col_idx[i]] += (length-1);
        }
    }
}
