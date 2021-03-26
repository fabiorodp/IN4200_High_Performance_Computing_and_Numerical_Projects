// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no


#include <stdlib.h> // rand, malloc, calloc and free.
#include <stdio.h>  // printf, scanf


void printing_SNN_table(int **SNN_table, int N)
{
    char ipt = 'n';
    printf("Please, type 'y' if you want to print SNN_table here:");
    scanf("%c", &ipt);

    if (ipt=='y')
        printf("Printing SNN_table...\n");
        for (int i = 0; i < N; i++)
        {
            for (int j=0; j < N; j++)
                printf("%d ", SNN_table[i][j]);

            printf("\n");
        }
}


void comparing_2D_tables(int **SNN_table, int **SNN_table_omp, int N)
{
    int is_equal = 1;
    for (size_t i = 0; i < N; i++)
        for (size_t j=0; j < N; j++)
            if (SNN_table[i][j] != SNN_table_omp[i][j]) is_equal=0;

    printf(is_equal==1 ? ">> True\n" : ">> False\n");
}


void printing_SNN_val(int *SNN_val, int length)
{
    char ipt = 'n';
    printf("Please, type 'y' if you want to print SNN_val here:");
    scanf("%c", &ipt);

    if (ipt=='y')
    {
        // testing returned values for SNN_val
        printf("Printing SNN_val...\n");
        for ( size_t i = 0; i < length; i++)
            printf("%d ", SNN_val[i]);
    }
}


void comparing_SNN_val(int *SNN_val, int *SNN_val_omp, int length)
{
    int is_equal = 1;
    for ( size_t i = 0; i < length; i++)
        if (SNN_val[i] != SNN_val_omp[i]) is_equal=0;

    printf(is_equal==1 ? ">> True\n" : ">> False\n");
}
