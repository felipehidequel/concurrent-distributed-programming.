#include <omp.h>
#include <stdio.h>

#define N 137

int main() {

  int A[N], B[N], C[N];

  int my_rank, my_sum = 1;
  int div = 2, salt = 1;
  int soma = 0;
  int parceiro;
  soma += my_sum;
  int from, to;
  int chunk;
  int P;
  int soma;
#pragma omp parallel num_threads(4) default(shared) private(my_rank, my_sum,   \
                                                            parceiro)
  {

    P = omp_get_num_threads();

    my_rank = omp_get_thread_num();
    chunk = N / omp_get_num_threads() + (my_rank < N / omp_get_num_threads());
    from = my_rank * chunk + (my_rank<(N%P)?0:(N%P));
    to = from + chunk;

    for (int i = from; i < to; i++) 
            {
            A[i] = B[i] + C[i];


      while (salt < omp_get_num_threads()) {


        if (my_rank % salt == 0) { // Escolhe o rank que participa do salto
          if (my_rank % div == 0) {
            parceiro = my_rank + salt;
            printf("Eu sou a thread %d e vou receber da thread %d\n", my_rank,
                   parceiro);
            
          } else {
            parceiro = my_rank - salt;
            int my_diff = A[i];
          }
        }
#pragma omp single
        // #pragma omp critical
        {
          div *= 2;
          salt *= 2;
        }
      }

            }
  }

  printf("Soma final: %d\n", soma);
  return 0;
}
