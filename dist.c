#include <omp.h>
#include <stdio.h>

#define N 100000

int main() {
  int a[N];
  int soma = 0;
  for (int i = 0; i < N; i++) {
    a[i] = 1;
    soma += a[i];
  }
  printf("Sequential sum: %d\n", soma);
  soma = 0;
  int my_sum, my_pri_i, my_ult, my_desl;
  int resto;
  int meu_chunk;
  int my_i;

#pragma omp parallel default(shared) private(my_sum, my_pri_i, my_ult,         \
                                             my_desl, meu_chunk, my_i)
  {
    my_sum = 0;
    resto = N % omp_get_num_threads();
    meu_chunk = (N / omp_get_num_threads()) + (omp_get_thread_num() < resto);
    my_pri_i = omp_get_thread_num() * meu_chunk +
               (omp_get_thread_num() < resto ? 0 : resto);
    my_ult = my_pri_i + meu_chunk;

    printf("Hello from %d of %d, my range is from=%d to %d\n",
           omp_get_thread_num(), omp_get_num_threads(), my_pri_i, my_ult);

    for (my_i = my_pri_i; my_i < my_ult; my_i++)
      my_sum += a[my_i];

#pragma omp atomic
    soma += my_sum;
  }
  printf("Parallel sum: %d\n", soma);
}