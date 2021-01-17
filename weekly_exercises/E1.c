// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no
// ./a.out

#include <stdio.h>
#include <stdlib.h>


int exercise1a() {

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
        printf("The limit %f is 4/5.\n", limit);

    } else {
        printf("The limit %f is not 4/5.\n", limit);
    }

    return 0;
}


int exercise1b(int n, int seed) {

    /* local variables */
    int array[n], i, max, min;

/*    printf("Enter size of the array : ");
    scanf("%d",&n);*/

    /* setting the seed */
    srand(seed);

    for ( i = 0; i < n; ++i) {
        array[i] = rand();
        printf( "array[%d] = %d\n", i, array[i]);
    }

    min=max=array[0];
    for(i=0; i<n; i++){

        if(min > array[i])
            min = array[i];

        if(max < array[i])
            max = array[i];
    }

    printf("minimum of array is : %d", min);
    printf("\nmaximum of array is : %d \n", max);

    return 0;
}


int exercise1c(){
    return 0;
}


int main(){
    exercise1b( 10, 11);
}
