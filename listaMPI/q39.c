#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void read_vector(int *vetor, int *local_vetor, int n, int *local_n, int rank,
                 int size, int *send_counts, int *desl);
void get_input(int *n, int *local_n, int rank, int size, int *escalar);
void block_compute(int size, int *send_counts, int *desl, int n);

int main(int argc, char *argv[]) {
  int *vetor1, *vetor2; // vetores originais
  int rank, size;
  int n, local_n, escalar;
  int *local_vetor1, *local_vetor2;
  long long int global_produto = 1;
  int *send_counts;
  int *desl;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  send_counts = (int *)malloc(size * sizeof(int));
  desl = (int *)malloc(size * sizeof(int));

  get_input(&n, &local_n, rank, size, &escalar);
  local_vetor1 = (int *)malloc(local_n * sizeof(int));
  local_vetor2 = (int *)malloc(local_n * sizeof(int));

  vetor1 = (int *)malloc(n * sizeof(int));
  vetor2 = (int *)malloc(n * sizeof(int));

  block_compute(size, send_counts, desl, n);

  read_vector(vetor1, local_vetor1, n, &local_n, rank, size, send_counts, desl);
  read_vector(vetor2, local_vetor2, n, &local_n, rank, size, send_counts, desl);

  int local_produto = 1;
  for (int k = 0; k < local_n; k++) {
    local_vetor1[k] *= escalar;
    local_produto += local_vetor2[k] * local_vetor2[k];
  }

  if (rank == 0) {
    printf("Vetor 1 passado:\n");
    print_vetor(n, vetor1);

    printf("Vetor 2 passado:\n");
    print_vetor(n, vetor2);
  }

  MPI_Reduce(&local_produto, &global_produto, 1, MPI_INT, MPI_PROD, 0,
             MPI_COMM_WORLD);

  MPI_Gatherv(local_vetor1, local_n, MPI_INT, vetor1, send_counts, desl,
              MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    printf("Produto interno do vetor 2: %lld\n", global_produto);

    printf("Vetor 1 multiplicado pelo escalar %d:\n", escalar);
    print_vetor(n, vetor1);
  }

  free(local_vetor1);
  free(local_vetor2);
  free(send_counts);
  free(desl);
  if (rank == 0) {
    free(vetor1);
    free(vetor2);
  }
  MPI_Finalize();

  return 0;
}

void get_input(int *n, int *local_n, int rank, int size, int *escalar) {
  if (rank == 0) {
#ifdef DEBUG
    *n = 10;
    *escalar = 4;
#else
    printf("Entre com o tamanho dos vetores:\n");
    scanf("%d", n);

    printf("Entre com o escalar: \n");
    scanf("%d", escalar);

#endif // DEBUG
  }

  MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(escalar, 1, MPI_INT, 0, MPI_COMM_WORLD);
  *local_n = *n / size + (rank < (*n % size));
}

void read_vector(int *vetor, int *local_vetor, int n, int *local_n, int rank,
                 int size, int *send_counts, int *desl) {
  if (rank == 0) {
    int aux;
    printf("Entre com %d valores:\n", n);
    for (int i = 0; i < n; i++) {
#ifdef DEBUG
      vetor[i] = (rand() * time(NULL)) % 20;
#else
      scanf("%d", &aux);
      vetor[i] = aux;
#endif // DEBUG
    }
    printf("\n");
  }
  MPI_Scatterv(vetor, send_counts, desl, MPI_INT, local_vetor, *local_n,
               MPI_INT, 0, MPI_COMM_WORLD);
}

void block_compute(int size, int *send_counts, int *desl, int n) {
  int rest = n % size;
  for (int r = 0; r < size; r++) {
    send_counts[r] = (n / size) + (r < (rest));
    desl[r] = r * (n / size) + (r < (rest) ? r : rest);
  }
}

void print_vetor(int n, int *vetor) {
  for (int i = 0; i < n; i++) {
    printf("%d ", vetor[i]);
  }
  printf("\n");
}