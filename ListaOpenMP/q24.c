#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void serial_func(int n) {
  int a[n];

  a[0] = 0;
  for (int i = 0; i < n; i++) {
    a[i] = a[i - 1] + i;
  }

  printf("Serial:\n");
  for (int i = 0; i < n; i++) {
    printf("%d\n", a[i]);
  }
}

void parallel_func(int n, int thread_count) {
  int a[n];

  a[0] = 0;
#pragma omp parallel num_threads(thread_count) default(none) shared(a, n)
  {
#pragma omp for
    for (int i = 0; i < n; i++) {
      a[i] = (i*(i+1))/2;
    }
  }

  printf("Parallel:\n");
  for (int i = 0; i < n; i++) {
    printf("%d\n", a[i]);
  }
}

int main(int argc, char *argv[]) {
  int n, thread_count;
  if (argc < 3) {
    printf("Usage: \n");
    printf("./q23 <Number of iteractions> <thread count>\n");
    return 1;
  }

  n = strtol(argv[1], NULL, 10);
  thread_count = strtol(argv[2], NULL, 10);

  serial_func(n);
  parallel_func(n, thread_count);

  return 0;
}