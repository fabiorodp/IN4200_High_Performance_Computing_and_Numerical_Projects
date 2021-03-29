// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <omp.h>


void create_SNN_graph1_omp(int N, char **table2D, int ***SNN_table)
{
    // global variables that are shared in the parallel region by default
    size_t z, i, j, k;

    // allocating 2D array for SNNs
    (*SNN_table) = calloc(N, sizeof **SNN_table);

#pragma omp parallel
    {
#pragma omp for private(z)
        for (z = 0; z < N; z++)  // not dependent on anybody
            (*SNN_table)[z] = calloc(N, sizeof ***SNN_table);

#pragma omp for private(i, j, k) schedule(dynamic)
        for (i = 0; i < N; i++)  // not dependent on anybody
            for (j = 0; j < N; j++)  // depends on i (the outer loop)
            {
                // checking if the node connection does not repeat i.e. 0-1 or 1-0
                if (j <= i) continue;

                // checking if both nodes are connected
                if ((table2D[i][j] == 0) && (table2D[j][i] == 0)) continue;

                // assigning values in SNN_table
                for (k = 0; k < N; k++) // depends on i and j
                    if ((table2D[i][k] == 1) && (table2D[j][k] == 1))
                        (*SNN_table)[i][j] = (*SNN_table)[j][i] += 1;
            }
    }
}
