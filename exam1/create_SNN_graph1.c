// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc create_SNN_graph1.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>
#include "read_graph_from_file1.c"


/*
Creating the corresponding Shared Nearest Neighbors SNN graph - version 1

Inputs
~~~~~~~~~~~~~~~~~~~~~~~~
int N: Number of Nodes.
char **table2D: Array containing the 2D table.

Output
~~~~~~~~~~~~~~~~~~~~~~~~
int ***SNN_table: 2D array containing the SNNs, allocated inside the function.

Example:
~~~~~~~~~~~~~~~~~~~~~~~~
For example, an inputs N=5 and table2D equals
nodes   0   1   2   3   4
0       0   1   1   1   0
1       1   0   1   1   0
2       1   1   0   1   1
3       1   1   1   0   1
4       0   0   1   1   0

generates a SNN_table equals
nodes   0   1   2   3   4
0       0   2   2   2   0
1       2   0   2   2   0
2       2   2   0   3   1
3       2   2   3   0   1
4       0   0   1   1   0
*/
void create_SNN_graph1(int N, char **table2D, int ***SNN_table)
{
    // allocating 2D array for SNNs
    (*SNN_table) = calloc(N, sizeof **SNN_table);
    for ( size_t i = 0; i < N; i++ )
        (*SNN_table)[i] = calloc(N, sizeof ***SNN_table);  // ***A = A[0][0][0]


    for ( size_t i = 0; i < N; i++ )  // locking rows
    {
        for ( size_t j = 0; j < N; j++ )  // locking cols
        {
            // checking if the node connection does not repeat i.e. 0-1 or 1-0
            if ( j <= i ) continue;

            // checking if both nodes are connected
            if ( (table2D[i][j] == 0) && (table2D[j][i] == 0) ) continue;

            for ( size_t k = 0; k < N; k++ )
            {
                // printf("%lu %lu %lu\n", i, j, k);
                // printf("Compare: %d %d\n", table2D[i][k], table2D[j][k]);

                if ( (table2D[i][k] == 1) && (table2D[j][k] == 1) )
                {
                    (*SNN_table)[i][j] = (*SNN_table)[j][i] += 1;
                }
            }
        }
    }
}


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

    // calling the function to return SNN_table
    create_SNN_graph1(N, table2D, &SNN_table);

    // checking the returned array values
    for (int i = 0; i < N; i++){
        for (int j=0; j < N; j++)
            printf("%d ", SNN_table[i][j]);
        printf("\n");
    }

    // freeing memory
    free(*table2D);
    free(table2D);
    free(*SNN_table);
    free(SNN_table);

    return 0;
}
