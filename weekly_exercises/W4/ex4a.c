// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc ex4a.c
// ./a.out

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>

int main()
{
    double arrA, arrB;
    int N = 100, s = 10, i;
    double *A = malloc(N * sizeof(double));
    double *B = calloc(N, sizeof(double));

    // Assigning some random numbers:
    srand(1); // Seeds the random number generator.

    // assigning random values to array B
    for (i = 0; i < N; ++i)
    {
        B[i] = (double)rand();
    } 

    clock_t t0 = clock();
    for (int i = 0; i < N ; ++i)
    {
        A[i] = s*B[i];
        //printf("%f\n", *(A+i));
    }
    clock_t t1 = clock();
    
    double elapsed_time = (double)(t1 - t0)/CLOCKS_PER_SEC;
    printf("Elapsed time: %lf ms\n", elapsed_time);

    double bytes = 2 * N * sizeof(*A);
    printf("Bytes per sec: %f\n", bytes/elapsed_time);
    printf("Flops per sec: %f\n", N/elapsed_time);

    free(A);
    free(B);

    return 0;
};
