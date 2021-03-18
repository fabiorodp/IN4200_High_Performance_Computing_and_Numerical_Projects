// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc read_graph_from_file1.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>
// #include <string.h>


/*
Reading Connectivity Graphs - version 1:

What does this function do:
1. reads a text file prescribed as input containing a connectivity graph;
2. outputs the number of nodes through N;
3. allocates a 2D table of correct dimension inside the function;
4. fills its content and passes it out (thus triple pointer as type for table2D).

Specifies:
~~~~~~~~~~~~~~~~~~~~~~~~
binary table2D N x N of char values (CG) or int values (SNN)
N:=num_nodes
table2D[i][j] = table2D[j][i]
table2D[i][i] = 0 := a node cannot be a nearst neighbor of itself
the # of values=1 in table2D is twice the # of edges

Example format:
~~~~~~~~~~~~~~~~~~~~~~~~
Example of table2D for the Connectivity Graphs file in 
data/graph_example.txt:

nodes   0   1   2   3   4
0       0   1   1   1   0
1       1   0   1   1   0
2       1   1   0   1   1
3       1   1   1   0   1
4       0   0   1   1   0

Arguments:
~~~~~~~~~~~~~~~~~~~~~~~~
char *filename: file containing a connectivity graph.
int *N := the number of nodes.
char ***table2D := 2D table of correct dimension.
*/
void read_graph_from_file1(char *filename, int *N, char ***table2D){

    // declaring variable pointer file
    FILE *file;

    // opening file
    file = fopen(filename, "r");

    // returning error if file not found
    if ( file == NULL ){
        perror("Error: File not found.");
        exit(1);
    }

    // getting the number of nodes
    char ln[64];
    while ( fgets(ln, sizeof ln, file) )
        if (sscanf(ln, "# Nodes: %d", N) == 1) break;  // efficiency
    
    //printf("%d\n", *N);
    
    // allocating 2D pointer arrays
    (*table2D) = calloc(*N, sizeof **table2D);  // *A = A[0], **A = A[0][0] 
    for ( int i = 0; i < *N; i++ )
        (*table2D)[i] = calloc(*N, sizeof ***table2D);  // ***A = A[0][0][0] = int

    // reading and assigning specific values to pointer arrays
    while ( fgets(ln, sizeof ln, file) ) {
        if (ln[0] == '#') continue;  // efficiency
        unsigned int temp1, temp2;  // to not incur in memory storage trafic
        sscanf(ln, "%u %u\n", &temp1, &temp2);
        // printf("%d %d\n", temp1, temp2);
        *(*(*table2D+temp1)+temp2) = *(*(*table2D+temp2)+temp1) += 1;
    }

    // closing file
    fclose(file);
}


//int main(int argc, char *argv[]){
//
//    // for the num. of nodes
//    int N;
//
//    // for the table2D
//    char **table2D;
//
//    // calling the function
//    read_graph_from_file1(argv[1], &N, &table2D);
//
//    // checking the returned array values
//    int values_of_1 = 0;
//    for (int i = 0; i < N; i++){
//        for (int j=0; j < N; j++){
//            printf("%d ", table2D[i][j]);
//            if ( table2D[i][j] == 1 ) values_of_1++;
//        }
//        printf("\n");
//    }
//
//    printf("Num. of Edges = 8 and Num. of values of 1 = %d\n", values_of_1);
//    printf("Ratio: %d\n", values_of_1/8);
//
//    // freeing memory
//    free(*table2D);
//    free(table2D);
//
//    return 0;
//}
