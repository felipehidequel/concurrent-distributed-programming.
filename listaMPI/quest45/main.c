#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void get_n(int *n, int *my_rank, int *comm_sz, MPI_Comm comm);
void create_datatype(int *array, int n, MPI_Datatype *upper_triangule);

int main() {
  int n, rank, comm_sz, *array;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Datatype upper_triangule;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(comm, &comm_sz);
  MPI_Comm_rank(comm, &rank);

  get_n(&n, &rank, &comm_sz, comm);
  array = malloc(n * n * sizeof(int));

  create_datatype(array, n, &upper_triangule);
  if (rank == 0) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        printf("array[%d][%d]: \n", i, j);
        scanf("%d", &array[i * n + j]);
      }
    }
    MPI_Send(array, 1, upper_triangule, 1, 0, comm);
  }

  if (rank == 1) {
    MPI_Recv(array, 1, upper_triangule, 0, 0, comm, MPI_STATUS_IGNORE);

    printf("Upper triangule matrix: \n");
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (j < i) {
          printf(" ");
          continue;
        }
        printf("%d ", array[i * n + j]);
      }
      printf("\n");
    }
  }

  free(array);
  MPI_Type_free(&upper_triangule);
  MPI_Finalize();

  return 0;
}

void get_n(int *n, int *my_rank, int *comm_sz, MPI_Comm comm) {
  if (*my_rank == 0) {
    printf("Enter the size of the matrix: \n");
    scanf("%d", n);
  }
  MPI_Bcast(n, 1, MPI_INT, 0, comm);
}

void create_datatype(int *array, int n, MPI_Datatype *upper_triangule) {
  int send_cout[n];
  int displs[n];

  for (int i = 0; i < n; i++) {
    send_cout[i] = n - i;
    displs[i] = i * n + i;
  }

  MPI_Type_indexed(n, send_cout, displs, MPI_INT, upper_triangule);
  MPI_Type_commit(upper_triangule);
}