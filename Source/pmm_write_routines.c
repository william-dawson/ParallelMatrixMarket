/*! \file pmm_write_routines.c
 \brief Implementation of write to file routines.
*/
#include "pmm_write_routines.h"

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const int max_line_length;

/******************************************************************************/
int PMM_CreateHeaderText(PMM_Header header, char **header_text);
int PMM_CreateDataText(PMM_Header header, PMM_Data data, char **data_text);
int PMM_PerformWrite(char *file_name, char *header_text, char *data_text,
                     MPI_Comm comm);

/******************************************************************************/
int PMM_WriteData(char *file_name, PMM_Header header, MPI_Comm comm,
                  PMM_Data data) {
  char *header_text;
  char *data_text;
  int error_value = EXIT_SUCCESS;

  if (PMM_CreateHeaderText(header, &header_text) != EXIT_SUCCESS)
    return EXIT_FAILURE;
  if (PMM_CreateDataText(header, data, &data_text) != EXIT_SUCCESS)
    return EXIT_FAILURE;
  if (PMM_PerformWrite(file_name, header_text, data_text, comm) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  free(header_text);
  free(data_text);

  return error_value;
}

/******************************************************************************/
int PMM_CreateHeaderText(PMM_Header header, char **header_text) {
  char temp_string[max_line_length];
  int error_value = EXIT_SUCCESS;

  *header_text = (char *)malloc(sizeof(char) * max_line_length);
  if (header_text == NULL) {
    perror("Create Header Malloc");
    return EXIT_FAILURE;
  }

  sprintf(*header_text, "%%%%MatrixMarket matrix ");
  switch (header.format) {
  case COORDINATE:
    strcat(*header_text, "coordinate");
    break;
  case ARRAY:
    strcat(*header_text, "array");
    break;
  }
  strcat(*header_text, " ");
  switch (header.data_type) {
  case REAL:
    strcat(*header_text, "real");
    break;
  case INTEGER:
    strcat(*header_text, "integer");
    break;
  case COMPLEX:
    strcat(*header_text, "complex");
    break;
  case PATTERN:
    strcat(*header_text, "pattern");
    break;
  }
  strcat(*header_text, " ");
  switch (header.symmetric) {
  case GENERAL:
    strcat(*header_text, "general");
    break;
  case SYMMETRIC:
    strcat(*header_text, "symmetric");
    break;
  case SKEWSYMMETRIC:
    strcat(*header_text, "skew-symmetric");
    break;
  case HERMITIAN:
    strcat(*header_text, "hermitian");
    break;
  }
  strcat(*header_text, "\n");
  sprintf(temp_string, "%ld %ld %ld \n", header.matrix_rows,
          header.matrix_columns, header.total_elements);
  strcat(*header_text, temp_string);

  return error_value;
}

/******************************************************************************/
int PMM_CreateDataText(PMM_Header header, PMM_Data data, char **data_text) {
  char temp_string[max_line_length];
  long int data_size;
  int error_value = EXIT_SUCCESS;
  int i;

  /* First Run Computes The Size Of The Text */
  data_size = 0;
  for (i = 0; i < data.number_of_values; ++i) {
    if (header.format == COORDINATE) {
      switch (header.data_type) {
      case REAL:
        sprintf(temp_string, "%ld %ld %lf\n", data.rows[i], data.columns[i],
                data.values[i]);
        break;
      case INTEGER:
        sprintf(temp_string, "%ld %ld %d\n", data.rows[i], data.columns[i],
                (int)data.values[i]);
        break;
      case COMPLEX:
        sprintf(temp_string, "%ld %ld %lf %lf\n", data.rows[i], data.columns[i],
                data.values[2 * i], data.values[2 * i + 1]);
        break;
      case PATTERN:
        sprintf(temp_string, "%ld %ld\n", data.rows[i], data.columns[i]);
        break;
      }
    } else if (header.format == ARRAY) {
      fprintf(stderr, "No support now for array type write");
      return EXIT_FAILURE;
    }
    data_size += strlen(temp_string);
  }

  *data_text = (char *)malloc(sizeof(char) * data_size + 1);
  for (i = 0; i < data.number_of_values; ++i) {
    if (header.format == COORDINATE) {
      switch (header.data_type) {
      case REAL:
        sprintf(temp_string, "%ld %ld %lf\n", data.rows[i], data.columns[i],
                data.values[i]);
        break;
      case INTEGER:
        sprintf(temp_string, "%ld %ld %d\n", data.rows[i], data.columns[i],
                (int)data.values[i]);
        break;
      case COMPLEX:
        sprintf(temp_string, "%ld %ld %lf %lf\n", data.rows[i], data.columns[i],
                data.values[2 * i], data.values[2 * i + 1]);
        break;
      case PATTERN:
        sprintf(temp_string, "%ld %ld\n", data.rows[i], data.columns[i]);
        break;
      }
    } else if (header.format == ARRAY) {
      fprintf(stderr, "No support now for array type write");
      return EXIT_FAILURE;
    }
    if (i == 0)
      strcpy(*data_text, temp_string);
    else
      strcat(*data_text, temp_string);
  }

  return error_value;
}

/******************************************************************************/
int PMM_PerformWrite(char *file_name, char *header_text, char *data_text,
                     MPI_Comm comm) {
  MPI_File fh;
  MPI_Offset header_size, write_offset;
  MPI_Offset text_size;
  MPI_Offset *write_size_per_processor;
  MPI_Status status;
  int rank;
  int i;
  int total_processes;
  int error_value = EXIT_SUCCESS;

  /* Compute Offsets*/
  header_size = strlen(header_text);

  /* Sizes on each process */
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &total_processes);
  write_size_per_processor =
      (MPI_Offset *)malloc(sizeof(MPI_Offset) * total_processes);
  text_size = strlen(data_text);
  MPI_Allgather(&text_size, 1, MPI_OFFSET, write_size_per_processor, 1,
                MPI_OFFSET, comm);

  /* Sum sizes to get the offset */
  write_offset = header_size;
  for (i = 0; i < rank; ++i) {
    write_offset += write_size_per_processor[i];
  }

  /* Actual Write */
  MPI_File_open(comm, file_name, MPI_MODE_WRONLY | MPI_MODE_CREATE,
                MPI_INFO_NULL, &fh);
  /* Header */
  if (rank == 0) {
    MPI_File_write_at(fh, 0, header_text, strlen(header_text), MPI_CHAR,
                      &status);
  }
  /* Data */
  MPI_File_write_at(fh, write_offset, data_text, strlen(data_text), MPI_CHAR,
                    &status);
  MPI_File_close(&fh);
  free(write_size_per_processor);

  return error_value;
}
