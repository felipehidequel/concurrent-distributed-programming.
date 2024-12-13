#include <omp.h>
#include <stdio.h>
// #include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int n, thread_count, i;
  int rank;
  if (argc < 3) {
    printf("Usage: \n");
    printf("./q23 <Number of iteractions> <thread count>\n");
    return 1;
  }

  n = strtol(argv[1], NULL, 10);
  thread_count = strtol(argv[2], NULL, 10);
  int *iteractions = (int *)malloc(sizeof(int) * n);

#pragma omp parallel num_threads(thread_count) default(none) private(          \
    i,rank) shared(n,iteractions)
  {

#pragma omp for
    for (i = 0; i < n; i++) {
      rank = omp_get_thread_num();
      iteractions[i] = rank;
    }
  }

  for (int i = 0; i < n; i++) {
    printf("Iteração %d -- thread %d\n", i, iteractions[i]);
  }
  free(iteractions);
  return 0;
}