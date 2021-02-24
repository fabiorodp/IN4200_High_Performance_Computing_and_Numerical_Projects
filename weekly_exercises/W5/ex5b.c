// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc ex5a.c
// ./a.out
// gcc w5_e2_unroll.c week5.h

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>


void alloc2D(double*** A, int n, int m){
    (*A) = calloc(n, sizeof(int*));
    for (int j = 0; j < m; j++)
        (*A)[j] = calloc(m, sizeof **A);
}


void mult(double** y2, double**A, double* B, int N){
    for (int j = 0; j < N; j++)
        for (int i = 0; i <= j; i++)
            (*y2)[j] += A[j][i]*B[i];
}


void mult_fast(double** y1, double**A, double* B, int N){
    int rest = N%4;
    for (int j = 0; j < N-rest; j+=4){
        for (int i = 0; i <= j; i++){
            (*y1)[j] += A[j][i]*B[i];
            (*y1)[j+1] += A[j+1][i]*B[i];
            (*y1)[j+2] += A[j+2][i]*B[i];
            (*y1)[j+3] += A[j+3][i]*B[i];
        }
        (*y1)[j+1] += A[j+1][j+1]*B[j+1];
        (*y1)[j+2] += A[j+2][j+1]*B[j+1];
        (*y1)[j+2] += A[j+2][j+2]*B[j+2];
        (*y1)[j+3] += A[j+3][j+1]*B[j+1];
        (*y1)[j+3] += A[j+3][j+2]*B[j+2];
        (*y1)[j+3] += A[j+3][j+3]*B[j+3];
    }
    
    for (int j = N-rest; j < N; j++)
        for (int i = 0; i <= j; i++)
            (*y1)[j] += A[j][i]*B[i];
}


int main(int argc, char const *argv[])
{
    int j, i, stride = 4, N=atoi(argv[1]);
    double* B = calloc(N, sizeof *B);
    double* y1 = calloc(N, sizeof *y1);
    double* y2 = calloc(N, sizeof *y2);
    double** A;
    alloc2D(&A, N, N);

    // Assigning values for A, B and y
    for (j = 0; j < N; j++){
        B[j] = 2;
        y1[j] = 0;
        y2[j] = 0;
        
        for (i = 0; i <= j; i++)
            A[j][i] = 2;
    }

    clock_t t0 = clock();
    mult_fast(&y1, A, B, N);
    clock_t t1 = clock();
    printf("Mult. fast took: %f ns\n", (double)10000*(t1-t0)/CLOCKS_PER_SEC);

    clock_t t2 = clock();
    mult(&y2, A, B, N);
    clock_t t3 = clock();
    printf("Mult. slow took: %f ns\n", (double)10000*(t3-t2)/CLOCKS_PER_SEC);
    
    printf("printing y1...\n");
    for (j = 0; j < N; j++)
        printf("%f ", y1[j]);
    
    printf("\nprinting y2...\n");
    for (j = 0; j < N; j++)
        printf("%f ", y2[j]);
    
    free(y1);
    free(y2);
    free(B);
    free(A);
    free(*A);

    return 0;
}
