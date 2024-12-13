/*
 * Author: Felipe Hidequel
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 */
void Count_sort(int a[], int n);
void Count_sort_parallel(int a[], int n, int thread_count);
int *init_vect(int n);
int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }
void isEqual(int a[], int b[], int n) {
  for (int i = 0; i < n; i++) {
    if (a[i] != b[i]) {
      printf("Not equal\n");
      return;
    }
  }
  printf("Equal\n");
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: \n");
    printf("./32 <Number of elements> <number of threads>\n");
    return 1;
  }

  int n = strtol(argv[1], NULL, 10);
  int thread_count = strtol(argv[2], NULL, 10);

  int *temp = init_vect(n);
  int *a = malloc(n * sizeof(int));
  int *b = malloc(n * sizeof(int));
  int *c = malloc(n * sizeof(int));

  double start, end;

  for (int i = 0; i < n; i++) {
    a[i] = temp[i];
    b[i] = temp[i];
    c[i] = temp[i];
  }

  start = omp_get_wtime();
  Count_sort(a, n);
  end = omp_get_wtime();
  printf("Serial time: %f\n", end - start);

  start = omp_get_wtime();
  Count_sort_parallel(b, n, thread_count);
  end = omp_get_wtime();
  printf("Parallel time: %f\n", end - start);

  start = omp_get_wtime();
  qsort(c, n, sizeof(int), compare);
  end = omp_get_wtime();
  printf("Qsort time: %f\n", end - start);


  printf("\n");
  free(a);
  free(b);
  free(c);
  free(temp);
  return 0;
}

void Count_sort(int a[], int n) {
  int i, j, count;
  int *temp = malloc(n * sizeof(int));
  for (i = 0; i < n; i++) {
    count = 0;
    for (j = 0; j < n; j++)
      if (a[j] < a[i])
        count++;
      else if (a[j] == a[i] && j < i)
        count++;
    temp[count] = a[i];
  }
  memcpy(a, temp, n * sizeof(int));
  free(temp);
}

void Count_sort_parallel(int a[], int n, int thread_count) {
  int i, j, count;
  int *temp = malloc(n * sizeof(int));

#pragma omp parallel num_threads(thread_count) default(none)                   \
    shared(a, n, temp, thread_count) private(i, j, count)
  {
#pragma omp for
    for (i = 0; i < n; i++) {
      count = 0;
      for (j = 0; j < n; j++)
        if (a[j] < a[i])
          count++;
        else if (a[j] == a[i] && j < i)
          count++;
      temp[count] = a[i];
    }
#pragma omp barrier
    int rest = n % thread_count;
    int my_rank = omp_get_thread_num();
    int my_chunk = n / thread_count + (my_rank < rest);
    int my_start = my_rank * my_chunk + (my_rank < rest ? my_rank : rest);

    memcpy(&a[my_start], &temp[my_start], my_chunk * sizeof(int));
  }

  free(temp);
}

int *init_vect(int n) {
  int *a = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    a[i] = rand() % 100;
  }
  return a;
}