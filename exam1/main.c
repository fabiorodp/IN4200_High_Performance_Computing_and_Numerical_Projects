// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc main.c
// ./a.out


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>
#include "read_graph_from_file2.c"

/*
File to test all the functions created in this exam 1.
 */


int main(int argc, char *argv[]){

    int *col_idx;
    int *row_ptr;
    int N;

    // calling function
    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);

    // testing returned values
    for ( int i = 0; i < 2*(int)sizeof(col_idx); i++)
        printf("%d", col_idx[i]);

    printf("\n");

    for ( int i = 0; i < N+1; i++)
        printf("%d", row_ptr[i]);

    printf("\n");

    free(col_idx);
    free(row_ptr);

    return 0;
}
