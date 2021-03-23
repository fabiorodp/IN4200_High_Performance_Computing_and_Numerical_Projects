// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// clang -Xpreprocessor -fopenmp integral_omp.c -lomp
// ./a.out

// SPMD technique *video7

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>
#include <omp.h>

#define NUM_THREADS 2

int main()
{
    static long num_steps = 100000;
    double step;
    int i, nthreads; double pi, sum[NUM_THREADS];

    step=1.0/(double)num_steps;
    omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
    {
        int i, id, nthrds;
        double x;

        // being sure that the compiler gave a correct num of threads
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0) nthreads = nthrds;

        for (i = id, sum[id] = 0.0; i < num_steps; i = i + nthrds) {
            x = (i + 0.5) * step;
            sum[id] += 4.0 / (1.0 + x * x);
        }
    }

    for (i=0, pi=0.0; i<nthreads; i++)
        pi+=sum[i]*step;

    printf("%f", pi);

    return 0;
}
