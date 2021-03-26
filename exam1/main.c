// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc create_SNN_graph1.c  // without OpenMP
// clang -Xpreprocessor -fopenmp main.c -lomp  // with OpenMP
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <time.h>
#include "create_SNN_graph1.c"
#include "create_SNN_graph1_omp.c"
#include "create_SNN_graph2.c"
#include "create_SNN_graph2_omp.c"
#include <omp.h>


int main(int argc, char *argv[])
{
    // declaration for the num. of nodes
    int N;

    // declaration for the table2D
    char **table2D;

    // calling the function to read file and return table2D and N
    read_graph_from_file1(argv[1], &N, &table2D);

    // declaration for the SNN_table
    int **SNN_table;

    printf("Creating SNN graph 1 without OMP...\n");
    // calling the function to return SNN_table
    clock_t t0 = clock();
    create_SNN_graph1(N, table2D, &SNN_table);
    clock_t t1 = clock();

    double elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf("Elapsed time: %f s.\n", elapsed_time);
    printf("Created SNN graph 1 without OMP.\n");

    // declaration for the SNN_table
    int **SNN_table_omp;

    printf("\nCreating SNN graph 1 with OMP...\n");
    // calling the function to return SNN_table
    t0 = clock();
    create_SNN_graph1_omp(N, table2D, &SNN_table_omp);
    t1 = clock();

    elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf("Elapsed time: %f s.\n", elapsed_time);
    printf("Created SNN graph 1 with OMP.\n");

    // checking the returned array values
    printf("Printing SNN_table...\n");
    for (int i = 0; i < N; i++){
        for (int j=0; j < N; j++)
            printf("%d ", SNN_table[i][j]);
        printf("\n");
    }

    printf("Printing SNN_table_omp...\n");
    for (int i = 0; i < N; i++){
        for (int j=0; j < N; j++)
            printf("%d ", SNN_table_omp[i][j]);
        printf("\n");
    }

    // freeing memory
    free(*table2D);
    free(table2D);
    free(*SNN_table);
    free(SNN_table);
    free(*SNN_table_omp);
    free(SNN_table_omp);

    // declaration for the CRS matrix
    int *col_idx;
    int *row_ptr;

    // calling the function to read file and return N, row_ptr and col_idx
    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);

    // declaration for the SNN_val
    int *SNN_val;

    printf("\nCreating SNN graph 2 without OMP...\n");
    // calling the function to return SNN_val
    t0 = clock();
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);
    t1 = clock();

    elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf("Elapsed time: %f s.\n", elapsed_time);
    printf("Created SNN graph 2 without OMP..\n");

    // declaration for the SNN_val
    int *SNN_val_omp;

    printf("\nCreating SNN graph 2 with OMP...\n");
    // calling the function to return SNN_val
    t0 = clock();
    create_SNN_graph2_omp(N, row_ptr, col_idx, &SNN_val_omp);
    t1 = clock();

    elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf("Elapsed time: %f s.\n", elapsed_time);
    printf("Created SNN graph 2 with OMP..\n");

    // testing returned values for SNN_val
    printf("Printing SNN_val...\n");
    for ( size_t i = 0; i < row_ptr[N+1]; i++)
        printf("%d ", SNN_val[i]);

    // testing returned values for SNN_val
    printf("\nPrinting SNN_val_omp...\n");
    for ( size_t i = 0; i < row_ptr[N+1]; i++)
        printf("%d ", SNN_val_omp[i]);

    free(col_idx);
    free(row_ptr);
    free(SNN_val);

    return 0;
}
