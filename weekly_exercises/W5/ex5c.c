// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

// compiling & running
// gcc ex5c.c
// ./a.out
// gcc w5_e3_optimize.c week5.h

#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf
#include <math.h>
#include <time.h>


void foo (int N, double** mat, double** s, int* v){
    int i, j;
    double val;

    for (j=0; j<N; j++)
        for (i=0; i<N; i++){
            val = 1.0*(v[j]%256);
            mat[i][j] = s[i][j] * (sin(val)*sin(val)-cos(val)*cos(val));
        }
}


void foo_optimal (int N, double** mat, double** s, int* v){
    int i, j, rest;
    double val, tmp;

    for (j=0; j<N; j++){
        val = v[j]%256;
        tmp = sin(val) * sin(val) - cos(val) * cos(val);
        rest = N%4;
        
        for (i=0; i<N-rest; i+=rest){
            mat[i][j] = s[i][j] * tmp;
            mat[i+1][j] = s[i+1][j] * tmp;
            mat[i+2][j] = s[i+2][j] * tmp;
            mat[i+3][j] = s[i+3][j] * tmp;
        }
    }
}


int main(int argc, char const *argv[])
{
    return 0;
}
