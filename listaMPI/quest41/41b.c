#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void all_reduce_ring(int *sum, int rank, int p, int *my_val, MPI_Comm comm) {
  int i;
  int temp_val;
  int dest, source, sendtag, recvtag;

  dest = rank + 1;
  source = rank - 1;
  if (rank == (p - 1)) {
    source = rank - 1;
    dest = 0;
  } else if (rank == 0) {
    source = p - 1;
    dest = 1;
  }

  *sum = temp_val = *my_val;
  for (i = 1; i < p; i++) {
    MPI_Sendrecv_replace(&temp_val, 1, MPI_INT, dest, i, source, i, comm, NULL);
    if (rank >= i)
      *sum += temp_val;
  }
}

int main() {
  int sum, rank, size, my_val;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  sum = 0;
  my_val = rank + 1;

  MPI_Barrier(comm);
  all_reduce_ring(&sum, rank, size, &my_val, comm);
  MPI_Barrier(comm);

  //   printf("Process %d -> My initial value: %d\n", rank, my_val);
  //   printf("Process %d -> My final value: %d\n", rank, sum);
  for (int r = 0; r < size; r++) {
    if (rank == r) {
      printf("Rank %d: \n", rank);
      printf("Initial valuer: \n");
      printf("%d\n", my_val);
      printf("sum: \n");
      printf("%d\n", sum);
      printf("\n");
    }
    MPI_Barrier(comm);
  }

  MPI_Finalize();

  return 0;
}