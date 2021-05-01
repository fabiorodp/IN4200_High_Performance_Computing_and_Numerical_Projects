#include <stdlib.h>
#include <stdio.h>
#include "MPI_double_layer_convolution.c"


int main(int argc, char *argv[])
{
    int M=6, N=5, K1=3, K2=2, count;

    float *input = malloc(M * N * sizeof *input);
    for ( int i = 0; i < M; i++ )
        for ( int j=0; j < N; j++ )
            input[i*N + j] = ++count;

    printf("\n");
    for (int i=0; i<M; i++)
    {
        for (int j=0; j<N; j++)
            printf("%f ", input[i*N + j]);
        
        printf("\n");
    }

    int bi=0;
    float *kernel1 = malloc(K1 * K1 * sizeof *input);
    for ( int i = 0; i < K1; i++ )
        for ( int j=0; j < K1; j++ )
            bi = kernel1[i*K1 + j] = (bi==0) ? 1 : 0;
    
    printf("\n");
    for (int i=0; i<K1; i++)
    {
        for (int j=0; j<K1; j++)
            printf("%f ", kernel1[i*K1 + j]);
        
        printf("\n");
    }

    float *output1;
    single_layer_convolution(M, N, input, K1, kernel1, &output1);

    printf("\n");
    for (int i=0; i<(M-K1+1); i++)
    {
        for (int j=0; j<(N-K1+1); j++)
            printf("%f ", output1[i*(N-K1+1) + j]);
        
        printf("\n");
    }

    bi=1;
    float *kernel2 = malloc(K2 * K2 * sizeof *input);
    for ( int i = 0; i < K2; i++ )
        for ( int j=0; j < K2; j++ )
            bi = kernel2[i*K2 + j] = (bi==0) ? 1 : 0;
    
    printf("\n");
    for (int i=0; i<K2; i++)
    {
        for (int j=0; j<K2; j++)
            printf("%f ", kernel2[i*K2 + j]);
        
        printf("\n");
    }

    float *output2;
    single_layer_convolution(M-K1+1, N-K1+1, output1, K2, 
                             kernel2, &output2);

    printf("\n");
    for (int i=0; i<(M-K1-K2+2); i++)
    {
        for (int j=0; j<(N-K1-K2+2); j++)
            printf("%f ", output2[i*(N-K1-K2+2) + j]);
        
        printf("\n");
    }
    
    free(input);
    free(kernel1);
    free(kernel2);
    free(output1);
}
