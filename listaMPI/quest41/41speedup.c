#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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
    *sum += temp_val;
  }
}

int main() {
  int sum, rank, size, my_val;
  MPI_Comm comm = MPI_COMM_WORLD;
  double start, end, elapsed, local_elapsed;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  sum = 0;
  my_val = 1;

  MPI_Barrier(comm);
  start = MPI_Wtime();
  all_reduce_ring(&sum, rank, size, &my_val, comm);
  end = MPI_Wtime();
  local_elapsed = end - start;

  MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
  if (rank == 0) {
    printf("Elapsed time %.6lf for allreduceRing\n", elapsed);
  }

  // sum = 0;
  // my_val = 1;

  // MPI_Barrier(comm);
  // start = MPI_Wtime();
  // MPI_Allreduce(&my_val, &sum, 1, MPI_INT, MPI_SUM, comm);
  // end = MPI_Wtime();
  // local_elapsed = end - start;

  // MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
  // if (rank == 0) {
  //   printf("Elapsed time %.6lf for allreduce butterfly\n", elapsed);
  // }

  MPI_Finalize();

  return 0;
}