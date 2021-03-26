// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc create_SNN_graph1.c  // without OpenMP
// clang -Xpreprocessor -fopenmp create_SNN_graph1.c -lomp  // with OpenMP
// ./a.out


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <omp.h>


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
void create_SNN_graph1_omp(int N, char **table2D, int ***SNN_table)
{
    // allocating 2D array for SNNs
    (*SNN_table) = calloc(N, sizeof **SNN_table);

#pragma omp parallel  // initialize parallel region
    {
        size_t i, j, k;

#pragma omp for  // parallelize single for loop
        for (i = 0; i < N; i++)  // allocating 2D array for SNNs
            (*SNN_table)[i] = calloc(N, sizeof ***SNN_table);

#pragma omp for schedule(dynamic)
        for (i = 0; i < N; i++)  // rows >> parallelize
            for (j = 0; j < N; j++)  // cols >> depend on the outer loop
            {
                // checking if the node connection does not repeat i.e. 0-1 or 1-0
                if (j <= i) continue;

                // checking if both nodes are connected
                if ((table2D[i][j] == 0) && (table2D[j][i] == 0)) continue;

                for (k = 0; k < N; k++)
                    if ((table2D[i][k] == 1) && (table2D[j][k] == 1))
                        (*SNN_table)[i][j] = (*SNN_table)[j][i] += 1;
            }
    }
}
