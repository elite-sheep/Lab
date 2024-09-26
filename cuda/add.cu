#include <stdio.h>

const int blockSize = 900;

__global__ void add(int* a, int* b, int* c) {
  c[threadIdx.x] = a[threadIdx.x] + b[threadIdx.x];
}

__global__ void vectorAdd(int* a, int* b, int n, int* out) {
  int sum = n * a[threadIdx.x] * b[threadIdx.x];
  atomicAdd(out, sum);
}

int main(void) {
  int a[blockSize];
  int b[blockSize];
  int c;
  int *d_a, *d_b, *d_c;
  int size = blockSize * sizeof(int);

  for (int i = 0; i < blockSize; ++i) {
    a[i] = 1;
    b[i] = 1;
  }

  cudaMalloc((void **)&d_a, size);
  cudaMalloc((void **)&d_b, size);
  cudaMalloc((void **)&d_c, sizeof(int));

  cudaMemset(d_c, 0, sizeof(int));

  cudaMemcpy(d_a, &a, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, &b, size, cudaMemcpyHostToDevice);

  add<<<1, blockSize>>>(d_a, d_b, d_b);
  vectorAdd<<<1, blockSize>>>(d_a, d_b, 5, d_c);

  cudaMemcpy(&c, d_c, sizeof(int), cudaMemcpyDeviceToHost);

  printf("%d\n", c);

  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_c);

  return 0;
}
