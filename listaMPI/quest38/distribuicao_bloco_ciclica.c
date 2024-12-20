/*
Suponha que um programa seja executado com comm_sz processos e que x seja um
vetor com n componentes. Como os componentes de x seriam distribuídos entre os
processos em um programa que usasse uma distribuição:
(a) em bloco? (b) cíclica?
(c) bloco-cíclica com tamanho de bloco b?
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_input(int rank, int comm_sz, int *n);

int main(int argc, char *argv[]) {
  int comm_sz, rank, n, local_n, bloco = 2;
  int *x, *local_x;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  get_input(rank, comm_sz, &n);
  local_n = n / comm_sz + (rank < n % comm_sz);
  local_x = (int *)malloc(sizeof(int) * (local_n));

  x = (int *)malloc(sizeof(int) * n);
  if (rank == 0) {
    for (int i = 0; i < n; i++)
      x[i] = i;
  }

  MPI_Bcast(x, n, MPI_INT, 0, MPI_COMM_WORLD);

  int count = 0;
  for (int i = bloco * rank; i < n; i += comm_sz * bloco) {
    for (int j = 0; j < bloco && j + i < n; j++) {
      local_x[count++] = x[i + j];
    }
  }

  printf("Processo %d -> Recebi as iterações:\n", rank);
  for (int i = 0; i < count; i++) {
    printf("%d ", local_x[i]);
  }

  printf("\n");

  MPI_Finalize();

  free(local_x);
  free(x);

  return 0;
}

void get_input(int rank, int comm_sz, int *n) {
  if (rank == 0) {
    printf("Entre com o tamanho do vetor\n");
    scanf("%d", n);
  }

  MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);
}