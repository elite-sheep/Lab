#include <stdio.h>

__global__ void helloKernel(void) {
}

int main() {
  helloKernel<<<1,1>>>();
  printf("hello world.\n");
  return 0;
}
