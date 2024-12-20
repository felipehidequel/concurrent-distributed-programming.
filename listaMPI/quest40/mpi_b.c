#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void read_vetor(int n, int *vetor, int rank, int *local_sum, MPI_Comm comm);
void prefix_sum(int size, int rank, int *local_sum, MPI_Comm comm);
void print_vetor(int n, int *vetor);

int main(void) {
  int *vetor = NULL;
  int *soma = NULL;
  int n, rank, size;
  int local_sum;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  n = size;
  if ((size & (size - 1)) != 0) {
    if (rank == 0) {
      printf("O número de processos deve ser uma potência de 2.\n");
    }
    MPI_Finalize();
    return 1;
  }

  soma = (int *)malloc(n * sizeof(int));

  read_vetor(n, vetor, rank, &local_sum, comm);
  prefix_sum(n, rank, &local_sum, comm);

  MPI_Gather(&local_sum, 1, MPI_INT, soma, 1, MPI_INT, 0, comm);

  if (rank == 0) {
    printf("A soma dos prefixos: \n");
    print_vetor(n, soma);
  }

  free(soma);
  MPI_Finalize();

  return 0;
}

void read_vetor(int n, int *vetor, int rank, int *local_sum, MPI_Comm comm) {
  vetor = (int *)malloc(sizeof(int) * n);
  if (rank == 0) {
    printf("Entre com %d elementos para o vetor:\n", n);
    for (int i = 0; i < n; i++) {
      scanf("%d", &vetor[i]);
    }
  }
  MPI_Scatter(vetor, 1, MPI_INT, local_sum, 1, MPI_INT, 0, comm);
  free(vetor);
}

void prefix_sum(int size, int rank, int *local_sum, MPI_Comm comm) {
  int partner;
  int aux;
  if (rank > 0) {
    partner = rank - 1;
    MPI_Recv(&aux, 1, MPI_INT, partner, 0, comm, NULL);
    *local_sum += aux;
#ifdef DEBUG
    printf("rank %d -> recebe %d de rank %d e soma a %d\n", rank, aux, partner,
           *local_sum);
#endif
  }
  if (rank < size - 1) {
    partner = rank + 1;
    MPI_Send(local_sum, 1, MPI_INT, partner, 0, comm);
#ifdef DEBUG
    printf("rank %d -> envia %d para rank %d\n", rank, *local_sum, partner);
#endif
  }
}

void print_vetor(int n, int *vetor) {
  for (int i = 0; i < n; i++) {
    printf("%d ", vetor[i]);
  }
  printf("\n");
}