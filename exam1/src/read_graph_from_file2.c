// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf


/*
Reading Connectivity Graphs - version 2 - Compressed Row Storage:

What does this function do:
1. Reads a text file that contains a connectivity graph;
2. Outputs the number of nodes through N;
3. Allocate and assign values for the row\_ptr, col\_idx 1D arrays inside the function;
4. Passes the arrays out (thus double pointer as type).

Justifications
~~~~~~~~~~~~~~~~~~~~~~~~
1st. More memory efficient than version 1 for the case where the
number of edges (N_edges) are smaller than N*N by orders of magnitude,
because the numerical values of the table2D (sparse matrix) from
version 1 are mostly zeros. It is a waste of float-point operations
when the sparse matrix is used in the way presented in version 1.
Besides, it is a waste of memory storage when a sparse matrix is
allocated as a 2D array.

2nd. CRS only stores the nonzero values and avoids multiplication
with zeros.

Specifies:
~~~~~~~~~~~~~~~~~~~~~~~~
- Uses two array1D:
    -> int **col_idx:
        |-> (*col_idx) = malloc(2*N_edges * sizeof col_idx)
        |-> consecutively stores for all nodes the indices of their
            nearest neighbors.

    -> int **row_ptr:
        |-> (*row_ptr) = malloc(N+1 * sizeof row_ptr)
        |-> "dissecting" the array col_idx with respect to the diff nodes.

Input and Output:
~~~~~~~~~~~~~~~~~~~~~~~~
char *filename: file containing a connectivity graph.
int *N := the number of nodes.
int **row_ptr := of length N + 1 contains the indices at which new rows
start in array val (not needed for binary values).
int **col_idx := records the original column position of the all non-zeros
values.

Example format:
~~~~~~~~~~~~~~~~~~~~~~~~
Examples of col_idx and row_ptr for the Connectivity Graphs file in
data/graph_example.txt:

col_idx: 1,2,3, 0,2,3, 0,1,3,4, 0,1,2,4, 2,3
row_ptr: 0,3,6,10,14,16
*/
void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx)
{
    FILE *file;

    // opening file
    file = fopen(filename, "r");

    // returning error if file not found
    if ( file == NULL ){
        perror("Error: File not found.");
        exit(1);
    }

    // getting the number of nodes:= N and edges:= N_edges
    char ln[128];
    unsigned int N_edges;
    while ( fgets(ln, sizeof ln, file) )
    {
        if (sscanf(ln, "#%*s%d%*s%u\n", N, &N_edges)) break;  // efficiency
    }

    // allocating arrays
    (*col_idx) = calloc( 2 * N_edges, sizeof **col_idx );
    (*row_ptr) = calloc( (*N + 1), sizeof **row_ptr );

    // declaring variables
    size_t node;
    unsigned int temp1, temp2, count = 0;

    // assigning values to the arrays
    for ( node = 0; node < (*N); node++ )  // to keep arrays' order
    {
        while ( fgets(ln, sizeof ln, file) )
        {
            if (ln[0] == '#') continue;  // efficiency

            // not incur in memory storage traffic
            sscanf(ln, "%u %u\n", &temp1, &temp2);

            if ( temp1 == node )
            {
                (*col_idx)[count] = temp2;
                count++;
            }
            else if ( temp2 == node )
            {
                (*col_idx)[count] = temp1;
                count++;
            }
        }

        // assigning the count
        (*row_ptr)[node+1] = count;

        // moving the cursor from the end to the beginning of the file
        rewind(file);
    }

    for ( size_t i=0; i < 2 * N_edges; i++ )
        printf("%d", (*col_idx)[i]);

    printf("\n");

    for ( size_t i=0; i < (*N + 1); i++ )
        printf("%d", (*row_ptr)[i]);

//
//    // sorting col_idx array
//    for (size_t i = 0; i < *N; i++)  // looping over row_ptr
//        for (size_t j = (*row_ptr)[i]; j < (*row_ptr)[i+1]; j++)
//            for (size_t k = j + 1; k <  (*row_ptr)[i+1]; k++)
//                if ( (*col_idx)[j] > (*col_idx)[k] )
//                {
//                    int temp =  (*col_idx)[j];
//                    (*col_idx)[j] = (*col_idx)[k];
//                    (*col_idx)[k] = temp;
//                }
}


int main(int argc, char *argv[])
{
    // declaration for the num. of nodes, CRS matrices and SNN_val
    int N, *col_idx, *row_ptr, *SNN_val;

    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);

//    for (size_t i = 0; i < row_ptr[N + 1]; i++)
//        printf("%d", col_idx[i]);
//
//    printf("\n");
//
//    for (size_t i = 0; i < N + 1; i++)
//        printf("%d", row_ptr[i]);
//
    free(row_ptr);
    free(col_idx);

    return 0;
}
