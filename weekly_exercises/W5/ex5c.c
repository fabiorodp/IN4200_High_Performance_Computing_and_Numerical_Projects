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


void alloc2D(double*** A, int n, int m){
    (*A) = calloc(n, sizeof **A);
    for (int i = 0; i < m; i++){
        (*A)[i] = calloc(m, sizeof ***A);
    }
}


void foo(double*** mat, double*** s, int** v, int n, int m){
    double val;
    for (int j = 0; j < n; j++)
        for (int i = 0; i < m; i++){
            val = 1.0*((*v)[j]%256);
            (*mat)[i][j] = (*s)[i][j] * (sin(val)*sin(val)-cos(val)*cos(val));
        }
}


void foo_optimal_col(double*** mat, double*** s, int** v, int n, int m){
    int rest = n%4;
    double tmp, cos_;

    for (int j = 0; j < n; j++){
        // sin^2(x)-cos^2(x) = 1 - 2*cos^2(x)
        cos_ = cos((*v)[j]%256);
        tmp = 1.0 - 2.0 * cos_ * cos_;

        for (int i = 0; i < m-rest; i += rest){
            (*mat)[i][j] = (*s)[i][j] * tmp;
            (*mat)[i+1][j] = (*s)[i+1][j] * tmp;
            (*mat)[i+2][j] = (*s)[i+2][j] * tmp;
            (*mat)[i+3][j] = (*s)[i+3][j] * tmp;
        }

        for (int i = rest; i < m; i++)
            (*mat)[i][j] = (*s)[i][j] * tmp;

    }
}


void foo_optimal_row(double*** mat, double*** s, int** v, int n, int m){
    int rest = n%4;
    double tmp, cos_;
    
    for (int i = 0; i < n-rest; i += rest){
        for (int j = 0; j < m; j++){
            // sin^2(x)-cos^2(x) = 1 - 2*cos^2(x)
            cos_ = cos((*v)[j]%256);
            tmp = 1.0 - 2.0 * cos_ * cos_;
            
            (*mat)[i][j] = (*s)[i][j] * tmp;
            (*mat)[i+1][j] = (*s)[i+1][j] * tmp;
            (*mat)[i+2][j] = (*s)[i+2][j] * tmp;
            (*mat)[i+3][j] = (*s)[i+3][j] * tmp;
        }
    }

    for (int i = rest; i < n; i++)
        for (int j = 0; j < m; j++){
            // sin^2(x)-cos^2(x) = 1 - 2*cos^2(x)
            cos_ = cos((*v)[j]%256);
            (*mat)[i][j] = (*s)[i][j] * (1.0 - 2.0 * cos_ * cos_);
        }
}


void foo_cos_vect(double*** mat, double*** s, int** v, int n, int m){
    int rest=m/4, i, j;
    double *val = malloc(m * sizeof *val), cos_;

    for (j = 0; j < n; j++){
        cos_ = cos((*v)[j]%256);
        val[j] = 1.0 - 2.0 * cos_ * cos_;
    }

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            (*mat)[i][j] = (*s)[i][j] * val[j];

    free(val);
}


int main(int argc, char const *argv[])
{
    int N = atoi(argv[1]);

    double** mat1;
    double** mat2;
    double** mat3;
    double** mat4;
    double** s;
    int* v = calloc(N, sizeof *v);

    alloc2D(&mat1, N, N);
    alloc2D(&mat2, N, N);
    alloc2D(&mat3, N, N);
    alloc2D(&mat4, N, N);
    alloc2D(&s, N, N);

    // assigning random values for s and v
    for (int i = 0; i < N; i++){
        v[i] = (double)rand()/RAND_MAX;

        for (int j = 0; j < N; j++){
            s[i][j] = (double)rand()/RAND_MAX;
        }
    }

    clock_t t0 = clock();
    foo(&mat1, &s, &v, N, N);
    clock_t t1 = clock();

    clock_t t2 = clock();
    foo_optimal_col(&mat2, &s, &v, N, N);
    clock_t t3 = clock();

    clock_t t4 = clock();
    foo_optimal_row(&mat3, &s, &v, N, N);
    clock_t t5 = clock();

    clock_t t6 = clock();
    foo_cos_vect(&mat4, &s, &v, N, N);
    clock_t t7 = clock();

    double elapsed_1 = (double) 10000*(t1-t0)/CLOCKS_PER_SEC;
    double elapsed_2 = (double) 10000*(t3-t2)/CLOCKS_PER_SEC;
    double elapsed_3 = (double) 10000*(t5-t4)/CLOCKS_PER_SEC;
    double elapsed_4 = (double) 10000*(t7-t6)/CLOCKS_PER_SEC;
    printf("\nWithout opt.: %f ns\n", elapsed_1);
    printf("Column waise opt.: %f ns\n", elapsed_2);
    printf("Row waise opt.: %f ns\n", elapsed_3);
    printf("Cos. vect. opt.: %f ns\n", elapsed_4);

    int result = (
        **mat1==**mat2 && **mat1==**mat3 && **mat1==**mat4
        && mat1[1][0]==mat2[1][0] && mat1[1][0]==mat3[1][0] && mat1[1][0]==mat4[1][0]
        && mat1[1][2]==mat2[1][2] && mat1[1][2]==mat3[1][2] && mat1[1][2]==mat4[1][2]) ? 1 : 0;
    printf("\nAre mat1, mat2, mat3 and mat4 equal? (0:False, 1:True): %d\n", result);

    free(*mat1);
    free(mat1);
    free(*mat2);
    free(mat2);
    free(*mat3);
    free(mat3);
    free(*mat4);
    free(mat4);
    free(*s);
    free(s);
    free(v);

    return 0;
}
