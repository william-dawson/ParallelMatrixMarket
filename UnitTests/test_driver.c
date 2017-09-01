////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
  char *input_file, *output_file;
  int rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    printf("Matrix Market Driver Program");
  }

  // Process Input
  if (argc < 3) {
    fprintf(stderr, "Specify input, output of the function\n");
    MPI_Abort(MPI_COMM_WORLD, -1);
    return -1;
  }

  input_file = malloc(sizeof(char) * strlen(argv[1]));
  output_file = malloc(sizeof(char) * strlen(argv[2]));

  MPI_Finalize();
  return 0;
}
