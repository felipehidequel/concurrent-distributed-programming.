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
  int comm_sz, rank, local_n, rest, n;
  int *x, *local_x;
  int *sendcounts, *displs; /* n_itters, deslocamento */

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  get_input(rank, comm_sz, &n);

  rest = n % comm_sz;
  local_n = n / comm_sz + (rank < rest);

  local_x = (int *)malloc(sizeof(int) * n);

  if (rank == 0) {
    x = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
      x[i] = i;
  }

  sendcounts = (int *)malloc(comm_sz * sizeof(int));
  displs = (int *)malloc(comm_sz * sizeof(int));

  for (int i = 0; i < comm_sz; i++) {
    sendcounts[i] = n / comm_sz + (i < rest);
    displs[i] = i * (n / comm_sz) + (i < rest ? i : rest);
  }

  MPI_Scatterv(x, sendcounts, displs, MPI_INT, local_x, local_n, MPI_INT, 0,
               MPI_COMM_WORLD);

  printf("Processo %d -> Recebi as iterações:\n", rank);
  for (int i = 0; i < local_n; i++) {
    printf("%d ", local_x[i]);
  }

  printf("\n");

  MPI_Finalize();

  free(local_x);
  free(sendcounts);
  free(displs);
  if (rank == 0)
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