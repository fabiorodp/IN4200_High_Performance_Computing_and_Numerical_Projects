// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


/*
A convolutional kernel is applied to an input array in a serialized fashion.

Arguments:
~~~~~~~~~~~~~~~~~~~~~
int M: The number of rows.
int N: The number of columns.
float *input: The input array in 1D fashion.
int K: The dimensions KxK for the input kernel in 1D array fashion.
float *kernel: The input kernel in 1D array fashion.
float **output: The output array in 1D array fashion.
*/
void single_layer_convolution(int M, int N, float *input, int K, float *kernel, float **output)
{
    size_t i, j, ii, jj, x;
    float temp;
    
    // allocating the output array in 1D fashion
    (*output) = malloc( (M-K+1) * (N-K+1) * sizeof **output );
    
    for ( i=0; i<=(M-K); i++ )
        for ( j=0; j<=(N-K); j++ )
            {
                temp=0.0;
                
                // fusion of nested loopings >> to increase performance
                for( x = 0; x < K*K; x++ )
                {
                    ii = x/K; jj = x%K;
                    temp += input[ (i+ii)*N + (j+jj) ] * kernel[ (ii*K + jj) ];
                }

                (*output)[ i*(N-K+1) + j ] = temp;
            }     
}

/*
Double convolutional kernels are applied to an input array.
*/
void double_layer_convolution(int M, int N, float *input, int K1, int K2, float *kernel1,
                              float *kernel2, int lenInput, int myRank, float **output, int *lenOutput)
{
    // allocating global variables
    size_t i;
    int len, M1_rank, M2_rank, N2_rank, lenOut2, q, r;
    float *resizedInput, *out1, *out2;

    M1_rank = (lenInput%N)!=0 ? lenInput / N + (N - (lenInput%N)) : lenInput / N;
    M2_rank = (M1_rank-K1+1);
    N2_rank = (N-K1+1);
    lenOut2 = (M1_rank-K1-K2+2) * (N-K1-K2+2);
    
    q = lenInput/N;
    r = lenInput%N;

    // allocating arrays
    len = lenInput + (N - r);
    resizedInput = calloc(len, sizeof *resizedInput);
    (*output) = malloc( lenOut2-(N - (lenInput%N)) * sizeof *(*output));

    // resizing input array to be able to make the calculations
    if ( (lenInput%N != 0) && (myRank != 0) )
    {
        // resizing to the correct shape of the matrix
        for ( i=0; i < len; i++ )
        {
            if ( i < N - r ) resizedInput[i] = 0;
            else resizedInput[i] = input[i - (N - r)];
        }

        // freeing unnecessary array input
        free(input);
        
        // computations of the first convolution:
        single_layer_convolution(M1_rank, N, resizedInput, K1, kernel1, &out1);

        // computations of the second convolution:
        single_layer_convolution(M2_rank, N2_rank, out1, K2, kernel2, &out2);
        
        // freeing unnecessary out1
        free(out1);
        
        // truncating output[out] to have only the needed element values
        (*lenOutput) = 0;
        for ( i = 0; i < lenOut2; i++ )
        {
            if ( i < (N - (lenInput%N)) ) continue;
            (*output)[i-(N - (lenInput%N))] = out2[i];
            (*lenOutput)++;
        }

        // freeing unnecessary out2
        free(out2);
    }

    else if ( (lenInput%N != 0) && (myRank == 0) )
    {   
        // resizing to the correct shape of the matrix
        for ( i = 0; i < len; i++ ) resizedInput[i] = input[i];

        // freeing unnecessary input
        free(input);
        
        // computations of the first convolution:
        single_layer_convolution(M1_rank, N, resizedInput, K1, kernel1, &out1);

        // freeing unnecessary resizedInput
        free(resizedInput);

        // computations of the second convolution:
        single_layer_convolution(M2_rank, N2_rank, out1, K2, kernel2, &out2);

        // freeing unnecessary out1
        free(out1);

        // truncating output[out] to have only the needed values 
        // assingning values in output[out] array
        (*lenOutput) = 0;
        for ( i = 0; i < lenOut2; i++ )
        {
            if ( i >= lenOut2-(N - (lenInput%N)) ) break;  // error here
            (*output)[i] = out2[i];
            (*lenOutput)++;
        }

        // freeing unnecessary out2
        free(out2);
    }

    else
    {
        // computations of the first convolution:
        single_layer_convolution(M1_rank, N, input, K1, kernel1, &out1);

        // freeing unnecessary resizedInput
        free(input);

        // computations of the second convolution:
        single_layer_convolution(M2_rank, N2_rank, out1, K2, kernel2, &out2);

        // freeing unnecessary out1
        free(out1);

        // assingning values in output[out] array
        for ( i = 0; i < lenOut2; i++ ) (*output)[i] = out2[i];
        (*lenOutput) = lenOut2;   // returning the length for the array output
        
        // freeing unnecessary out2
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
void MPI_double_layer_convolution(int M, int N, float *input, int K1, float *kernel1, int K2, 
                                  float *kernel2, float **output)
{
    // getting number of processes and current rank
    int NUM_OF_RANKS, myRank;
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_OF_RANKS);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // global variables
    size_t i, j, u, count, sum, entry;
    float *myInput, *myOutput;
    int lenMyOutput;
    int lenOutput = (M-K1-K2+2) * (N-K1-K2+2);
    int div = lenOutput/NUM_OF_RANKS;
    int rem = lenOutput%NUM_OF_RANKS;
    int M_out = M - K1 - K2 + 2;
    int N_out = N - K1 - K2 + 2;
    int minNumElem = (K1+K2-2)*(N+1)+1;

    // allocating arrays for the number of elements and displacement that is sent from each process.
    int num_elements[NUM_OF_RANKS], displs[NUM_OF_RANKS], displs_maxRank[lenOutput];

    // allocating arrays for the number of elements and displacement that is received from each process.
    int recvcounts[NUM_OF_RANKS], recvDispls[NUM_OF_RANKS];
    int numEntries, numBlocks_remainder, numBlocks;

    // calculate displacements and number of rows for each process.
    if ( NUM_OF_RANKS == lenOutput )
    {
        // calculate displacement and number of elements for each rank
        // participants=6 >> 6 blocks with 1 element
        // returning displs[participants=6] = {0,1,5,6,10,11}
        // returning num_elements[participants=6] = {19, 19, 19, 19, 19, 19}
        sum = 0;
        for ( u = 0; u < NUM_OF_RANKS; u++ )
        {
            num_elements[u] = minNumElem;
            
            sum += u == 0 ? 0 : num_elements[u-1];
            displs[u] = u == 0 ? 0 : sum;
        }
    }

    else
    {
        // calculate displacement and number of elements for each rank
        // Example: NUM_OF_RANKS=4 >> 2 blocks with 1 element and 2 with 2 elements
        // returning displs[NUM_OF_RANKS=4] = {0,1,5,10}
        // returning num_elements[NUM_OF_RANKS=4] = {19, 19, 20, 20}
        numEntries = lenOutput/NUM_OF_RANKS;
        numBlocks_remainder = lenOutput%NUM_OF_RANKS;
        numBlocks = NUM_OF_RANKS - numBlocks_remainder;

        // calculate displacement and number of elements for all possible ranks (maxRanks)
        count = 0;
        for ( i = 0; i < M_out; i++ )
            for ( j = 0; j < N_out; j++ )
            {
                displs_maxRank[count] = i*N+j;
                count++;
            }
        
        for ( entry = 0; entry < (numBlocks+numBlocks_remainder); entry++ )
        {
            if ( entry < numBlocks )
            {
                displs[entry] = ( entry == 0 ? 0 : displs_maxRank[entry*numEntries] );
                num_elements[entry] = displs_maxRank[numEntries-1]+minNumElem;
            }
            else if ( entry == numBlocks )
            {
                displs[entry] = ( entry == 0 ? 0 : displs_maxRank[entry*numEntries] );
                num_elements[entry] = displs_maxRank[NUM_OF_RANKS-numBlocks_remainder+numEntries]-displs_maxRank[NUM_OF_RANKS-numBlocks_remainder]+minNumElem;
            }
            else
            {
                displs[entry] = ( entry == 0 ? 0 : displs_maxRank[entry*numEntries+1] );
                num_elements[entry] = displs_maxRank[NUM_OF_RANKS-numBlocks_remainder+numEntries]-displs_maxRank[NUM_OF_RANKS-numBlocks_remainder]+minNumElem;
            }
        }
    }
    
    myInput = malloc( num_elements[myRank] * sizeof *myInput );
    // for ( int u = 0; u < NUM_OF_RANKS; u++ ) printf("num_elements[%d]=%d, displs[%d]=%d\n", u, num_elements[u], u, displs[u]);
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

    // computing the convolutions
    double_layer_convolution(M, N, myInput, K1, K2, kernel1, kernel2, num_elements[myRank], myRank, &myOutput, &lenMyOutput);

    // The location, relative to the recvbuf parameter, of the data from each communicator process. 
    // The data that is received from process j is placed into the receive buffer of the root process 
    // offset displs[x] elements from the sendbuf pointer.
    sum = 0;
    for ( u = 0; u < NUM_OF_RANKS; u++ )
    {
        if ( u < NUM_OF_RANKS-rem ) recvcounts[u] = div;
        else recvcounts[u] = div+1;

        sum += u == 0 ? 0 : recvcounts[u-1];
        recvDispls[u] = u == 0 ? 0 : sum;
    }

    MPI_Gatherv(
        myOutput,               // void *sendbuf[in]: The handle to a buffer that contains the data to be sent to the root process.
        lenMyOutput,            // int sendcount[in]: The number of elements in the send buffer.
        MPI_FLOAT,              // MPI_Datatype sendtype: The data type of each element in the buffer.
        *output,                // void *recvbuf[out]: The handle to a buffer on the root process that contains the data that is received from each process, including data that is sent by the root process.
        recvcounts,             // int *recvcounts[][in]: The number of elements that is received from each process.
        recvDispls,             // int *displs[][in]:
        MPI_FLOAT,              // MPI_Datatype recvtype:
        0,                      // int root:
        MPI_COMM_WORLD          // MPI_Comm comm:
    );
}
