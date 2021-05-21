// compiling & running
// clang -Xpreprocessor -fopenmp answers.c -lomp
// ./a.out

// mpicc -o answer.c
// mpirun -np 2 ./a.out

#include <stdlib.h> // rand, malloc, calloc, realloc and free.
#include <stdio.h>  // printf, scanf
#include <mpi.h>
#include <math.h>
#include <omp.h>
#include "string.h"
#include <mpi.h>

#define PI 3.14159265
#define EXP 2.71828

double power(double x, int n)
{
    for ( int i = 1; i <= n; ++i )
        x *= x;

    return x;
}

double factorial(double fact, int n)
{
    for ( int i = 1; i <= n; ++i )
        fact *= i;

    return fact;
}

double sine_taylor(double x,int n)
{
    double sine = x;
    int isNeg;
    isNeg = 1;
    for ( double i = 3; i <= n; i+=2 )
    {
        if(isNeg)
        {
            sine -= power(x,i)/factorial(i, n);
            isNeg = 0;
        }
        else
        {
            sine += power(x,i)/factorial(i, n);
            isNeg = 1;
        }
    }
    return sine;
}

/*
EXERCISE 1:

 We have to notice from the given code that the for-loops are independent of each other,
 but they were formulated together in not an economical way. Since the first and second for-loops
 are not dependent on each other, they should be detached. Indeed, if the given architecture in
 the exercise is applied, the code will be slow because the second (nested) for-loop is computed
 n times for the first for-loop times n times for the second for-loop, totalizing n*n calculations.
 If we split the for-loop, the second for-loop will only be computed n times, saving a lot
 of computer power and bringing more efficiency for the code.

 In addition, the second for-loop is formulated in a column-major looping order, where C[j][i] is
 being assigned values according to 'i' and 'j' order, which causes cache misses and compromises
 performance. Therefore, it would be preferable 'j' and 'i' order, row-major order, for better
 performance in C programming.

 Also, 'exp' and 'sin' functions are expensive not preferable because they are known as slow. It
 would be better to make the calculations with simple floating-point operations.

 Finally, it is possible to implement a for-loop unrolling in the second for-loop, which brings
 more efficiency for large operations.

 Output from the suggested code:

        % clang -Xpreprocessor -fopenmp answers.c -lomp
        % ./a.out

        Given results from the exercise:
        c[0]=1 c[1]=1 c[2]=1 c[3]=2 c[4]=2 c[5]=2 c[6]=2 c[7]=2 c[8]=2 c[9]=2
        2 2 2 2 2 2 2 2 2 2
        2 2 2 2 2 2 2 2 2 2
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        Wall-time for given code: 0.000020

        My version results from the exercise:
        c[0]=1 c[1]=1 c[2]=1 c[3]=2 c[4]=2 c[5]=2 c[6]=2 c[7]=2 c[8]=2 c[9]=2
        2 2 2 2 2 2 2 2 2 2
        2 2 2 2 2 2 2 2 2 2
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        1 1 1 1 1 1 1 1 1 1
        Wall-time for my code: 0.000001
        Speedup: 21.000000
*/
void exercise1()
{
    int n=10, c[10], a[10][10];
    int b[10][10]={{1,1,1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1,1,1}};
    int d[10]={1,1,1,1,1,1,1,1,1,1};
    int e[10]={1,1,1,1,1,1,1,1,1,1};

    double t0 = omp_get_wtime();
    for ( int i=0; i<n; i++ )
    {
        c[i] = exp(1.0*i/n) + sin(3.1415926*i/n);

        for ( int j=0; j<n; j++ )
            a[j][i] = b[j][i] + d[j]*e[i];
    }
    double t1 = omp_get_wtime();

    printf("Given results from the exercise:\n");

    for ( int i=0; i<n; i++ )
        printf("c[%d]=%d ", i, c[i]);

    printf("\n");
    for ( int i=0; i<n; i++ )
    {
        for ( int j=0; j<n; j++ )
            printf("%d ", a[i][j]);

        printf("\n");
    }
    printf("Wall-time for given code: %f\n", t1-t0);

    // my version
    int n1=10, c1[10], a1[10][10];
    int b1[10][10]={{1,1,1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1,1,1}};
    int d1[10]={1,1,1,1,1,1,1,1,1,1};
    int e1[10]={1,1,1,1,1,1,1,1,1,1};

    double t2 = omp_get_wtime();
//    for ( int i=0; i<n1; i++ )
//        c1[i] = exp(1.0*i/n1) + sin(3.1415926*i/n1);  // possible to increase performance here
//        // c1[i] = EXP*(1.0*i/n1) + sine_taylor(PI*i/n1, 2);  // not need a high n; (slower)

    double ev, exp_div_n=exp(1.0/n1), pi_div_n=3.1415926/n1;
    c1[0] = 0.;
    for ( int i = 1; i<=(n/2); i++ )
        c1[i] = sin(i*pi_div_n);

    for ( int i = (n/2)+1; i<n; i++ )
        c1[i] = c1[n1-i]; // taking advantage of sin(pi-x)=sin(x)

    // exp(1.0*i/n) = exp(1.0/n)^i
    ev = 1.0;
    for ( int i = 0; i<n; i++)
    {
        c1[i] += ev;  //
        ev *= exp_div_n;
    }

    for ( int i=0; i<n1; i+=5 )
        for ( int j=0; j<n1; j++ )
        {
            a1[i][j] = b1[i][j] + d1[i]*e1[j];
            a1[i+1][j] = b1[i+1][j] + d1[i+1]*e1[j];
            a1[i+2][j] = b1[i+2][j] + d1[i+2]*e1[j];
            a1[i+3][j] = b1[i+3][j] + d1[i+3]*e1[j];
            a1[i+4][j] = b1[i+4][j] + d1[i+4]*e1[j];
            a1[i+5][j] = b1[i+5][j] + d1[i+5]*e1[j];
        }

    // remainder
    if ( n%5 != 0 )
        for ( int i = n-(n%5); i<n1; i++ )
            for ( int j = 0; j<n1; j++ )
                a1[i][j] = b1[i][j] + d1[i]*e[j];

    double t3 = omp_get_wtime();

    printf("My version results from the exercise:\n");
    for ( int i=0; i<n; i++ )
        printf("c[%d]=%d ", i, c[i]);

    printf("\n");
    for ( int i=0; i<n; i++ )
    {
        for ( int j=0; j<n; j++ )
            printf("%d ", a[i][j]);

        printf("\n");
    }

    printf("Wall-time for my code: %f\n", t3-t2);
    printf("Speedup: %f\n", (t1-t0) / (t3-t2));
}

/*
EXERCISE 2:

 Assume:
 - 40GB/s of memory bandwidth (performance of the paths between memory and cache);
 - 100 GFLOP/s (floating-point operations per second in double precision DP);

 Theory observations:
 - Latency or depth:
 - Wall-clock = elapsed time;
 - CPU time does not encompass contributions from I/O, context switches, other processes.
 - Speedup: Time_seq / Time_parallel

 - MACHINE BALANCE (B_m) = possible memory bandwidth[GWords/s] (b_max) /
                           peak performance[GFlops/s] (P_max)
    -- where
        --- b_m = GB/s / bytes (if double-precision, it occupies 8 bytes in memory) = GW/s
        --- P_max = #cores * #GHz * flops per cycle = GFlops/s

 - CODE BALANCE (B_c) = (#loads + #stores) / #floatingPointOperations (flops)
 - LIGHTSPEED RATIO = B_m / B_c of peak_max

 - If B_c is larger than B_m, then the speed of the computation is determined by the memory traffic:
    -- MEMORY TRAFFIC TIME: (#iterations * #bytesInMemory) /
                            (theoreticalMemoryBandwidth (GB/s) * (#iterations/peak
                             performance[GFlops/s]) )
 Question:
 - What is the CPU time for n = 10^10 when the following code run?

 double s = 0.0;
 for (i=0; i<n; i++)
     s += a[i]*a[i];
     // loads 1 value for 'a';
     // assumes that 's' is in register, so no cache traffic during the iterations;
     // 2 floating-points operations.

B_m = (40/8) / 100 = 0.05
B_c = 1 / 2 = 0.5
MT_t = (10^10 * 8) / (40 * (10^10 / 10*2))
*/

/*
EXERCISE 3:
 - Dependency between 16 tasks;
 - All the 16 tasks are equally time-consuming, requiring one hour of a worker.
 - 3 workers.

Question:
 -  How many hours minimum do 3 workers need to finish all the 16 tasks?

Table:
 Hour/Worker
    1         2         3
 1  t1,1      idle      idle
 2  t1,2      t2,1      idle
 3  t1,3      t2,2      t3,1
 4  t1,4      t2,3      t3,2
 5  t4,1      t2,4      t3,3
 6  t4,2      t3,4      idle
 7  t4,3      idle      idle
 8  t4,4      idle      idle
*/

/*
EXERCISE 4:

 Function:
    for ( int i = 0; i<n-1; i++ )
        u[i] = func(u[i+1]);

 The function above can not be parallelized with "#pragma omp parallel for" because of dependencies
 between the looping iterations. Indeed, the iteration step 'i' is dependent to step 'i+1', thus
 it is not guaranteed that the task will be split between threads properly. However, with the
 following modification, introduction of a helper array v, it can be parallelized as wanted:

 double *v = malloc( (n-1)* sizeof *v);

 # pragma omp parallel for
     for ( int i = 0; i<n-1; i++ )
        v[i] = (double)func(u[i+1]);

     for ( int i = 0; i<n-1; i++ )
        u[i] = v[i];

 free(v);
*/

/*
EXERCISE 5:
 False sharing is one of the possible reason for a bad scalability of a parallel program.

 FALSE SHARING: It happens in NUMA and UMA systems when 2 or more OMP threads load the same
 cache line from memory to their private cache. Then, these threads modify variables that reside
 on the same cache line. This invalidates the cache line and forces a memory update to maintain
 cache coherency, compromising the performance of the program.

 Example:
 https://software.intel.com/content/www/us/en/develop/articles/avoiding-and-identifying-false-sharing-among-threads.html

 Others problems with scalability may be communication overhead, synchronization loss, NUMA
 locality, bandwidth bottlenecks and load imbalance.

 LOAD IMBALANCE: This issue occurs when synchronization point are reached earlier by some workers
 than to others, in other words, the work distribution was not efficiently done. This problem
 reflects that some workers that could have been in use is actually in idle waiting for a task. As
 consequence, computer resources are not utilized.

 COMMUNICATION OVERHEAD: It happens when the data volume to be communicated is not proportional
 to the overall area of the domain cuts. Good practice always tries to reduce boundary area
 as far as possible for a minimum number of overheads (communications). A disproportionate number
 of overheads compromises efficiency and is not preferable. Also, we need to be aware of the
 locality of the data dependencies because the communication cost increases linearly with the
 distance that it was to bridge to the CPU cache.

*/

/*
EXERCISE 6:

    OUTPUT:
     On rank 0, own_value=12
     On rank 1, own_value=16
     On rank 2, own_value=12
     On rank 3, own_value=16
     On rank 4, own_value=12
     On rank 5, own_value=16
     On rank 6, own_value=12
     On rank 7, own_value=16
*/

/*
EXERCISE 7:
*/
int count_occurrence(const char *text_string, const char *pattern)
{
    int i, count=0;
    size_t lenStr1 = strlen(text_string);
    size_t lenStr2 = strlen(pattern);

    for ( i = 0; i <= (lenStr1-lenStr2); i++ )
        if ( strncmp(&text_string[i], pattern, lenStr2) == 0 )
            count++;

    return count;
}

/*
EXERCISE 8:
*/
int count_occurrence_omp(const char *text_string, const char *pattern)
{
    int i, count=0;
    size_t lenStr1 = strlen(text_string);
    size_t lenStr2 = strlen(pattern);

#pragma omp parallel for reduction(+ : count)
    for ( i = 0; i <= (lenStr1-lenStr2); i++ )
        if ( strncmp(&text_string[i], pattern, lenStr2) == 0 )
            count++;

    return count;
}

int main( int argc, char **argv )
{
    exercise1();

    const char text_string[] = "ATTTGCGCAGACCTAAGCA";
    const char pattern[] = "GCA";

    double t0 = omp_get_wtime();
    int count1 = count_occurrence(text_string, pattern);
    double t1 = omp_get_wtime();

    double t2 = omp_get_wtime();
    int count2 = count_occurrence_omp(text_string, pattern);
    double t3 = omp_get_wtime();

    printf("\nCount1=%d and time=%f\n", count1, t1-t0);
    printf("Count2=%d and time=%f\n", count2, t3-t2);
    printf("Speedup = %f\n", (t1-t0) / (t3-t2));

    return 0;
}
