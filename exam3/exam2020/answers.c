// compiling & running
// clang -Xpreprocessor -fopenmp answers.c -lomp
// ./a.out

#include <stdlib.h> // rand, malloc, calloc, realloc and free.
#include <stdio.h>  // printf, scanf
#include <mpi.h>
#include <math.h>
#include <omp.h>
#include "string.h"
#include <mpi.h>

/*
EXERCISE 1:

*/

/*
EXERCISE 2:

*/

/*
EXERCISE 3:

 - All the three 2D arrays table1, mask and table2 are allocated beforehand, where table1 is of
 dimension N × N, mask of dimension n × n, and table2 of dimension (N−n+1)×(N−n+1).

 - It can be assumed that N is much larger than n. (For example, N is at least 10000 whereas n is
 at most 10.)

*/
double** sweep(int N, double **table1, int n, double **mask, double**table2,
               int verbose, double *time)
{
    int i,j,ii,jj;
    double temp;

    if ( verbose ) printf("Computing array values...\n");

    double t0 = omp_get_wtime();
    for (i=0; i<=N-n; i++)
        for (j=0; j<=N-n; j++)
        {
            temp = 0.0;
            for (ii = 0; ii < n; ii++)
                for (jj = 0; jj < n; jj++)
                    temp += table1[i + ii][j + jj] * mask[ii][jj];
            table2[i][j] = temp;
        }
    double t1 = omp_get_wtime();

    if ( verbose == 2 )
    {
        for ( i=0; i<=N-n; i++ )
        {
            for ( j=0; j<=N-n; j++ )
                printf("%f ", table2[i][j]);

            printf("\n");
        }
    }
    (*time) = t1-t0;
    return table2;
}

/*
collapse-clause:
*/
double** sweep_omp(int N, double **table1, int n, double **mask, double**table2,
                   int verbose, double *time)
{
    int i,j,ii,jj;
    double temp;

    if ( verbose ) printf("Computing array values...\n");

    double t0 = omp_get_wtime();

#pragma omp parallel for private(j, ii, jj, temp) collapse(2)
    for (i=0; i<=N-n; i++)
        for (j=0; j<=N-n; j++)
        {
            temp = 0.0;
            for (ii = 0; ii < n; ii++)
                for (jj = 0; jj < n; jj++)
                    temp += table1[i + ii][j + jj] * mask[ii][jj];
            table2[i][j] = temp;
        }

    double t1 = omp_get_wtime();

    if ( verbose == 2 )
    {
        for ( i=0; i<=N-n; i++ )
        {
            for ( j=0; j<=N-n; j++ )
                printf("%f ", table2[i][j]);

            printf("\n");
        }
    }

    (*time) = t1-t0;
    return table2;
}

/*
EXERCISE 4:

*/

/*
EXERCISE 5:

*/

int main( int argc, char **argv )
{
    // global variables
    int i, j, N=10000, n=10;

    // ########### allocating 2D array point-to-point arrays ###########

    double **table1 = (double **)malloc( N * sizeof(double *) );
    double **mask = (double **)malloc( n * sizeof(double *) );
    double **table2 = (double **)malloc( (N-n+1) * sizeof(double *) );

    for ( i=0; i<N; i++ )
        table1[i] = (double *)malloc(N * sizeof(double));

    for ( i=0; i<n; i++ )
        mask[i] = (double *)malloc(N * sizeof(double));

    for ( i=0; i<=N-n; i++ )
        table2[i] = (double *)malloc(N * sizeof(double));

    // ########### done ###########

    // ########### assigning values ###########

    for ( i = 0; i < N; i++ )
        for ( j = 0; j < N; j++ )
            table1[i][j] = 10.0;  // *(*(table1+i)+j)=10.0;

    for ( i = 0; i < n; i++ )
        for ( j = 0; j < n; j++ )
            mask[i][j] = 0.1;

    // ########### done ###########

    double serialized_time, parallelized_time;
    double **result1, **result2;
    result1 = sweep(N, table1, n, mask, table2, 1, &serialized_time);
    result2 = sweep_omp(N, table1, n, mask, table2, 1, &parallelized_time);
    printf("Serialized time = %f s.\n", serialized_time);
    printf("Parallelized time = %f s.\n", parallelized_time);
    printf("Speedup = %f.\n", serialized_time/parallelized_time);

    int isEqual = 1;
    for ( i = 0; i < n; i++ )
        for ( j = 0; j < n; j++ )
            if ( result1[i][j] != result2[i][j] ) isEqual=0;

    ( isEqual ) ? printf("Arrays are equal!") : printf("Arrays are NOT equal!");

    return 0;
}
