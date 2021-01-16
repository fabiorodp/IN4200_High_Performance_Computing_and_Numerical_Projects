#include <stdio.h>
#include <float.h>

int main() {
  printf("Storage size for double : %lu \n", sizeof(double));
  printf("Minimum float positive value: %E\n", DBL_MIN );
  printf("Maximum float positive value: %E\n", DBL_MAX );
  printf("Precision value: %d\n", DBL_DIG );
   
  return 0;
}
