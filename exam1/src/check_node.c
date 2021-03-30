// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf


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
int node_id: Given node_id by user.
int tau: Given tau by user.
int N: Number of nodes.
int *row_ptr: Row pointers.
int *col_idx: Column indices.
int *SNN_val: SNN values.

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
    // error if node_id >= N
    if ( node_id >= N )
    {
        printf("\nError: The node_id can not be higher than the number of "
               "nodes N.\n");
        exit(1);
    }

    // store nodes that are in the same cluster
    // maximum possible are N minus 1 that is represented by the given node_id
    int *cluster = malloc(row_ptr[N] * sizeof *cluster);

    // global variables
    size_t i, j, x;

    // assigning -1 for all elements in cluster array
    // to not have any value equals node_id in cluster array
    for ( i = 0; i < row_ptr[N]; i++ ) cluster[i] = -1;

    // finding the clustered nodes and storing in the cluster array
    int count = 0;
    for ( i = row_ptr[node_id]; i < row_ptr[node_id + 1]; i++ ) // in element/row
    {
        for ( j = row_ptr[col_idx[i]]; j < row_ptr[col_idx[i] + 1]; j++ )  // in element/col
            if ( SNN_val[j] >= tau )
            {
                int is_in = 0;

                // checking if the node is already stored
                for ( x = 0; x < row_ptr[N]; x++ )
                {
                    if ( (cluster[x] == col_idx[j]) || (node_id == col_idx[j]) )
                        is_in = 1;
                }

                if ( is_in == 0 )
                {
                    cluster[count] = col_idx[j];
                    count++;
                }
            }
    }

    // reducing memory usage
    cluster = realloc(cluster, count * sizeof *cluster);

    // printing clustered nodes
    printf(">> The Nodes that form a cluster with node_id %d and tau %d: ",
           node_id, tau);
    for ( size_t z = 0; z < count; z++)
    {
        if ( z != 0 ) printf(",");
        printf("%d", cluster[z]);
    }

    // freeing memory
    free(cluster);
}
