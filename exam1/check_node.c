// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc check_node.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>
#include "create_SNN_graph2.c"


/*
Checking whether a node can be in a cluster, where each node in the cluster
is directly connected with at least another node in the same cluster,
with the number of SNNs between them being equal or larger than the
threshold tau.

For a given node (node_idx) and a threshold value (tau), whether it can be
inside a cluster based on the input SNN graph.

If yes, the function prints out the other nodes inside the cluster.

Inputs
~~~~~~~~~~~~~~~~~~~~~~~~
int node_id:
int tau:
int N:
int *row_ptr:
int *col_idx:
int *SNN_val:

Example:
~~~~~~~~~~~~~~~~~~~~~~~~
For example,
an col_idx equals [ 1,2,3,    0,2,3,    0,1,3,4,    0,1,2,4,    2,3 ]
and row_ptr equals [ 0,3,6,10,14,16 ]
SNN_val equals [ 2,2,2,  2,2,2,  2,2,3,1,  2,2,3,1,  1,1 ]

<< node_id equals 4
<< tau equals 1 or 2
>> 0,1,2,3

<< node_id equals 2
<< tau equals 3
>> 2, 3
*/
void check_node(int node_id, int tau, int N, int *row_ptr, int *col_idx,
                int *SNN_val)
{
    // store
    int *store = malloc(row_ptr[N+1] * sizeof(int));

    // error if node_id >= N
    if ( node_id >= N )
    {
        printf("\nError: The node_id can not be higher than the number of "
               "nodes N.\n");
        exit(1);
    }

    int count = 0;
    for ( size_t i = 0; i < N; i++ )
    {
//        printf("\n i=%lu, node_id=%d\n", i, node_id);
        if ( i != node_id ) continue;

        size_t init = row_ptr[i];
        size_t end = row_ptr[i + 1];
//        printf("\n init=%lu, end=%lu\n", init, end);

        for ( size_t j = init; j < end; j++ )
        {
            for ( size_t ii = 0; ii < N; ii++ )
            {
//                printf("\ncol_idx[j=%lu] == %d == ii==%lu\n",
//                       j, col_idx[j], ii);

                if ( col_idx[j] == ii )
                {
                    size_t init2 = row_ptr[ii];
                    size_t end2 = row_ptr[ii + 1];

//                    printf("\ninit2=%lu, end2=%lu\n", init2, end2);

                    for ( size_t jj = init2; jj < end2; jj++ )
                    {

//                        printf("\nSNN_val[jj=%lu]=%d >= tau=%d\n",
//                               jj, SNN_val[jj], tau);

                        if ( SNN_val[jj] >= tau )
                        {
                            store[count] = col_idx[jj];
                            count++;
//                            printf("\ncount=%d, col_idx[jj=%lu]=%d\n",
//                                   count, jj, col_idx[jj]);
                        }
                    }
                }
            }
        }
    }

    for ( size_t z = 0; z < count; z++)
        printf("\n %d \n", store[z]);

    free(store);
}

int main(int argc, char *argv[])
{
    // declaration for the CRS matrix
    int *col_idx;
    int *row_ptr;

    // declaration for the num. of nodes
    int N;

    // calling the function to read file and return N, row_ptr and col_idx
    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);

    // declaration for the SNN_val
    int *SNN_val;

    // calling the function to return SNN_val
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);

    // testing returned values for SNN_val
    // row_ptr[N+1] is the length of col_idx
    for ( size_t i = 0; i < row_ptr[N+1]; i++)
        printf("%d", SNN_val[i]);

    // declaration for node_id
    int node_id = 3;

    // declaration for tau
    int tau = 3;

    // calling the function to print the nodes that are in the cluster
    check_node(node_id, tau, N, row_ptr, col_idx, SNN_val);

    return 0;
}
