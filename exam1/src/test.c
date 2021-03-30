// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include "read_graph_from_file2.c"


void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx)
{
    FILE *file;

    // opening file
    file = fopen(filename, "r");

    fscanf(file, "%*[^\n]\n"); // skip one line
    fscanf(file, "%*[^\n]\n"); // skip one line

    fscanf(infile, "%*s %*s %d %*s %d \n", N, N_links);
    int *to_arr = malloc(*N_links * sizeof(int));
    int *from_arr = malloc(*N_links * sizeof(int));
    *col_idx = malloc(*N_links * sizeof(int));
    *row_ptr = malloc((*N + 1) * sizeof(int));
    (*row_ptr)[0] = 0; // First row starts on index zero

    fscanf(infile, "%*[^\n]\n"); // skip one line
    c = 0;

    // Reading the file and storing row_ptr values as
    // Also saving to and from values for making col_idx
    while (fscanf(infile, "%d %d\n", &from, &to) != EOF) {
        if(from != to) {
            (*row_ptr)[to + 1]++; // One more element linking to given web page
            to_arr[c]     = to;
            from_arr[c]   = from;
            c++;
        }
    }

    col_idx_index = 0;
    for (int i=0; i<*N; i++) {
        for (int j=0; j< *N_links; j++){
            if (to_arr[j] == i) {
                (*col_idx)[col_idx_index] = from_arr[j];
                col_idx_index++;
            }
        }
    }
    for (int i = 1; i < (*N+1); i++) {
        (*row_ptr)[i] += (*row_ptr)[i-1];
    }
    free(to_arr);
    free(from_arr);
}

int main(int argc, char *argv[])
{
    // declaration for the num. of nodes, CRS matrices and SNN_val
    int N, *col_idx, *row_ptr, *SNN_val;

    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);

    for ( size_t i=0; i < row_ptr[N+1]; i++ )
        printf("%d", col_idx[i]);

//    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);
//
//    for ( size_t i=0; i < row_ptr[N+1]; i++ )
//        printf("%d", SNN_val[i]);

    free(col_idx);
    free(row_ptr);
    free(SNN_val);

    return 0;
}
