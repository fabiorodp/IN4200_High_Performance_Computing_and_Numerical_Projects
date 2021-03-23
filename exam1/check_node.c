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
    // error if node_id >= N
    if ( node_id >= N )
    {
        printf("\nError: The node_id can not be higher than the number of "
               "nodes N.\n");
        exit(1);
    }

    // store nodes that are in the same cluster
    // maximum possible are N minus 1 that is represented by the given node_id
    int *cluster = malloc(N-1 * sizeof(int));

    // hard-coding to not have any value equals node_id in cluster array
    for ( size_t x = 0; x < (N-1); x++ )
        cluster[x] = -1;

    // finding the clustered nodes and storing in the cluster array
    int count = 0;
    for ( size_t i = 0; i < N; i++ )
    {
        if ( i != node_id ) continue;

        size_t init = row_ptr[i];
        size_t end = row_ptr[i + 1];

        for ( size_t j = init; j < end; j++ )
            for ( size_t ii = 0; ii < N; ii++ )
                if ( col_idx[j] == ii )
                {
                    size_t init2 = row_ptr[ii];
                    size_t end2 = row_ptr[ii + 1];

                    for ( size_t jj = init2; jj < end2; jj++ )
                        if ( SNN_val[jj] >= tau )
                        {
                            // checking if the node is already stored
                            int is_in = 0;
                            for ( size_t x = 0; x < (N-1); x++ )
                                if ( (cluster[x] == col_idx[jj])
                                || (node_id== col_idx[jj]))
                                    is_in = 1;

                            if ( is_in == 0 )
                            {
                                cluster[count] = col_idx[jj];
                                count++;
                            }
                        }
                }
    }

    // reducing memory usage
    cluster = realloc(cluster, count * sizeof(int));

    // printing clustered nodes
    printf("\nNodes that form a cluster with node_id %d and tau %d: ",
           node_id, tau);
    for ( size_t z = 0; z < count; z++)
    {
        if ( z != 0 ) printf(",");
        printf("%d", cluster[z]);
    }
    printf("\n");

    // freeing memory
    free(cluster);
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

    // declaration for node_id
    int node_id = 4;

    // declaration for tau
    int tau = 3;

    // calling the function to print the nodes that are in the cluster
    check_node(node_id, tau, N, row_ptr, col_idx, SNN_val);

    return 0;
}
