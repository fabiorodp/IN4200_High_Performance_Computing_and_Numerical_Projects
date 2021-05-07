// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc test3.c
// ./a.out


#include <stdio.h>
#include <stdlib.h>


#define NUM_OF_RANKS 6
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
    {
        printf("disp=%d and #elements=%d\n", displs[x], num_elements[x]);
        int M1_rank = (num_elements[x]%N)!=0 ? num_elements[x] / N + (N - (num_elements[x]%N)) : num_elements[x] / N;
        printf("M1_rank=%d\n", M1_rank);
        int M2_rank = M1_rank-K1+1;
        printf("M2_rank=%d\n", M2_rank);
        int N2_rank = N-K1+1;
        printf("N2_rank=%d\n", N2_rank);

    }

    int lenOutput = (M-K1-K2+2) * (N-K1-K2+2);
    int div = lenOutput/NUM_OF_RANKS;
    int rem = lenOutput%NUM_OF_RANKS;

    // computing the number of elements that is received from each process.
    int *recvcounts = malloc( NUM_OF_RANKS * sizeof *recvcounts );

    // The location, relative to the recvbuf parameter, of the data from each communicator process. 
    // The data that is received from process j is placed into the receive buffer of the root process 
    // offset displs[x] elements from the sendbuf pointer.
    int *recvDispls = malloc( NUM_OF_RANKS * sizeof *displs );

    int sum = 0;
    for ( int x = 0; x < NUM_OF_RANKS; x++ )
    {
        if ( x < NUM_OF_RANKS-(lenOutput%NUM_OF_RANKS) ) recvcounts[x] = div;
        else recvcounts[x] = div+1;

        sum += x == 0 ? 0 : recvcounts[x-1];
        recvDispls[x] = x == 0 ? 0 : sum;
        // printf("\nrecvcounts[x=%d]=%d and recvDispls[x=%d]=%d", x, recvcounts[x], x, recvDispls[x] );
    }

    // printf("\n%d\n", lenOutput);
    return 0;
}
