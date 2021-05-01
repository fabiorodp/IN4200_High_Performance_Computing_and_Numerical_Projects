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
    int maxNeededProcesses = (M-K1-K2+2) * (N-K1-K2+2)

    // getting number of processes and current rank
    int numProcesses, myRank;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // calculate displacements and number of rows for each process.
    int participants, remain;
    if ( numProcesses > maxNeededProcesses ) participants = maxNeededProcesses;
    else participants = numProcesses;

    int *n_cols = malloc( participants * sizeof *n_cols );
    int *displs = malloc( participants * sizeof *displs );

    if ( (M * N) % participants == 0 ) remainder = 0
    if ( (M * N) % participants != 0 ) remainder = (M * N) % participants;

    int cols = N/size;
    int remainder = N%size;

    // Last remainder processes gets an extra row.
    displs[0] = 0;
    for ( int rank = 0; rank < size-1; rank++ ) {
        n_cols[rank] = cols + ((rank >= (size - remainder)) ? 1:0);
        displs[rank+1] = displs[rank] + n_cols[rank];
    }
    n_cols[size-1] = cols + ((size-1) >= (size - remainder) ? 1:0);

    double *myA = malloc( N*n_cols[myrank] * sizeof *myA );
    double *my_x = malloc( n_cols[myrank] * sizeof *my_x );
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Define types.
    MPI_Datatype col_temp, col_vec, recv_temp, recv_col;

    // This is the type used to send the data.
    MPI_Type_vector(N, 1, N, MPI_DOUBLE, &col_temp);
    // This line is necesarry, but a bit confusing. See the README.
    MPI_Type_create_resized(col_temp, 0, sizeof(double), &col_vec);
    MPI_Type_commit(&col_vec);

    // This type is used to recive. Only difference is the stride.
    MPI_Type_vector(N, 1, n_cols[myrank], MPI_DOUBLE, &recv_temp);
    MPI_Type_create_resized(recv_temp, 0, sizeof(double), &recv_col);
    MPI_Type_commit(&recv_col);

    // Scatter A and x.
    MPI_Scatterv(A,                 // sendbuf, matters only for root process.
                 n_cols,
                 displs,
                 col_vec,           // The new type used to send.
                 myA,
                 n_cols[myrank],
                 recv_col,          // New type used to receive.
                 0,
                 MPI_COMM_WORLD);

    MPI_Scatterv(x,
                 n_cols,
                 displs,
                 MPI_DOUBLE,
                 my_x,
                 n_cols[myrank],
                 MPI_DOUBLE,
                 0,
                 MPI_COMM_WORLD);
}

araay[1][2]
array[1*N+2]
