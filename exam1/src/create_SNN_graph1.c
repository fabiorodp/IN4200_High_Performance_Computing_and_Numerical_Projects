// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf


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
    // allocating 2D array for SNN_table
    (*SNN_table) = calloc(N, sizeof **SNN_table);
    for (size_t i = 0; i < N; i++)
        (*SNN_table)[i] = calloc(N, sizeof ***SNN_table);  // ***A = A[0][0][0]

    for (size_t i = 0; i < N; i++)  // not dependent for loop construct
        for (size_t j = 0; j < N; j++)  // depend on the outer loop
        {
            // checking if the node connection does not repeat i.e. 0-1 or 1-0
            // continue clause used for avoiding unnecessary work for the loop
            if (j <= i) continue;

            // checking if both nodes are connected
            // continue clause used for avoiding unnecessary work for the loop
            if ((table2D[i][j] == 0) && (table2D[j][i] == 0)) continue;

            // assigning values in SNN_table
            for (size_t k = 0; k < N; k++)
                if ((table2D[i][k] == 1) && (table2D[j][k] == 1))
                    (*SNN_table)[i][j] = (*SNN_table)[j][i] += 1;
        }
}
