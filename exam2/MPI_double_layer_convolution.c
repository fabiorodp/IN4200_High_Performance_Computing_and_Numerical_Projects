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
    {
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

    // for( size_t x = 0; x < N*N; n++ )  // fusion of nested loopings
    //     size_t i = x/N; size_t j = x%N;
}


void double_layer_convolution(int M, int N, float *input, int K1, int K2, float *kernel1,
                              float *kernel2, int lenInput, int myRank, float **output, int *lenOutput)
{
    int s, i, j, ii, jj, len, M1_rank, M2_rank, N2_rank, M_out, N_out, lenOut1, lenOut2;
    float *resizedInput, *out1, *out2;
    double temp;

    M1_rank = lenInput/N + N - (lenInput%N);
    lenOut1 = (M1_rank-K1+1) * (N-K1+1);
    out1 = malloc( lenOut1 * sizeof *out1 );
    printf("\nlenOut1=%d\n", lenOut1 );
    printf("\nM1_rank=%d\n", M1_rank );

    M2_rank = (M1_rank-K1+1);
    N2_rank = (N-K1+1);
    M_out = M1_rank-K1-K2+2;
    N_out = N-K1-K2+2;
    lenOut2 = M_out * N_out;
    out2 = malloc( lenOut2 * sizeof *out1 );
    printf("\nlenOut2=%d\n", lenOut2 );
    printf("\nM_out=%d\n", M_out );
    printf("\nN_out=%d\n", N_out );
    printf("\nM2_rank=%d\n", M2_rank );

    // resizing input array to be able to make the calculations
    if ( (lenInput%N != 0) && (myRank != 0) )
    {
        len = lenInput + (N - (lenInput%N));
        resizedInput = malloc(len * sizeof *resizedInput);

        for ( i=0; i < len; i++ )
        {
            if ( i < N - (lenInput%N) ) resizedInput[i] = 0;
            else resizedInput[i] = input[i- (N - (lenInput%N))];
        }

        free(input);

        printf("\n\n");
        for ( s = 0; s < len; s++ ) 
            printf("resizedInput[s=%d]=%f ", s, resizedInput[s]);
        
        // computations of the first convolution:
        for ( i=0; i<=(M1_rank-K1); i++ )
        {
            for ( j=0; j<=(N-K1); j++ )
                {
                    printf("i=%d and j=%d\n", i, j);
                    temp=0.0;
                    for (ii=0; ii<K1; ii++)
                        for (jj=0; jj<K1; jj++)
                        {
                            printf("ii=%d and jj=%d >> ", ii, jj);
                            printf("%f * %f\n", resizedInput[ (i+ii)*N + (j+jj) ], kernel1[ ii*K1 + jj ]);
                            temp += resizedInput[ (i+ii)*N + (j+jj) ] * kernel1[ (ii*K1 + jj) ];
                        }
                    printf("out1[%d]=%f\n", i*(N-K1+1) + j, temp);
                    out1[ i*(N-K1+1) + j ] = temp;
                }
        }

        printf("\n\n");
        for ( s = 0; s < lenOut1; s++ )
            printf("out1[s=%d]=%f ", s, out1[s]);

        // computations of the second convolution:
        for ( i=0; i<=M2_rank-K2; i++ )
        {
            for ( j=0; j<=N2_rank-K2; j++ )
                {
                    printf("i=%d and j=%d\n", i, j);
                    temp=0.0;
                    for (ii=0; ii<K2; ii++)
                        for (jj=0; jj<K2; jj++)
                        {
                            printf("ii=%d and jj=%d >> ", ii, jj);
                            printf("%f * %f\n", out1[ (i+ii)*N2_rank + (j+jj) ], kernel2[ ii*K2 + jj ]);
                            temp += out1[ (i+ii)*N2_rank + (j+jj) ] * kernel2[ (ii*K2 + jj) ];
                        }
                    printf("out2[%d]=%f\n", i*(N2_rank-K2+1) + j, temp);
                    out2[ i*(N2_rank-K2+1) + j ] = temp;
                }
        }

        // freeing unnecessary out2
        free(out1);

        printf("\n\n");
        for ( s = 0; s < lenOut2; s++ )
            printf("out2[s=%d]=%f ", s, out2[s]);
        
        // truncating output[out] to have only the needed values 
        // freeing unnecessary out2
        (*output) = malloc( lenOut2-(N - (lenInput%N)) * **output);

        (*lenOutput) = 0;
        for ( s = 0; s < lenOut2; s++ )
        {
            if ( s < (N - (lenInput%N)) ) continue;
            (*output)[s-(N - (lenInput%N))] = out2[s];
            (*lenOutput)++;
        }

        free(out2);
    }

    else if ( (lenInput%N != 0) && (myRank == 0) )
    {
        // resizing to the correct shape of the matrix
        len = lenInput + (N - (lenInput%N));                        // correct length
        resizedInput = malloc(len * sizeof *resizedInput);          // correct allocated matrix in 1D fashion

        for ( i=0; i < len; i++ )
        {
            if ( i >= lenInput ) resizedInput[i] = 0;
            else resizedInput[i] = input[i];
        }

        // freeing unnecessary input
        free(input);

        printf("\n\n");
        for ( s = 0; s < len; s++ ) 
            printf("resizedInput[s=%d]=%f ", s, resizedInput[s]);
        
        // computations of the first convolution:
        for ( i=0; i<=(M1_rank-K1); i++ )
        {
            for ( j=0; j<=(N-K1); j++ )
                {
                    printf("i=%d and j=%d\n", i, j);
                    temp=0.0;
                    for (ii=0; ii<K1; ii++)
                        for (jj=0; jj<K1; jj++)
                        {
                            printf("ii=%d and jj=%d >> ", ii, jj);
                            printf("%f * %f\n", resizedInput[ (i+ii)*N + (j+jj) ], kernel1[ ii*K1 + jj ]);
                            temp += resizedInput[ (i+ii)*N + (j+jj) ] * kernel1[ (ii*K1 + jj) ];
                        }
                    printf("out1[%d]=%f\n", i*(N-K1+1) + j, temp);
                    out1[ i*(N-K1+1) + j ] = temp;
                }
        }

        // freeing unnecessary resizedInput
        free(resizedInput);

        printf("\n\n");
        for ( s = 0; s < lenOut1; s++ )
            printf("out1[s=%d]=%f ", s, out1[s]);

        // computations of the second convolution:
        for ( i=0; i<=M2_rank-K2; i++ )
        {
            for ( j=0; j<=N2_rank-K2; j++ )
                {
                    printf("i=%d and j=%d\n", i, j);
                    temp=0.0;
                    for (ii=0; ii<K2; ii++)
                        for (jj=0; jj<K2; jj++)
                        {
                            printf("ii=%d and jj=%d >> ", ii, jj);
                            printf("%f * %f\n", out1[ (i+ii)*N2_rank + (j+jj) ], kernel2[ ii*K2 + jj ]);
                            temp += out1[ (i+ii)*N2_rank + (j+jj) ] * kernel2[ (ii*K2 + jj) ];
                        }
                    printf("out2[%d]=%f\n", i*(N2_rank-K2+1) + j, temp);
                    out2[ i*(N2_rank-K2+1) + j ] = temp;
                }
        }

        // freeing unnecessary out2
        printf("\n freeing out1...\n");
        free(out1);

        printf("\n\n");
        for ( s = 0; s < lenOut2; s++ )
            printf("out2[s=%d]=%f ", s, out2[s]);
        
        // truncating output[out] to have only the needed values 
        // freeing unnecessary out2
        (*output) = malloc( lenOut2-(N - (lenInput%N)) * **output);

        printf("\n truncating output...\n");
        (*lenOutput) = 0;
        for ( s = 0; s < lenOut2; s++ )
        {
            if ( (*lenOutput) == lenOut2-(N - (lenInput%N)) ) break;
            (*output)[s] = out2[s];
            (*lenOutput)++;
        }

        printf("\n freeing out2...\n");
        free(out2);
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

    // if ( myRank == 0 ) 
    //     printf("\nM=%d, N=%d, input[29]=%f, K1=%d, kernel1[1]=%f, K2=%d, kernel2[1]=%f\n", M,N,input[29], K1, kernel1[1], K2, kernel2[1]);

    // if ( myRank == 1 ) 
    // {
    //     printf("\nM=%d, N=%d, K1=%d, K2=%d\n", M,N,K1,K2);
    //     for ( int a=0; a < K1*K1; a++) printf("%f ", kernel1[a]);
    // }

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

    float *myInput = malloc( num_elements[myRank] * sizeof *myInput );
    MPI_Barrier(MPI_COMM_WORLD);

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

    // convolution operations inside ranks
    float *myOutput;
    int lenMyOutput;

    if ( myRank == 0 )
    {
        double_layer_convolution(M, N, myInput, K1, K2, kernel1, kernel2, num_elements[myRank], myRank, &myOutput, &lenMyOutput);
        
        printf("\n\n");
        for (int i=0; i<lenMyOutput; i++)
        {
            printf("output[i=%d]=%f", i, myOutput[i]);
            printf("\nlenMyOutput=%d\n", lenMyOutput);
        }
    }
        

    // single_layer_convolution(M-K1+1, N-K1+1, myOutput1, K2, kernel2, &myOutput2);
    // printf("%f", myOutput2[0]);

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
