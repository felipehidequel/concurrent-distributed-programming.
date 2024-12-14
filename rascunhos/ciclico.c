#include <omp.h>
#include <stdio.h>

#define N 137

int main() {
  int my_sum, my_pri_i, my_ult, my_desl;
  int meu_chunk;
  int my_i;

#pragma omp parallel default(shared) private(my_sum, my_pri_i, my_ult,         \
                                             my_desl, meu_chunk, my_i)
  {
    my_pri_i = omp_get_num_threads() - (omp_get_thread_num() + 1);
    my_ult = N;
    my_desl = N / omp_get_num_threads() +
              (omp_get_thread_num() < N % omp_get_num_threads());

    printf("Thread %d of %d, my range is from=%d to %d\n", omp_get_thread_num(),
           omp_get_num_threads(), my_pri_i, my_ult);

    for (my_i = my_pri_i; my_i < my_ult; my_i += my_desl) {
      // Simulação sem processamento demorado:
      printf("Thread %d processing i=%d with weight=%d\n", omp_get_thread_num(),
             my_i, my_i == 0 ? 2 : 2 * my_i);
    }
  }

  return 0;
}
