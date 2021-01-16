#include <stdio.h>
#include <float.h>

int main() {
  printf("Storage size for long double : %lu \n", sizeof(long double));
  printf("Minimum float positive value: %LE\n", LDBL_MIN );
  printf("Maximum float positive value: %LE\n", LDBL_MAX );
  printf("Precision value: %d\n", LDBL_DIG );
   
  return 0;
}
