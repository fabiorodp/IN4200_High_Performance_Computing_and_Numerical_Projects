// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// clang -Xpreprocessor -fopenmp integral_omp_for.c -lomp
// ./a.out

// SPMD technique *video11&12

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>
#include <omp.h>


int main()
{
    static long num_steps = 100000;
    double step;
    double pi, sum=0.0;
    step=1.0/(double)num_steps;

    #pragma omp parallel
    {
        double x;
        #pragma omp for reduction(+:sum)
            {
                for (int i = 0; i < num_steps; i++)
                {
                    x = (i + 0.5) * step;
                    sum += 4.0 / (1.0 + x * x);
                }
            }
    }

    pi = step * sum;
    printf("%f", pi);

    return 0;
}
