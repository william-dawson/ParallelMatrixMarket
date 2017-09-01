/******************************************************************************/
/* Standard Headers */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Our Headers */
#include "pmm_header.h"
#include "pmm_data.h"
#include "pmm_read_routines.h"

/******************************************************************************/
int main(int argc, char *argv[]) {
  char *input_file, *output_file;
  int rank;
  PMM_Header file_header;
  PMM_Data file_data;

  /* Init */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    printf("Matrix Market Driver Program\n");
  }

  /* Process Input */
  if (argc < 3) {
    fprintf(stderr, "Specify input, output of the function\n");
    MPI_Abort(MPI_COMM_WORLD, -1);
    return -1;
  }
  input_file = malloc(sizeof(char) * strlen(argv[1]));
  output_file = malloc(sizeof(char) * strlen(argv[2]));
  input_file = argv[1];
  output_file = argv[2];

  /* Process The Header */
  PMM_ReadHeader(input_file, MPI_COMM_WORLD, &file_header);

  /* Process The Data  */
  PMM_ReadData(input_file, file_header, MPI_COMM_WORLD, &file_data);

  /* Now Write Back Out To File */

  MPI_Finalize();
  return 0;
}
