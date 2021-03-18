// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc create_SNN_graph2.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>


/*
Creating the corresponding Shared Nearest Neighbors SNN graph - version 2 -
Compressed Row Storage:

Inputs
~~~~~~~~~~~~~~~~~~~~~~~~
int N: Number of Nodes.
int *row_ptr:
int *col_idx:

Output
~~~~~~~~~~~~~~~~~~~~~~~~
int **SNN_val: 1D array containing the SNN's values, allocated inside the
function, in a CRS format.

Specifies:
~~~~~~~~~~~~~~~~~~~~~~~~
- An 2D array for SNN:
    |-> unsigned int **SNN_val:
        |-> (*SNN_val) = malloc(2*N_edges * sizeof SNN_val)

Example:
~~~~~~~~~~~~~~~~~~~~~~~~
For example,
an col_idx equals [1,2,3,   0,2,3,  0,1,3,4,    0,1,2,4,    2,3]
and row_ptr equals [0,3,6,10,14,16]
generates a SNN_val equals [ ]
*/
void create_SNN_graph2(char *filename, int *N, char ***table2D){

}


int main(int argc, char const *argv[]){

    return 0;
}
