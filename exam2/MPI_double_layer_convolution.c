// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


/*
Double convolutional kernels are applied to an input 2D array in 
a serialized fashion.

Arguments:
~~~~~~~~~~~~~~~~~~~~~
int M: The number of rows for the input 2D array.
int N: The number of columns for the input 2D array.
float **input: The input 2D array.
int K1: The dimensions K1xK1 for the first input kernel 2D array.
float **kernel1: The first input kernel 2D array.
int K2: The dimensions K2xK2 for the second input kernel 2D array.
float **kernel2: The second input kernel 2D array.
float **output: The output 2D array.
*/
void double_layer_convolution(int M, int N, float **input, 
                              int K1, float **kernel1, int K2, 
                              float **kernel2, float **output)
{

}


/*
Double convolutional kernels are applied to an input 2D array in 
a parallel fashion using MPI (distributed-memory).

Arguments:
~~~~~~~~~~~~~~~~~~~~~
int M: The number of rows for the input 2D array.
int N: The number of columns for the input 2D array.
float **input: The input 2D array.
int K1: The dimensions K1xK1 for the first input kernel 2D array.
float **kernel1: The first input kernel 2D array.
int K2: The dimensions K2xK2 for the second input kernel 2D array.
float **kernel2: The second input kernel 2D array.
float **output: The output 2D array.

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
other processes, output is simply a NULL double pointer, bith before and
adter the function call.
- The MPI implementation should not be hard-coded for a specific number
of MPI processes. Neither should M, N, K1 and K2 be fixed as specific 
values. (it can be assumed that M and N are much larger than K1 and K2, 
as well as the total number of processes.)
- For simplicity, the division of work can adopt a 1D block-wise 
decomposition with respect to the rows. However, neither M nor M-K1-K2+2 
should be assumedvto be divisible by the total number of processes.
- In the beginning of function MPI_double_layer_convolution, the MPI 
colective communication function MPI_Scatterv is suggested to be called, 
with process 0 being the root, so that the content of 2D array input is 
appropriately distributed. (The content of array input should not be 
broadcasted in its entirety from process 0 to the other porcesses.)
- At the end of function MPI_double_..., the MPI collective communication
function MPI_Gatherv is suggested to be called, with process 0 being the 
root, to collect the computed results from all the processes.
*/
void MPI_double_layer_convolution(int M, int N, float **input, 
                                  int K1, float **kernel1, int K2, 
                                  float **kernel2, float **output)
{

}
