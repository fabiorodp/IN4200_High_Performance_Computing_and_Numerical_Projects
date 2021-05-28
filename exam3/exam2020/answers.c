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
EXERCISE 1.a:
            W1      W2      W3
    h1      t1.1    idle    idle
    h2      t2.1    t1.2    idle
    h3      t3.1    t.2.2   t.1.3
    h4      t4.1    t.3.2   t.2.3
    h5      t1.4    t.1.5   t.4.2
    h6      t3.3    t.2.4   t.1.5
    h7      t5.2    t.4.3   t.3.4
    h8      t2.5    t.5.3   t.4.4
    h9      t3.5    t.4.5   idle
    h10     t4.5    idle    idle
    h11     t5.5    idle    idle
*/

/*
EXERCISE 1.b:

*/

/*
EXERCISE 1.c:

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
EXERCISE 3.a.:
 OMP Directives and Clauses:
 - "#pragma omp parallel for" directive was used before the outermost for-loop "for (i=0; i<=N-n;
 i++)" because its iterations "i" are completely independent and possible to parallelize.
 - The private-clause is used because iterations "j", "ii" and "jj" are dependent on each other and
 not possible to parallelize without race conditions. Thus, we make these iterations private for
 each thread, fixing the race conditions and reducing OpenMP overhead.
 - The Collapse-clause is used because the 2 outermost for-loops "for (i=0; i<=N-n; i++)" and
 "for (j=0; j<=N-n; j++)" iterates in the same range, therefore can be fused in a unique for-loop
 and it might increase performance and reduce OpenMP overhead.
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
EXERCISE 3.b.:
 - Two (2) Intel Xeon 24-core processors of model 8168;
 - 4 Flops per clock cycle;
 - Sweep function: 2 loads, 1 store and 2 flops

 QUESTIONS & ANSWERS:
 - How would you estimate the theoretical minimum computing time needed by the function sweep
 when the values of N and n are known? Please elaborate your reasoning.
 >> The theoretical minimum computing time for the function sweep can be estimated by deriving
 two theoretical estimates Time_fp and Time_memory. The former looks at how fast the flops can
 be executed. The latter consider the time for memory traffic between the last-cache-level (L3)
 and the physical memory (RAM), where this communication is often a bottleneck for performance.
 We observe that there are 2 floating-point operations at sweep function, therefore:
 Time_fp = #flops * #iterations / #cores * #CPUs * #theoreticalFlops * CPUClockRate
 Time_memory = #L/S * #iterations * #bytes / #cores * #CPUs * #memoryChannels #memoryFrequency *
 #bytes
 Therefore, the theoretical minimum computing time is the maximum value between Time_fp and
 Time_memory.

 - In reality the actual computing time is longer than the theoretical minimum estimate.
 What can be the reasons for this performance discrepancy?
 >> The reasons of having the actual computing time different from the above theoretical estimate
 can include:
    (1) the realistic memory bandwidth achievable per CPU is considerably less than 6 · memory
    frequency · 8 (as can be found by the STREAM benchmark);
    (2) the L3 cache is not “perfectly” utilized (such as due to the m-way associativity);
    (3) the actual amount of memory traffic is larger than the theoretical estimate due to
    imperfect alignment (and/or imperfect use of the cache lines);
    (4) loop overhead associated with the ii- and jj-indexed loops;
    (5) imperfect load balancing between the threads.
*/

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
