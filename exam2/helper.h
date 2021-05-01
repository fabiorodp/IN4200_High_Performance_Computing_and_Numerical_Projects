// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


// to index a 2D matrix in 1D style
#define idx(i,j) (i*N + j)


// Used to index local copies of the data in matrix A.
#define idx2(i,j) (i*n_cols[myrank] + j)


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
void single_layer_convolution(int M, int N, float **input, int K, 
                              float **kernel, float **output)
{
    int i, j, ii, jj;
    double temp;
    for ( i=0; i<=M-K; i++ )
        for ( j=0; j<N-K; j++ )
            {
                temp=0.0;
                for (ii=0; ii<K; ii++)
                    for (jj=0; jj<K; jj++)
                        temp += input[i+ii][j+jj]*kernel[ii][jj];
                
                output[i][j] = temp;
            }
}
