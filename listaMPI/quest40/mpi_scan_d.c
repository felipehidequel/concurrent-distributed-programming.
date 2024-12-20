#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void get_input(int *n, int rank, MPI_Comm comm) {
  if (rank == 0) {
    printf("Enter with order for the vector:\n");
    scanf("%d", n);
  }
  MPI_Bcast(n, 1, MPI_INT, 0, comm);
}

int main(int argc, char *argv[]) {
  MPI_Comm comm = MPI_COMM_WORLD;
  int rank, size, n;
  int *sum_prefix, *vector;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  get_input(&n, rank, comm);

  vector = (int *)malloc(sizeof(int) * n);
  sum_prefix = (int *)malloc(sizeof(int) * n);

  srand(rank + time(NULL));
  for (int i = 0; i < n; i++) {
    vector[i] = rand() % 10;
  }

  MPI_Barrier(comm);
  MPI_Scan(vector, sum_prefix, n, MPI_INT, MPI_SUM, comm);
  MPI_Barrier(comm);

  for (int r = 0; r < size; r++) {
    if (rank == r) {
      printf("Rank %d: \n\n", rank);
      printf("Initial Vector: \n");
      for (int i = 0; i < n; i++) {
        printf("%d ", vector[i]);
      }
      printf("\nPrefix sum: \n");

      for (int i = 0; i < n; i++) {
        printf("%d ", sum_prefix[i]);
      }
      printf("\n");
    }
    MPI_Barrier(comm);
  }

  free(vector);
  free(sum_prefix);

  MPI_Finalize();

  return 0;
}
