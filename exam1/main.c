// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc -fopenmp main.c main.h
// clang -Xpreprocessor -fopenmp main.c main.h -lomp
// ./a.out


#include <stdlib.h> // rand, malloc, calloc, realloc and free.
#include <stdio.h>  // printf, scanf
#include <time.h>  // clock
#include "main.h"
#include "src/read_graph_from_file1.c"
#include "src/read_graph_from_file2.c"
#include "src/create_SNN_graph1.c"
#include "src/create_SNN_graph1_omp.c"
#include "src/create_SNN_graph2.c"
#include "src/create_SNN_graph2_omp.c"
#include "src/check_node.c"


void test_read_and_create_SNN_graph1(char *filename)
{
    // declaration for the num. of nodes
    int N;

    // declaration for the table2D
    char **table2D;

    // declaration for the SNN_table
    int **SNN_table;

    // calling the function to read file and return table2D and N
    read_graph_from_file1(filename, &N, &table2D);

    printf("Creating SNN graph 1 without OMP...\n");
    // calling the function to return SNN_table
    clock_t t0 = clock();
    create_SNN_graph1(N, table2D, &SNN_table);
    clock_t t1 = clock();

    double elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf(">> Elapsed time: %f s.\n", elapsed_time);

    // declaration for the SNN_table_omp
    int **SNN_table_omp;

    printf("Creating SNN graph 1 with OMP...\n");
    // calling the function to return SNN_table
    t0 = clock();
    create_SNN_graph1_omp(N, table2D, &SNN_table_omp);
    t1 = clock();

    double elapsed_time1 = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf(">> Elapsed time: %f s.\n", elapsed_time1);

    // printing speedup
    printf("Speedup (SNN_table_omp/SNN_table):\n>> %f\n",
           elapsed_time1/elapsed_time);

    // comparing if SNN_table and SNN_table_omp are equals
    printf("Comparing if SNN_table and SNN_table_omp are equals:\n");
    comparing_2D_tables(SNN_table, SNN_table_omp, N);

    // checking if the user want to print SNN_table
    printing_SNN_table(SNN_table, N);

    // freeing memory
    free(*table2D);
    free(table2D);
    free(*SNN_table);
    free(SNN_table);
    free(*SNN_table_omp);
    free(SNN_table_omp);
}


void test_read_and_create_SNN_graph2(char *filename)
{
    // declaration for the num. of nodes
    int N;

    // declaration for the CRS matrix
    int *col_idx;
    int *row_ptr;

    // calling the function to read file and return N, row_ptr and col_idx
    read_graph_from_file2(filename, &N, &row_ptr, &col_idx);

    // declaration for the SNN_val
    int *SNN_val;

    printf("Creating SNN graph 2 without OMP...\n");
    // calling the function to return SNN_val
    clock_t t0 = clock();
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);
    clock_t t1 = clock();

    double elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf(">> Elapsed time: %f s.\n", elapsed_time);

    // declaration for the SNN_val
    int *SNN_val_omp;

    printf("Creating SNN graph 2 with OMP...\n");
    // calling the function to return SNN_val
    t0 = clock();
    create_SNN_graph2_omp(N, row_ptr, col_idx, &SNN_val_omp);
    t1 = clock();

    double elapsed_time1 = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf(">> Elapsed time: %f s.\n", elapsed_time1);

    // printing speedup
    printf("Speedup (SNN_val_omp/SNN_val):\n>> %f\n",
           elapsed_time1/elapsed_time);

    // Comparing if SNN_val and SNN_val_omp are equals
    printf("Comparing if SNN_val and SNN_val_omp are equals:\n");
    comparing_SNN_val(SNN_val, SNN_val_omp, row_ptr[N+1]);

    // checking if the user want to print SNN_val
    printing_SNN_val(SNN_val, row_ptr[N+1]);

    free(col_idx);
    free(row_ptr);
    free(SNN_val);
    free(SNN_val_omp);
}


void test_check_node(char *filename)
{
    // declaration for the num. of nodes
    int N;

    // declaration for the CRS matrix
    int *col_idx;
    int *row_ptr;

    // calling the function to read file and return N, row_ptr and col_idx
    read_graph_from_file2(filename, &N, &row_ptr, &col_idx);

    // declaration for the SNN_val
    int *SNN_val;

    // calling the function to return SNN_val
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);

    // declaration for node_id
    int node_id = 4;

    // declaration for tau
    int tau = 3;

    // calling the function to print the nodes that are in the cluster
    check_node(node_id, tau, N, row_ptr, col_idx, SNN_val);

    free(col_idx);
    free(row_ptr);
    free(SNN_val);
}


int main(int argc, char *argv[])
{
    printf("\nTesting read_graph_from_file1.c and create_SNN_graph1.c.....\n");
    test_read_and_create_SNN_graph1(argv[1]);

    printf("\nTesting read_graph_from_file2.c and create_SNN_graph2.c.....\n");
    test_read_and_create_SNN_graph2(argv[1]);

    printf("\n\nTesting check_node.c......................................\n");
    test_check_node(argv[1]);

    return 0;
}
