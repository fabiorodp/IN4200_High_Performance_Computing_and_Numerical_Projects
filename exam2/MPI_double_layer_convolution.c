// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


/*
A convolutional kernel is applied to an input 2D array in 
a serialized fashion.

Arguments:
~~~~~~~~~~~~~~~~~~~~~
int M: The number of rows for the input 2D array.
int N: The number of columns for the input 2D array.
float **input: The input 2D array.
int K: The dimensions KxK for the input kernel 2D array.
float **kernel: The input kernel 2D array.
float **output: The output 2D array.
*/
void single_layer_convolution(int M, int N, float *input, int K, 
                              float *kernel, float **output)
{
    int i, j, ii, jj;
    double temp;
    (*output) = malloc( (M-K+1) * (N-K+1) * sizeof **output );
    for ( i=0; i<=(M-K); i++ )
        for ( j=0; j<=(N-K); j++ )
            {
                printf("i=%d and j=%d\n", i, j);
                temp=0.0;
                for (ii=0; ii<K; ii++)
                    for (jj=0; jj<K; jj++)
                    {
                        printf("ii=%d and jj=%d >> ", ii, jj);
                        printf("%f * %f\n", input[ (i+ii)*N + (j+jj) ], kernel[ ii*K + jj ]);
                        temp += input[ (i+ii)*N + (j+jj) ] * kernel[ (ii*K + jj) ];
                    }
                printf("output[%d]=%f\n", i*(N-K+1) + j, temp);
                (*output)[ i*(N-K+1) + j ] = temp;
            }
}


/*
Double convolutional kernels are applied to an input 2D array in 
a parallel fashion using MPI (distributed-memory).

Arguments:
~~~~~~~~~~~~~~~~~~~~~
int M: The number of rows for the input 2D array.
int N: The number of columns for the input 2D array.
float *input: The input 2D array in 1D fashion.
int K1: The dimensions K1xK1 for the first input kernel 2D array.
float *kernel1: The first input kernel 2D array in 1D fashion.
int K2: The dimensions K2xK2 for the second input kernel 2D array.
float *kernel2: The second input kernel 2D array in 1D fashion.
float *output: The output 2D array in 1D fashion.

Remarks:
~~~~~~~~~~~~~~~~~~~~~
- All the function arguments, except float **input and float **output,
have the same values on all the MPI processes -> Broadcast from process 0;
- The 2D array float **input is only allocated on process 0 beforehand,
with M rows and N columns. On all the other processes, input is just a
NULL double pointer when function MPI_double_... is called.
- The 2D array float **output is only allocated on process 0 beforehand,
with M-K1-K2+2 rows and N-K1-K2+2 columns. Upon return, the content of
output on process 0 should be identical with that would have been returned 
by calling the sequential function double_layer_convolution. On all the
other processes, output is simply a NULL double pointer, both before and
after the function call.
- The MPI implementation should not be hard-coded for a specific number
of MPI processes. Neither should M, N, K1 and K2 be fixed as specific 
values. (it can be assumed that M and N are much larger than K1 and K2, 
as well as the total number of processes.)
- For simplicity, the division of work can adopt a 1D block-wise 
decomposition with respect to the rows. However, neither M nor M-K1-K2+2 
should be assumed to be divisible by the total number of processes.
- In the beginning of function MPI_double_layer_convolution, the MPI 
colective communication function MPI_Scatterv is suggested to be called, 
with process 0 being the root, so that the content of 2D array input is 
appropriately distributed. (The content of array input should not be 
broadcasted in its entirety from process 0 to the other porcesses.)
- At the end of function MPI_double_..., the MPI collective communication
function MPI_Gatherv is suggested to be called, with process 0 being the 
root, to collect the computed results from all the processes.
*/
void MPI_double_layer_convolution(int M, int N, float *input, 
                                  int K1, float *kernel1, int K2, 
                                  float *kernel2, float *output)
{
    // calculate max needed processes
    int maxNeededRanks = (M-K1-K2+2) * (N-K1-K2+2);

    // getting number of processes and current rank
    int NUM_OF_RANKS, myRank;
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_OF_RANKS);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // calculate displacements and number of rows for each process.
    int participants, remain;
    if ( NUM_OF_RANKS > maxNeededRanks ) participants = maxNeededRanks;
    else participants = NUM_OF_RANKS;

    // allocating arrays
    int *num_elements = malloc(participants * sizeof *num_elements);  // # of elem. transfered
    int *displs = malloc(participants * sizeof *displs);              // idx of the first

    // output row and col sizes
    int M_out = M - (K1-1) - (K2-1);  // 6-(3-1)-(2-1) = 3
    int N_out = N - (K1-1) - (K2-1);  // 5-(3-1)-(2-1) = 2
    int lenElem = (K1+K2-2)*(N+1)+1;  // (3+2-2)*(5+1)+1 = 19

    if ( participants == maxNeededRanks )
    {
        // calculate displacement and number of elements for each rank
        // participants=6 >> 6 blocks with 1 element
        // returning displs[participants=6] = {0,1,5,6,10,11}
        // returning num_elements[participants=6] = {19, 19, 19, 19, 19, 19}
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
        // Example: participants=4 >> 2 blocks with 1 element and 2 with 2 elements
        // returning displs[participants=4] = {0,1,5,10}
        // returning num_elements[participants=4] = {19, 19, 20, 20}
        int numEntries = maxNeededRanks/participants;               // 6/4 = 1
        int numBlocks_remainder = maxNeededRanks%participants;      // 6%4 = 2
        int numBlocks = participants - numBlocks_remainder;         // 4-2 = 2
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
                num_elements[entry] = displs_maxRank[participants-numBlocks_remainder+numEntries]-displs_maxRank[participants-numBlocks_remainder]+lenElem;
            }
            else  // 2, 3
            {
                displs[entry] = ( entry == 0 ? 0 : displs_maxRank[entry*numEntries+1] );
                num_elements[entry] = displs_maxRank[participants-numBlocks_remainder+numEntries]-displs_maxRank[participants-numBlocks_remainder]+lenElem;
            }
        }
    }

    double *myInput = malloc( num_elements[myRank] * sizeof *myInput );
    MPI_Barrier(MPI_COMM_WORLD);
    
    // // Define types.
    // MPI_Datatype col_temp, col_vec, recv_temp, recv_col;

    // // This is the type used to send the data.
    // MPI_Type_vector(N, 1, N, MPI_DOUBLE, &col_temp);

    // // This line is necesarry, but a bit confusing. See the README.
    // MPI_Type_create_resized(col_temp, 0, sizeof(double), &col_vec);
    // MPI_Type_commit(&col_vec);

    // // This type is used to recive. Only difference is the stride.
    // MPI_Type_vector(N, 1, n_cols[myrank], MPI_DOUBLE, &recv_temp);
    // MPI_Type_create_resized(recv_temp, 0, sizeof(double), &recv_col);
    // MPI_Type_commit(&recv_col);

    // Scatter A and x.
    MPI_Scatterv(input,                 /* void *sendbuf [in]:              The pointer to a buffer that contains the data to be sent 
                                                                            by the root process.
                                        */
                 num_elements,          // int *sendcounts [in]:            The number of elements to send to each process.
                 displs,                /* int *displs [in]:                The locations of the data to send to each communicator 
                                                                            process. Each location in the array is relative to the 
                                                                            corresponding element of the sendbuf array. In the sendbuf, 
                                                                            sendcounts, and displs parameter arrays, the nth element of 
                                                                            each array refers to the data to be sent to the nth 
                                                                            communicator process.
                                        */
                 MPI_FLOAT,             // MPI_Datatype sendtype [in]:      The MPI data type of each element in the buffer.
                 myInput,               /* void *recvbuf [out]:             The pointer to a buffer that contains the data that is 
                                                                            received on each process.
                                        */
                 num_elements[myRank],  /* int recvcount [out]:             The number of elements in the receive buffer. If the 
                                                                            count is zero, the data part of the message is empty. 
                                        */
                 MPI_FLOAT,             // MPI_Datatype recvtype [out]:     The data type of the elements in the receive buffer.
                 0,                     /* int root:                        The rank in the sending process within the specified 
                                                                            communicator.
                                        */
                 MPI_COMM_WORLD         // MPI_Comm comm:                   The MPI_Comm communicator handle.
    );

    // convolution operations
    for ( int x = 0; x < num_elements[myRank]; x++ )
        printf("rank=%d and input_elements=%f\n", myRank, myInput[x]);

    // // getter
    // if (myrank==0) {
    //     output = malloc(N * sizeof *output);
    // }
    // MPI_Reduce(my_y, // Send buffer.
    //            y, // Receive buffer.
    //            N,
    //            MPI_DOUBLE,
    //            MPI_SUM,
    //            0, // Root, the result ends up here.
    //            MPI_COMM_WORLD
    // );
}
