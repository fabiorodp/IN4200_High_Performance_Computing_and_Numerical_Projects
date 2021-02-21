// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc ex4c.c
// ./a.out
// gcc w4_e3_matmult.c week4.h

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>


// Print matrix values.
void printMatrix2D(double **matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%f ", matrix[i][j]);  // matrix[i][j]
        
        printf("\n");
    }
}


int alloc2DRandomMatrix(double*** arr, int n, int m, int set_rand_value)
{
    printf("Allocating matrix...\n");

    (*arr) = (double**) calloc(n, sizeof(double*));

    for (int i = 0; i < n; i++)
        *(*arr+i) = (double*) calloc(m, sizeof(double));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (set_rand_value == 1)
                *(*(*arr+i)+j) = (double) rand() / (double) RAND_MAX;  // rand # between 0 and 1
            
            printf("%f ", *(*(*arr+i)+j) );
        }
        printf("\n");
    }

    if (!(*arr)[0] || !*arr) {
        // Allocation failed.
        printf("Allocation failed\n");
        return 1;
    }

    printf("Allocating done.\n\n");
    return 0;
}


void matmult(
    double*** arr1, double*** arr2, double*** arr_result, 
    int n, int m, int p)
{
    for (int i = 0; i < n; i++)
        for (int k = 0; k < p; k++)
            for (int j = 0; j < m; j++)
                *(*(*arr_result+i)+k) += *(*(*arr1+i)+j) * *(*(*arr2+j)+k);
}


void matmult_fast(
    double*** arr1, double*** arr2, double*** arr_result, 
    int n, int m, int p)
{
    int rest = p%4;
    for (int i = 0; i < n; i++)
        for (int k = 0; k < p-rest; k+=4) {
            for (int j = 0; j < m; j++) {
                *(*(*arr_result+i)+k) += *(*(*arr1+i)+j) * *(*(*arr2+j)+k);
                *(*(*arr_result+i)+k+1) += *(*(*arr1+i)+j) * *(*(*arr2+j)+k+1);
                *(*(*arr_result+i)+k+2) += *(*(*arr1+i)+j) * *(*(*arr2+j)+k+2);
                *(*(*arr_result+i)+k+3) += *(*(*arr1+i)+j) * *(*(*arr2+j)+k+3);
            }
        for (int k = p-rest; k < p; k++)
            for (int j = 0; j < m; j++)
                *(*(*arr_result+i)+k) += *(*(*arr1+i)+j) * *(*(*arr2+j)+k);
        }
}


int main(int argc, char const *argv[])
{
    // getting number of row and columns as arguments
    int n = atoi(argv[1]) , m = atoi(argv[2]), p = atoi(argv[3]);

    // allocating 2D matrices and setting random element values
    double **A, **B, **C, **D;
    srand(1);  // seeding random numbers
    alloc2DRandomMatrix(&A, n, m, 1);  // &A := calling by reference
    alloc2DRandomMatrix(&B, m, p, 1);
    alloc2DRandomMatrix(&C, n, p, 0);
    alloc2DRandomMatrix(&D, n, p, 0);

    // matrix multiplication C = A*B
    clock_t t0 = clock();
    matmult(&A, &B, &C, n, m, p);
    clock_t t1 = clock();
    double elapsed_matmult = (double) (t1-t0)/CLOCKS_PER_SEC;

    // printing results for the matrix C
    printMatrix2D(C, n, p);

    // matrix multiplication D = A*B
    clock_t t2 = clock();
    matmult_fast(&A, &B, &D, n, m, p);
    clock_t t3 = clock();
    double elapsed_matmult_fast = (double) (t2-t3)/CLOCKS_PER_SEC;

    // printing results for the matrix D
    printf("\n");
    printMatrix2D(D, n, p);

    printf("\n");
    printf("Total time matmult:     %f s\n", elapsed_matmult);
    printf("Total time matmultfast: %f s\n", elapsed_matmult_fast);
    printf("Speedup ratio: %f\n", elapsed_matmult/elapsed_matmult_fast);

    free(A);
    free(B);
    free(C);
    free(D);

    return 0;
}
