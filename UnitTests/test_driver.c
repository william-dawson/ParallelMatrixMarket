////////////////////////////////////////////////////////////////////////////////
// Standard Headers
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Our Headers
#include "mmheader.h"
#include "process_header.h"

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
  char *input_file, *output_file;
  int rank;
  MMHeader file_header;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    printf("Matrix Market Driver Program\n");
  }

  // Process Input
  if (argc < 3) {
    fprintf(stderr, "Specify input, output of the function\n");
    MPI_Abort(MPI_COMM_WORLD, -1);
    return -1;
  }
  input_file = malloc(sizeof(char) * strlen(argv[1]));
  output_file = malloc(sizeof(char) * strlen(argv[2]));
  input_file = argv[1];
  output_file = argv[2];

  // Process The Header
  ReadHeader(input_file, &file_header, MPI_COMM_WORLD);

  // Process The Data

  PrintHeader(file_header);

  MPI_Finalize();
  return 0;
}
