/*! \file pmm_process_header.c
 \brief Implementation of header processing routines.
*/
#include "pmm_read_routines.h"

#include <mpi.h>
#include <stdio.h>
#include <string.h>

extern const int max_line_length;

/******************************************************************************/
int PMM_ProcessInfoLine(const char *line, PMM_Header *header);
int PMM_BroadcastHeader(PMM_Header *header, MPI_Comm comm, int rank);

/******************************************************************************/
int PMM_ReadHeader(char *file_name, MPI_Comm comm, PMM_Header *header) {
  FILE *fp;
  char line_buffer[max_line_length];
  int rank;
  char first_char;
  char *str_error;
  int error_value = EXIT_SUCCESS;

  MPI_Comm_rank(comm, &rank);
  if (rank == 0) {
    fp = fopen(file_name, "r");
    if (fp == NULL) {
      fprintf(stderr, "Invalid file: %s\n", file_name);
      return EXIT_FAILURE;
    } else {
      /* Get The First Line */
      str_error = fgets(line_buffer, max_line_length, fp);
      if (str_error == NULL) {
        fprintf(stderr, "Error while reading header\n");
        return EXIT_FAILURE;
      }
      error_value = PMM_ProcessInfoLine(line_buffer, header);
      if (error_value == EXIT_FAILURE)
        return error_value;

      /* Get the length of the header */
      header->header_length = strlen(line_buffer);
      do {
        str_error = fgets(line_buffer, max_line_length, fp);
        if (str_error == NULL) {
          fprintf(stderr, "Error while reading header\n");
          return EXIT_FAILURE;
        }
        header->header_length += strlen(line_buffer);
        first_char = line_buffer[0];
      } while (first_char == '%');

      /* Get The Matrix Size */
      error_value =
          sscanf(line_buffer, "%ld %ld %ld", &(header->matrix_rows),
                 &(header->matrix_columns), &(header->total_elements));
      if (error_value != 3) {
        fprintf(stderr, "Error while reading header size info.\n");
      } else {
        error_value = EXIT_SUCCESS;
      }
      fclose(fp);
    }
  }

  /* Share The Header Info With The Other Processes */
  error_value = PMM_BroadcastHeader(header, comm, 0);
  if (error_value != EXIT_SUCCESS)
    return EXIT_FAILURE;

  return error_value;
}

/******************************************************************************/
int PMM_ProcessInfoLine(const char * const line, PMM_Header *header) {
  char format[max_line_length];
  char data_type[max_line_length];
  char symmetric[max_line_length];
  char line_copy[max_line_length];
  char *token;
  int error_value = EXIT_SUCCESS;

  /* Copy the line because strtok is not const */
  strcpy(line_copy,line);

  /* Extra info */
  token = strtok(line_copy, " ");
  token = strtok(NULL, " ");

  /* Extract the information. */
  token = strtok(NULL, " ");
  if (strcmp(token, "coordinate") == 0) {
    header->format = COORDINATE;
  } else if (strcmp(token, "array") == 0) {
    header->format = ARRAY;
  } else {
    fprintf(stderr, "Illegal Matrix Type\n.");
    return EXIT_FAILURE;
  }

  token = strtok(NULL, " ");
  if (strcmp(token, "real") == 0) {
    header->data_type = REAL;
  } else if (strcmp(token, "integer") == 0) {
    header->data_type = INTEGER;
  } else if (strcmp(token, "complex") == 0) {
    header->data_type = COMPLEX;
  } else if (strcmp(token, "pattern") == 0) {
    header->data_type = PATTERN;
  } else {
    fprintf(stderr, "Illegal Matrix Data Type\n.");
    return EXIT_FAILURE;
  }

  token = strtok(NULL, " ");
  if (strcmp(token, "general\n") == 0) {
    header->symmetric = GENERAL;
  } else if (strcmp(token, "symmetric\n") == 0) {
    header->symmetric = SYMMETRIC;
  } else if (strcmp(token, "skew-symmetric\n") == 0) {
    header->symmetric = SKEWSYMMETRIC;
  } else if (strcmp(token, "hermitian\n") == 0) {
    header->symmetric = HERMITIAN;
  } else {
    fprintf(stderr, "Illegal Matrix Symmetry Type\n.");
    return EXIT_FAILURE;
  }

  return error_value;
}

/******************************************************************************/
int PMM_BroadcastHeader(PMM_Header *header, MPI_Comm comm, int rank) {
  int error_value = EXIT_SUCCESS;

  MPI_Bcast(&(header->format),         1, MPI_INT,  0, comm);
  MPI_Bcast(&(header->data_type),      1, MPI_INT,  0, comm);
  MPI_Bcast(&(header->symmetric),      1, MPI_INT,  0, comm);
  MPI_Bcast(&(header->header_length),  1, MPI_INT,  0, comm);
  MPI_Bcast(&(header->matrix_rows),    1, MPI_LONG, 0, comm);
  MPI_Bcast(&(header->matrix_columns), 1, MPI_LONG, 0, comm);
  MPI_Bcast(&(header->total_elements), 1, MPI_LONG, 0, comm);

  return error_value;
}
