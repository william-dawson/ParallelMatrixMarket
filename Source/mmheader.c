////////////////////////////////////////////////////////////////////////////////
#include "mmheader.h"
#include <mpi.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
int BroadcastHeader(MMHeader *header, MPI_Comm comm, int rank) {
  MPI_Bcast(&(header->format), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->data_type), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->symmetric), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->header_length), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->matrix_rows), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->matrix_columns), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->total_elements), 1, MPI_INT, 0, comm);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
void PrintHeader(MMHeader header) {
  printf("%%%%MatrixMarket matrix ");
  switch (header.format) {
  case COORDINATE:
    printf("coordinate");
    break;
  case ARRAY:
    printf("array");
    break;
  }
  printf(" ");
  switch (header.data_type) {
  case REAL:
    printf("real");
    break;
  case INTEGER:
    printf("integer");
    break;
  case COMPLEX:
    printf("complex");
    break;
  case PATTERN:
    printf("pattern");
    break;
  }
  printf(" ");
  switch (header.symmetric) {
  case GENERAL:
    printf("general");
    break;
  case SYMMETRIC:
    printf("symmetric");
    break;
  case SKEWSYMMETRIC:
    printf("skew-symmetric");
    break;
  case HERMITIAN:
    printf("hermitian");
    break;
  }
  printf("\n");
  printf("%d %d %d \n", header.matrix_rows, header.matrix_columns,
         header.total_elements);
}
