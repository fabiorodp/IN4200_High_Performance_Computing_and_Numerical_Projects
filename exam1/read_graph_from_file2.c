// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc read_graph_from_file2.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>


/*
Reading Connectivity Graphs - version 2 - Compressed Row Storage:

What does this function do:
1. This function reads a text file that contains a connectivity graph;
2. Outputs the number of nodes through N;
3. Assign values for the row_ptr, col_idx 1D arrays inside the function;
4. Passes the arrays out out (thus double pointer as type).

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

Specificies:
~~~~~~~~~~~~~~~~~~~~~~~~
- Uses two array1D: 
    -> int **col_idx:
        |-> (*col_idx) = malloc(2*N_edges * sizeof col_idx)
        |-> consecutively stores for all nodes the indices of their 
            nearest neighbors.
    
    -> int **row_ptr:
        |-> (*row_ptr) = malloc(N+1 * sizeof row_ptr)
        |-> "dissecting" the array col_idx with respect to the diff nodes.

- An extra array for SNN:
    |-> unsigned int **SNN_val:
        |-> (*SNN_val) = malloc(2*N * sizeof SNN_val)

Example format:
~~~~~~~~~~~~~~~~~~~~~~~~
Examples of col_idx and row_ptr for the Connectivity Graphs file in 
data/graph_example.txt:

col_idx: 1,2,3, 0,2,3, 0,1,3,4, 0,1,2,4, 2,3
row_ptr: 0,3,6,10,14,16

Arguments:
~~~~~~~~~~~~~~~~~~~~~~~~
*filename: file containing a connectivity graph.
*N := the number of nodes.
**row_ptr := of length N + 1 contains the indices at which new rows 
start in array val (not needed for binary values).
**col_idx := records the original column position of the all nonzeros
values.
*/
void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx){

    FILE *file;

    // opening file
    file = fopen(filename, "r");

    // returning error if file not found
    if ( file == NULL ){
        perror("Error: File not found.");
        exit(1);
    }

    // getting the number of nodes:= N and edges:= N_edges
    char ln[64];
    unsigned int N_edges;
    while ( fgets(ln, sizeof ln, file) )
        if (sscanf(ln, "#%*s%d%*s%u\n", N, &N_edges)) break;  // efficiency
        // printf("%d %d\n", *N, N_edges);
    
    // allocating arrays
    (*col_idx) = calloc( 2 * N_edges, sizeof **col_idx );
    (*row_ptr) = calloc( (*N + 1), sizeof **row_ptr );

    // assigning values to the arrays
    unsigned int count = 0;
    for (int node = 0; node < (*N)+1; node++)  // to keep arrays' order
    {   
        while ( fgets(ln, sizeof ln, file) ) 
        {
            if (ln[0] == '#') continue;  // efficiency
            
            unsigned int temp1, temp2;  // to not incur in memory storage trafic
            sscanf(ln, "%u %u\n", &temp1, &temp2);

            // printf("%u %u\n", temp1, temp2);
            // printf("comparision: temp1:%u node:%d\n", temp1, node);
            // printf("comparision: temp2:%u node:%d\n", temp2, node);

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
        
        // printf("idx%u\n\n", idx);
        
        // assigning the count
        (*row_ptr)[node+1] = count;
        
        // moving the cursor from the end to the beggining of the file
        rewind(file);
    }

    for ( int i = 0; i < 2*N_edges; i++)
        printf("%d", (*col_idx)[i]);
    
    printf("\n");
    
    for ( int i = 0; i < *N+1; i++)
        printf("%d", (*row_ptr)[i]);
    
    printf("\n");
}

int main(int argc, char *argv[]){

    int *col_idx;
    int *row_ptr;
    int N;
    read_graph_from_file2(argv[1], &N, &row_ptr, &col_idx);

    free(col_idx);
    free(row_ptr);

    return 0;
}
