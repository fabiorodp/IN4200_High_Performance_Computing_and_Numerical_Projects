// Author: Fabio Rodrigues Pereira
// E-mail: fabior@uio.no

#include <stdio.h>

int exercise1() {

    int a = 2;
    double limit = 1;
    char sign[] = "n";

    while (a < 1000000000) {

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
        printf("The limit %f is correct!\n", limit);

    } else {
        printf("The limit %f is wrong!\n", limit);
    }

    return 0;
};

int main(){
    exercise1();
}
