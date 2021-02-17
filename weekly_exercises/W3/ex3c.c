// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc ex3c.c
// ./a.out

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    float x = atoi(argv[1]);
    int power = atoi(argv[2]);
    double result = 1;

    // Exercise 3
    
    if (power == 0)
    {
        result = 1;
    }

    // calculating the power of a number
    clock_t t0 = clock();
    for (int i = 1; i <= power; ++i) 
        result *= x;
    clock_t t1 = clock();

    double elapsed_time = (double)(t1 - t0)/CLOCKS_PER_SEC;
    printf(
        "My script takes %f s to calculate %f.\n", elapsed_time, result
    );

    // calculating the running time of the pow function
    clock_t t2 = clock();
    float result2 = pow(atoi(argv[1]), power);
    clock_t t3 = clock();

    double elapsed_time2 = (double)(t3 - t2)/CLOCKS_PER_SEC;
    printf(
        "POW script takes %f s to calculate %f.\n", elapsed_time2, result2
    );

    return 0;
};
