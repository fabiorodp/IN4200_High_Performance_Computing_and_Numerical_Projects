// compiling & running
// mpicc -o MPI_main MPI_main.c
// mpirun -np 2 ./MPI_main 6 5 3 2


#include <stdlib.h> // rand, malloc, calloc, realloc and free.
#include <stdio.h>  // printf, scanf
#include <mpi.h>
#include <math.h>

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
Exercise 1:

 We have to notice from the given code that the for-loops are independent of each other,
 but they were formulated together. Since the first and second for-loops are not dependent
 on each other, they should be detached. Indeed, if the given architecture in the exercise
 is applied, the code will be slow because the second (nested) for-loop is computed n times
 for the first for-loop times n times for the second for-loop, totalizing n*n calculations.
 If we split the for-loop, the second for-loop will only be computed n times, saving a lot
 of computer power and bringing more efficiency for the code.

 In addition, the second for-loop is formulated in a column-major looping, where C[j][i] is
 being assigned values according to 'i' and 'j' order, which would be preferable 'j' and 'i'
 order for better performance in C programming.

 Finally, 'exp' and 'sin' functions are not preferable because they are known as slow. It would
 be better to make the calculations straight inside the for-loop.
*/
void exercise1()
{
    int n=5, c[5], a[5][5];
    int b[5][5]={{1,1,1,1,1}, {1,1,1,1,1}};
    int d[5]={1,1,1,1,1};
    int e[5]={1,1,1,1,1};

    for ( int i=0; i<n; i++ )
    {
        c[i] = exp(1.0*i/n) + sin(3.1415926*i/n);

        for ( int j=0; j<n; j++ )
            a[j][i] = b[j][i] + d[j]*e[i];
    }

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

    // my version
    int n1=5, c1[5], a1[5][5];
    int b1[5][5]={{1,1,1,1,1}, {1,1,1,1,1}};
    int d1[5]={1,1,1,1,1};
    int e1[5]={1,1,1,1,1};

    for ( int i=0; i<n1; i++ )
        c1[i] = EXP*(1.0*i/n1) + sine_taylor(PI*i/n1, 1000);

    for ( int i=0; i<n1; i++ )
        for ( int j=0; j<n1; j++ )
            a1[i][j] = b1[i][j] + d1[i]*e1[j];

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

}

/*
Exercise 2:

 Assume:
 - 40GB/s of memory bandwidth (performance of the paths between memory and cache);
 - 100 GFLOP/s (floating-point operations per second in double precision DP);

 Theory observations:
 - Latency or depth:
 - Wallclock = elapsed time;
 - CPU time does not emcompass contributions from I/O, context switches, other processes.
 - Speedup: Time_seq / Time_parallel
 - Throughput:
 - Number of bus transactions (cache line transfers):
 - Number of loads and stores:
 - Number of floating-point operations:
 - Machine Balanca BM = possible memory bandwidth[GWords/s] (B_max) /
                        peak performance[GFlops/s] (P_max)

                        ***page 63***

 Question:
 - What is the CPU time for n = 10^10 when the following code run?

 double s = 0.0;
 for (i=0; i<n; i++)
     s += a[i]*a[i];

*/
void exercise2()
{
}

int main( int argc, char **argv )
{
    exercise1();

    return 0;
}
