// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <omp.h>
//#include "read_graph_from_file2.c"


void create_SNN_graph2_omp(int N, int *row_ptr, int *col_idx, int **SNN_val)
{
    // allocating SNN_val that has the same length of col_idx
    (*SNN_val) = calloc(row_ptr[N], sizeof **SNN_val);

    // global variables are shared by default in the parallel region
    size_t z, x, i, j, row_nr;
    unsigned long count;

#pragma omp parallel for private(z, x, i, j, row_nr) reduction(+:count)
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

//int main(int argc, char *argv[])
//{
//    // declaration for the num. of nodes, CRS matrices and SNN_val
//    int N, *col_idx, *row_ptr, *SNN_val;
//
//    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);
//
//    for (size_t i = 0; i < row_ptr[N]; i++)
//        printf("%d", col_idx[i]);
//
//    printf("\n");
//
//    for (size_t i = 0; i < N + 1; i++)
//        printf("%d", row_ptr[i]);
//
//    printf("\n");
//
//    create_SNN_graph2_omp(N, row_ptr, col_idx, &SNN_val);
//
//    for (size_t i = 0; i < row_ptr[N]; i++)
//        printf("%d", SNN_val[i]);
//
//    free(SNN_val);
//    free(row_ptr);
//    free(col_idx);
//}
