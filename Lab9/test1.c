		#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 16

void my_function(int y);

// Global vars for data and text segments
static int glb;
static int glb_init = 1000;
static int tmp1 = 10;

int main()
{

    printf("Global Non-Init: %p\n", &glb);
    printf("Global Init: %p\n", &glb_init);
    printf("Static int: %p\n", &tmp1);
    int x;
    printf("Location of x: %p\n", &x);
    int i = 0;
    printf("Location of i: %p\n", &i);

    my_function(i);
    printf("Function: %p\n", my_function);
    pause();

    return 0;
}

void my_function(int y) {
  int new_x;
  printf("Location of new_x: %p\n", &new_x);
  char *data;
  // printf("Location of data: %p\n", data);
  printf("Location of data: %p\n", &data);
  data = malloc(SIZE);
  printf("Location of data: %p\n", data);
  new_x = y;

}
