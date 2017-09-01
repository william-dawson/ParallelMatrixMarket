////////////////////////////////////////////////////////////////////////////////
#include <mpi.h>
#include <stdio.h>
#include <string.h>

#include "pmm_read_routines.h"
void ProcessInfoLine(char *line, PMM_Header *header);
int BroadcastHeader(PMM_Header *header, MPI_Comm comm, int rank);

////////////////////////////////////////////////////////////////////////////////
int PMM_ReadHeader(char *file_name, MPI_Comm comm, PMM_Header *header) {
  FILE *fp;
  char line_buffer[1024];
  int rank;
  char first_char;

  MPI_Comm_rank(comm, &rank);
  if (rank == 0) {
    fp = fopen(file_name, "r");
    if (fp == NULL) {
      fprintf(stderr, "Invalid file: %s\n", file_name);
      return -1;
    } else {
      // Get The First Line
      fgets(line_buffer, 1024, fp);
      ProcessInfoLine(line_buffer, header);

      // Get the length of the header
      header->header_length = strlen(line_buffer);
      do {
        fgets(line_buffer, 1024, fp);
        header->header_length += strlen(line_buffer);
        first_char = line_buffer[0];
      } while (first_char == '%');

      // Get The Matrix Size
      sscanf(line_buffer, "%d %d %ld", &(header->matrix_rows),
             &(header->matrix_columns), &(header->total_elements));
      fclose(fp);
    }
  }

  // Share The Header Info With The Other Processes
  BroadcastHeader(header, comm, 0);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
void ProcessInfoLine(char *line, PMM_Header *header) {
  char format[1024];
  char data_type[1024];
  char symmetric[1024];
  char *tokenizer;

  // Extra info
  tokenizer = strtok(line, " ");
  tokenizer = strtok(NULL, " ");

  // Extract the information.
  tokenizer = strtok(NULL, " ");
  if (strcmp(tokenizer, "coordinate"))
    header->format = COORDINATE;
  else if (strcmp(tokenizer, "array"))
    header->format = ARRAY;

  tokenizer = strtok(NULL, " ");
  if (strcmp(tokenizer, "real"))
    header->data_type = REAL;
  else if (strcmp(tokenizer, "integer"))
    header->data_type = INTEGER;
  else if (strcmp(tokenizer, "complex"))
    header->data_type = COMPLEX;
  else if (strcmp(tokenizer, "pattern"))
    header->data_type = PATTERN;

  tokenizer = strtok(NULL, " ");
  if (strcmp(tokenizer, "general"))
    header->symmetric = GENERAL;
  else if (strcmp(tokenizer, "symmetric"))
    header->symmetric = SYMMETRIC;
  else if (strcmp(tokenizer, "skew-symmetric"))
    header->symmetric = SKEWSYMMETRIC;
  else if (strcmp(tokenizer, "hermitian"))
    header->symmetric = HERMITIAN;
}


////////////////////////////////////////////////////////////////////////////////
int BroadcastHeader(PMM_Header *header, MPI_Comm comm, int rank) {
  MPI_Bcast(&(header->format), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->data_type), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->symmetric), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->header_length), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->matrix_rows), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->matrix_columns), 1, MPI_INT, 0, comm);
  MPI_Bcast(&(header->total_elements), 1, MPI_LONG, 0, comm);

  return 0;
}
