/******************************************************************************/
/* Standard Headers */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Our Headers */
#include "pmm_data.h"
#include "pmm_header.h"
#include "pmm_read_routines.h"
#include "pmm_write_routines.h"

/******************************************************************************/
int main(int argc, char *argv[]) {
  char *input_file, *output_file;
  int rank;
  PMM_Header file_header;
  PMM_Data file_data;
  int error_code;

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
  if (rank == 0) {
    printf("  Input Files:\n");
    printf("    input_file: %s\n", input_file);
    printf("    output_file: %s\n", output_file);
  }

  /* Process The Header */
  if (rank == 0) {
    printf("  Reading Header\n");
  }
  error_code = PMM_ReadHeader(input_file, MPI_COMM_WORLD, &file_header);
  if (error_code == EXIT_FAILURE) {
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  /* Process The Data  */
  if (rank == 0) {
    printf("  Reading Data\n");
  }
  InitializePMM_Data(&file_data);
  error_code =
      PMM_ReadData(input_file, file_header, MPI_COMM_WORLD, &file_data);
  if (error_code == EXIT_FAILURE) {
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  /* Now Write Back Out To File */
  if (rank == 0) {
    printf("  Writing Back To File\n");
  }
  error_code =
      PMM_WriteData(output_file, file_header, MPI_COMM_WORLD, file_data);

  /* Cleanup */
  if (rank == 0) {
    printf("  Cleanup\n");
  }
  CleanupPMM_Data(&file_data);

  MPI_Finalize();
  return 0;
}
