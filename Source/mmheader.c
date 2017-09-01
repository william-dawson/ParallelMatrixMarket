////////////////////////////////////////////////////////////////////////////////
#include "mmheader.h"
#include <mpi.h>
#include <stdio.h>

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
  printf("%d %d %ld \n", header.matrix_rows, header.matrix_columns,
         header.total_elements);
}
