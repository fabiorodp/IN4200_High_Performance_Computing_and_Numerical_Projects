// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// clang -Xpreprocessor -fopenmp helloworld.c -lomp
// ./a.out


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>
#include <omp.h>


int main()
{
#pragma omp parallel
    {
    int ID = omp_get_thread_num();
    printf("hello(%d)", ID);
    printf(" world(%d)\n", ID);
    }
}

// race condition
//>>hello(3) world(3)
//>>hello(2) world(2)
//>>hello(1) world(1)
//>>hello(0) world(0)
