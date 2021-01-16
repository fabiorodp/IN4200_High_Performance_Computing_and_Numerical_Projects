#include <stdio.h>

// function declaration
int *getMax(int *, int *);

int main(void) {
    
  // integer variables
  int x = 100;
  int y = 200;
  
  // pointer variable
  int *max = NULL;
  
  /**
   * get the variable address that holds the greater value
   * for this we are passing the address of x and y
   * to the function getMax()
   */
  max = getMax(&x, &y);
  
  // print the greater value
  printf("Max value: %d\n", *max);
  
  return 0;
}

// function definition
int *getMax(int *m, int *n) {

  /**
   * if the value pointed by pointer m is greater than n
   * then, return the address stored in the pointer variable m
   */
  if (*m > *n) {
    return m;
  }
  /**
   * else return the address stored in the pointer variable n
   */
  else {
    return n;
  }
  
}
