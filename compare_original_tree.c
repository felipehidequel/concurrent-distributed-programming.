#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int my_rank, my_sum = 1;
  int div = 2, salt = 1;
  int soma = 0;
  int parceiro;
  int recebimentos_t1 = 0, adicoes_t1 = 0; // Contadores para a topologia 1
  int recebimentos_t2 = 0, adicoes_t2 = 0; // Contadores para a topologia 2
  int thread_num = atoi(argv[1]);
  printf("Numero de threads: %d", thread_num);

  // Primeiro bloco: todas as threads participam
  printf("=== Topologia 1: Comunicação entre todas as threads ===\n");
#pragma omp parallel num_threads(thread_num) default(shared) private(my_rank,  \
                                                                     parceiro)
  {
    my_rank = omp_get_thread_num();

    while (salt < omp_get_num_threads()) {
#pragma omp single
      printf("\nSalto: %d\n", salt);

      if (my_rank % salt == 0) {
        if (my_rank % div == 0) {
          parceiro = my_rank + salt;
          printf("Thread %d vai receber da thread %d\n", my_rank, parceiro);
          if (my_rank == 0)
            recebimentos_t1++;
        } else {
          parceiro = my_rank - salt;
          printf("Thread %d vai enviar para a thread %d\n", my_rank, parceiro);
        }
      }

#pragma omp barrier // Garantir sincronização entre saltos

#pragma omp single
      {
        div *= 2;
        salt *= 2;
      }
    }

#pragma omp single
    if (my_rank == 0)
      adicoes_t1++;
  }

  // Reset das variáveis
  div = 2;
  salt = 1;
  soma = 0;

  // Segundo bloco: apenas a thread 0 realiza a soma
  printf("\n=== Topologia 2: Apenas thread 0 realiza somas ===\n");
#pragma omp parallel num_threads(4) default(shared) private(my_rank)
  {
    my_rank = omp_get_thread_num();

    if (my_rank != 0) {
#pragma omp atomic
      recebimentos_t2++; // Cada thread não-zero envia para a thread 0
      printf("Thread %d enviando dados para a thread 0\n", my_rank);
    }

#pragma omp barrier

    if (my_rank == 0) {
      for (int i = 1; i < omp_get_num_threads(); i++) {
        printf("Thread 0 recebendo dados da thread %d\n", i);
        soma += my_sum;
        adicoes_t2++;
      }
    }
  }

  // Resultados das comparações
  printf("\nSoma final: %d\n", soma);
  printf("\n--- Comparação entre Topologias ---\n");
  printf("Topologia 1 - Recebimentos (Núcleo 0): %d, Adições (Núcleo 0): %d\n",
         recebimentos_t1, adicoes_t1);
  printf("Topologia 2 - Recebimentos (Núcleo 0): %d, Adições (Núcleo 0): %d\n",
         recebimentos_t2, adicoes_t2);

  return 0;
}
