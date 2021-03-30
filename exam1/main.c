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
    printf("Reading Connectivity Graph 1...\n");
    clock_t t0 = clock();
    read_graph_from_file1(filename, &N, &table2D);
    clock_t t1 = clock();

    double elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf(">> CPU time: %f s.\n", elapsed_time);

    // checking if the user want to print table2D
    printf("Please, type 'y' if you want to print table2D here:");
    printing_table2D(table2D, N);

    // calling the function to return SNN_table
    printf("Creating SNN graph 1 without OMP...\n");
    double tt0 = omp_get_wtime();
    t0 = clock();
    create_SNN_graph1(N, table2D, &SNN_table);
    t1 = clock();
    double tt1 = omp_get_wtime();

    elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    double elapsed_time1 = tt1-tt0;
    printf(">> CPU time: %f s && ELAPSED Time: %f s.\n", elapsed_time, elapsed_time1);

    // declaration for the SNN_table_omp
    int **SNN_table_omp;

    // calling the function to return SNN_table
    printf("Creating SNN graph 1 with OMP...\n");
    tt0 = omp_get_wtime();
    t0 = clock();
    create_SNN_graph1_omp(N, table2D, &SNN_table_omp);
    t1 = clock();
    tt1 = omp_get_wtime();

    double elapsed_time_omp = (double)(t1-t0)/CLOCKS_PER_SEC;
    double elapsed_time_omp1 = tt1-tt0;
    printf(">> CPU time: %f s && ELAPSED Time: %f s.\n", elapsed_time_omp, elapsed_time_omp1);

    // Speedup between SNN_table and SNN_table_omp
    printf("Speedup between SNN_table and SNN_table_omp:\n>> SNN_table/SNN_table_omp = %f",
           elapsed_time1/elapsed_time_omp1);

    // comparing if SNN_table and SNN_table_omp are equals
    printf("\nComparing if SNN_table and SNN_table_omp are equals:\n");
    comparing_2D_tables(SNN_table, SNN_table_omp, N);

    // checking if the user want to print SNN_table
    printf("Please, type 'y' if you want to print SNN_table here:");
    printing_SNN_table(SNN_table, N);

    // checking if the user want to print SNN_table_omp
    printf("Please, type 'y' if you want to print SNN_table_omp here:");
    printing_SNN_table(SNN_table_omp, N);

    // freeing memory
    free(*table2D);
    free(table2D);
    free(*SNN_table);
    free(SNN_table);
    free(*SNN_table_omp);
    free(SNN_table_omp);
}


void test_read_and_create_SNN_graph2(char *filename, int *N, int **col_idx,
                                     int **row_ptr, int **SNN_val)
{
    // calling the function to read file and return N, row_ptr and col_idx
    printf("Reading Connectivity Graph 2...\n");
    clock_t t0 = clock();
    read_graph_from_file2(filename, &(*N), &(*row_ptr), &(*col_idx));
    clock_t t1 = clock();

    double elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    printf(">> CPU time: %f s.\n", elapsed_time);

    // checking if the user want to print col_idx and row_ptr:
    printf("Please, type 'y' if you want to print col_idx and row_ptr here:");
    printing_CRS((*col_idx), (*row_ptr), (*N));

    // calling the function to return SNN_val
    printf("\nCreating SNN graph 2 without OMP...\n");
    double tt0 = omp_get_wtime();
    t0 = clock();
    create_SNN_graph2((*N), (*row_ptr), (*col_idx), &(*SNN_val));
    t1 = clock();
    double tt1 = omp_get_wtime();

    elapsed_time = (double)(t1-t0)/CLOCKS_PER_SEC;
    double elapsed_time1 = tt1-tt0;
    printf(">> CPU time: %f s && ELAPSED Time: %f s.\n", elapsed_time, elapsed_time1);

    // declaration for the SNN_val
    int *SNN_val_omp;

    // calling the function to return SNN_val
    printf("Creating SNN graph 2 with OMP...\n");
    tt0 = omp_get_wtime();
    t0 = clock();
    create_SNN_graph2_omp((*N), (*row_ptr), (*col_idx), &SNN_val_omp);
    t1 = clock();
    tt1 = omp_get_wtime();

    double elapsed_time_omp = (double)(t1-t0)/CLOCKS_PER_SEC;
    double elapsed_time_omp1 = tt1-tt0;
    printf(">> CPU time: %f s && ELAPSED Time: %f s.\n", elapsed_time_omp, elapsed_time_omp1);

    // Speedup between SNN_val and SNN_val_omp
    printf("Speedup between SNN_val and SNN_val_omp:\n>> SNN_val/SNN_val_omp = %f",
           elapsed_time1/elapsed_time_omp1);

    // Comparing if SNN_val and SNN_val_omp are equals
    printf("\nComparing if SNN_val and SNN_val_omp are equals:\n");
    comparing_SNN_val((*SNN_val), SNN_val_omp, (*row_ptr)[*N]);

    // checking if the user want to print SNN_val
    printf("Please, type 'y' if you want to print SNN_val here:");
    printing_SNN_val((*SNN_val), (*row_ptr)[*N]);

    // checking if the user want to print SNN_val_omp
    printf("\nPlease, type 'y' if you want to print SNN_val_omp here:");
    printing_SNN_val(SNN_val_omp, (*row_ptr)[*N]);

    free(SNN_val_omp);
}


void test_check_node(int N, int *col_idx, int *row_ptr, int *SNN_val)
{
    // user input for node_id and tau:
    int node_id, tau;
    printf("Please, give the node_id number:");
    scanf("%d", &node_id); getchar();
    printf("Now, the tau number:");
    scanf("%d", &tau); getchar();

    // calling the function to print the nodes that are in the cluster
    check_node(node_id, tau, N, row_ptr, col_idx, SNN_val);
}


/*
This is an example of the output using the data/graph_example.txt:

% ./a.out data/graph_example.txt 4

Testing read_graph_from_file1.c and create_SNN_graph1.c.....
Reading Connectivity Graph 1...
>> CPU time: 0.000079 s.
Please, type 'y' if you want to print table2D here:y
Printing...
0 1 1 1 0
1 0 1 1 0
1 1 0 1 1
1 1 1 0 1
0 0 1 1 0
Creating SNN graph 1 without OMP...
>> CPU time: 0.000019 s && ELAPSED Time: 0.000021 s.
Creating SNN graph 1 with OMP...
>> CPU time: 0.001021 s && ELAPSED Time: 0.000510 s.
Speedup between SNN_table and SNN_table_omp:
>> SNN_table/SNN_table_omp = 0.041141
Comparing if SNN_table and SNN_table_omp are equals:
>> True
Please, type 'y' if you want to print SNN_table here:y
Printing...
0 2 2 2 0
2 0 2 2 0
2 2 0 3 1
2 2 3 0 1
0 0 1 1 0
Please, type 'y' if you want to print SNN_table_omp here:y
Printing...
0 2 2 2 0
2 0 2 2 0
2 2 0 3 1
2 2 3 0 1
0 0 1 1 0

Testing read_graph_from_file2.c and create_SNN_graph2.c.....
Reading Connectivity Graph 2...
>> CPU time: 0.000229 s.
Please, type 'y' if you want to print col_idx and row_ptr here:y
Printing col_idx...
1 2 3 0 2 3 0 1 3 4 0 1 2 4 2 3
Printing row_ptr...
0 3 6 10 14 16
Creating SNN graph 2 without OMP...
>> CPU time: 0.000008 s && ELAPSED Time: 0.000009 s.
Creating SNN graph 2 with OMP...
>> CPU time: 0.000273 s && ELAPSED Time: 0.000114 s.
Speedup between SNN_val and SNN_val_omp:
>> SNN_val/SNN_val_omp = 0.079498
Comparing if SNN_val and SNN_val_omp are equals:
>> True
Please, type 'y' if you want to print SNN_val here:y
Printing SNN_val...
2 2 2 2 2 2 2 2 3 1 2 2 3 1 1 1
Please, type 'y' if you want to print SNN_val here:y
Printing SNN_val...
2 2 2 2 2 2 2 2 3 1 2 2 3 1 1 1

Testing check_node.c......................................
Please, give the node_id number:4
Now, the tau number:1
>> The Nodes that form a cluster with node_id 4 and tau 1: 0,1,3,2
*/
int main(int argc, char *argv[])
{
    // setting the number of threads
    omp_set_num_threads(atoi(argv[2]));

    printf("\nTesting read_graph_from_file1.c and create_SNN_graph1.c.....\n");
    test_read_and_create_SNN_graph1(argv[1]);

    // declaration for the num. of nodes, CRS matrices and SNN_val
    int N, *col_idx, *row_ptr, *SNN_val;

    printf("\nTesting read_graph_from_file2.c and create_SNN_graph2.c.....\n");
    test_read_and_create_SNN_graph2(argv[1], &N, &col_idx, &row_ptr, &SNN_val);

    printf("\n\nTesting check_node.c......................................\n");
    test_check_node(N, col_idx, row_ptr, SNN_val);

    free(col_idx);
    free(row_ptr);
    free(SNN_val);

    return 0;
}
