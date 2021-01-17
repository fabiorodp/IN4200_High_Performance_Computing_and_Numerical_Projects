// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no
// ./a.out

#include <stdio.h>
#include <stdlib.h>


void pointers_example (){
    int a, *p;
    p = &a;
    a = 100;
    *p = 5;

    // returns 5
    printf("Pointer *p has changed the variable a to %d \n", a);
}


void array_example (){
    int *p, i;
    p = malloc(3 * sizeof(int));
    *(p+0) = 2;
    *(p+1) = 4;
    *(p+2) = 6;

    if ( p == NULL){
        perror("Error: Memory not allocated.");
    };

    for ( i=0; i<3; ++i ) {
        printf("array[%d] = %d \n", i, p[i]);
    }
}


void exercise1a() {

    int a = 2;
    double limit = 1;
    char sign[] = "n";

    while (a < 1000002) {

        double denominator;

        if (sign[0] != 'p') {
            denominator = -2 ^ a;
            sign[0] = 'p';

        } else {
            denominator = +2 ^ a;
            sign[0] = 'n';
        }

        limit += 1 / denominator;
        a += 2;
    }

    if (limit == 4. / 5) {
        printf("The limit %f is 4/5. \n", limit);

    } else {
        printf("The limit %f is not 4/5. \n", limit);
    }
}


void exercise1b(int n, int seed) {

    /* local variables */
    int array[n], i, max, min;

/*  printf("Enter size of the array : ");
    scanf("%d",&n);*/

    /* setting the seed */
    srand(seed);

    for ( i = 0; i < n; ++i) {
        array[i] = rand();
        printf( "array[%d] = %d \n", i, array[i]);
    }

    min=max=array[0];
    for(i=0; i<n; i++){

        if(min > array[i])
            min = array[i];

        if(max < array[i])
            max = array[i];
    }

    printf("minimum of array is : %d \n", min);
    printf("maximum of array is : %d \n", max);
}


int exercise1c(int m, int n, int seed){

    int i, j, array[m][n];

    /* setting the seed */
    srand(seed);

    for ( i=0; i<m; ++i ) {
        for ( j=0; j<n; ++j ) {
            array[i][j] = i+j;
        }
    }

    printf("%d \n", array[0][0]);

    return 0;
}

int main(){
    pointers_example();  // returns 5
    array_example();  // returns array
    exercise1a();
    exercise1b(3, 10);
    // exercise1c();
}
