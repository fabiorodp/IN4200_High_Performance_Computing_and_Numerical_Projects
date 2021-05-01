// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc test3.c
// ./a.out


#include <stdio.h>
#include <stdlib.h>


#define NUM_OF_RANKS 5
#define maxNeededRanks 6


int main( int argc, char **argv )
{
    int M=6, N=5, K1=3, K2=2;
    int matrix[30] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                     16, 17,18,19,20,21,22,23,24,25,26,27,
                     28,29,30};
    
    // allocating arrays
    int *num_elements = malloc(NUM_OF_RANKS * sizeof *num_elements);  // # of elem. transfered
    int *displs = malloc(NUM_OF_RANKS * sizeof *displs);              // idx of the first

    // output row and col sizes
    int M_out = M - (K1-1) - (K2-1);  // 6-(3-1)-(2-1) = 3
    int N_out = N - (K1-1) - (K2-1);  // 5-(3-1)-(2-1) = 2
    int lenElem = (K1+K2-2)*(N+1)+1;  // (3+2-2)*(5+1)+1 = 19

    if ( NUM_OF_RANKS == maxNeededRanks )
    {
        // calculate displacement and number of elements for each rank
        // NUM_OF_RANKS=6 >> 6 blocks with 1 element
        // returning displs[NUM_OF_RANKS=6] = {0,1,5,6,10,11}
        // returning num_elements[NUM_OF_RANKS=6] = {19, 19, 19, 19, 19, 19}
        int count = 0;
        for ( int i = 0; i < M_out; i++ )
            for ( int j = 0; j < N_out; j++ )
            {
                displs[count] = i*N+j;
                num_elements[count] = (K1+K2-2)*(N+1)+1;
                count++;
            }
    }
    else
    {
        // calculate displacement and number of elements for each rank
        // Example: NUM_OF_RANKS=4 >> 2 blocks with 1 element and 2 with 2 elements
        // returning displs[NUM_OF_RANKS=4] = {0,1,5,10}
        // returning num_elements[NUM_OF_RANKS=4] = {19, 19, 20, 20}
        int numEntries = maxNeededRanks/NUM_OF_RANKS;               // 6/4 = 1
        int numBlocks_remainder = maxNeededRanks%NUM_OF_RANKS;      // 6%4 = 2
        int numBlocks = NUM_OF_RANKS - numBlocks_remainder;         // 4-2 = 2
        int displs_maxRank[maxNeededRanks];                         // [0,1,5,6,10,11]

        // calculate displacement and number of elements for all possible ranks (maxRanks)
        int count = 0;
        for ( int i = 0; i < M_out; i++ )
            for ( int j = 0; j < N_out; j++ )
            {
                displs_maxRank[count] = i*N+j;
                count++;
            }
        
        for ( int entry = 0; entry < (numBlocks+numBlocks_remainder); entry++ )
        {
            if ( entry < numBlocks )  // 0, 1
            {
                displs[entry] = ( entry == 0 ? 0 : displs_maxRank[entry*numEntries] );
                num_elements[entry] = displs_maxRank[numEntries-1]+lenElem;
            }
            else if ( entry == numBlocks )
            {
                displs[entry] = ( entry == 0 ? 0 : displs_maxRank[entry*numEntries] );
                num_elements[entry] = displs_maxRank[NUM_OF_RANKS-numBlocks_remainder+numEntries]-displs_maxRank[NUM_OF_RANKS-numBlocks_remainder]+lenElem;
            }
            else  // 2, 3
            {
                displs[entry] = ( entry == 0 ? 0 : displs_maxRank[entry*numEntries+1] );
                num_elements[entry] = displs_maxRank[NUM_OF_RANKS-numBlocks_remainder+numEntries]-displs_maxRank[NUM_OF_RANKS-numBlocks_remainder]+lenElem;
            }
        }
    }

    for ( int x = 0; x < NUM_OF_RANKS; x++ )
        printf("disp=%d and #elements=%d\n", displs[x], num_elements[x]);

    return 0;
}
