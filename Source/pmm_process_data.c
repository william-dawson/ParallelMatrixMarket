/*! \file pmm_process_data.c
 \brief Implementation of data processing routines.
*/
#include "pmm_read_routines.h"
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pmm_data.h"
#include "pmm_header.h"

extern const int max_line_length;

/******************************************************************************/
int PMM_ExtractRawText(char *file_name, PMM_Header header, MPI_Comm comm,
                       char **raw_text);
int PMM_ExtractData(char *raw_text, PMM_Header header, PMM_Data *data,
                    MPI_Comm comm);
int CalculateIndices(PMM_Data *data, PMM_Header header, MPI_Comm comm);

/******************************************************************************/
int PMM_ReadData(char *file_name, PMM_Header header, MPI_Comm comm,
                 PMM_Data *data) {
  char *raw_text;
  int error_value = EXIT_SUCCESS;

  if (PMM_ExtractRawText(file_name, header, comm, &raw_text) == EXIT_FAILURE)
    return EXIT_FAILURE;

  if (PMM_ExtractData(raw_text, header, data, comm) == EXIT_FAILURE)
    return EXIT_FAILURE;

  free(raw_text);

  return error_value;
}

/******************************************************************************/
int PMM_ExtractRawText(char *file_name, PMM_Header header, MPI_Comm comm,
                       char **raw_text) {
  MPI_File fh;
  MPI_Offset total_file_size;
  MPI_Offset local_read_size;
  MPI_Offset local_read_size_plus_buffer;
  MPI_Offset length_minus_header;
  MPI_Offset local_start_read;
  MPI_Offset start_char;
  MPI_Offset end_char;
  MPI_Status status;
  const MPI_Offset min_read_size = max_line_length;
  int total_processes;
  int rank;
  char *read_buffer;
  char *buffer_offset;
  int error_value = EXIT_SUCCESS;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &total_processes);

  error_value =
      MPI_File_open(comm, file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  if (error_value != MPI_SUCCESS) {
    fprintf(stderr, "Failure opening the file to read\n");
    return EXIT_FAILURE;
  }
  error_value = MPI_File_get_size(fh, &total_file_size);
  if (error_value != MPI_SUCCESS) {
    fprintf(stderr, "Failure getting file size\n");
    return EXIT_FAILURE;
  }

  /* Compute Offsets and Amount of Data To Read */
  length_minus_header = total_file_size - header.header_length;
  local_read_size = fmax(min_read_size, length_minus_header / total_processes);
  local_read_size_plus_buffer = local_read_size + min_read_size;
  local_start_read = local_read_size * rank + header.header_length;

  /* Handle small files */
  if (local_start_read > total_file_size) {
    local_start_read = 0;
    local_read_size = 0;
    local_read_size_plus_buffer = 0;
  } else if (local_start_read + local_read_size > total_file_size) {
    local_read_size = total_file_size - local_start_read;
    local_read_size_plus_buffer = local_read_size;
  }

  read_buffer = (char *)malloc(local_read_size_plus_buffer * sizeof(char));
  if (read_buffer == NULL) {
    perror("Extract Raw Text Buffer Malloc");
    return EXIT_FAILURE;
  }

  /* Perform Actual Read */
  error_value =
      MPI_File_read_at_all(fh, local_start_read, read_buffer,
                           local_read_size_plus_buffer, MPI_CHAR, &status);
  if (error_value != MPI_SUCCESS) {
    fprintf(stderr, "Failure reading the file\n");
    return EXIT_FAILURE;
  }
  error_value = MPI_File_close(&fh);
  if (error_value != MPI_SUCCESS) {
    fprintf(stderr, "Failure closing the file to read\n");
    return EXIT_FAILURE;
  }

  /* Remove Duplicate Data */
  start_char = 0;
  end_char = 0;
  if (local_read_size > 0) {
    if (rank > 0) {
      while (read_buffer[start_char] != '\n') {
        start_char++;
      }
      start_char++;
    }

    end_char = local_read_size;
    while (end_char < local_read_size_plus_buffer &&
           read_buffer[end_char] != '\n') {
      end_char = end_char + 1;
    }
    end_char++;
  }
  buffer_offset = read_buffer + start_char;

  /* Copy The String Out */
  *raw_text = (char *)malloc((end_char - start_char + 1) * sizeof(char));
  if (raw_text == NULL) {
    perror("Extract Raw Text Buffer Malloc");
    return EXIT_FAILURE;
  }

  strncpy(*raw_text, buffer_offset, end_char - start_char);

  free(read_buffer);
  return error_value;
}

/******************************************************************************/
int PMM_ExtractData(char *raw_text, PMM_Header header, PMM_Data *data,
                    MPI_Comm comm) {
  char *temporary_line;
  char *search_pointer;
  double *dbl_value_ptr;
  int *int_value_ptr;
  long int i;
  long int blank_lines;
  int elements_read;
  int error_value = EXIT_SUCCESS;

  /* Count the number of lines to process */
  data->number_of_values = 0;
  if (strlen(raw_text) > 0) {
    for (i = 0; i < strlen(raw_text); ++i) {
      if (raw_text[i] == '\n')
        data->number_of_values++;
    }
  }

  /* Allocate The Data*/
  data->rows = (long int *)malloc(data->number_of_values * sizeof(long int));
  if (data->rows == NULL) {
    perror("Extract Data Malloc");
    return EXIT_FAILURE;
  }
  data->columns = (long int *)malloc(data->number_of_values * sizeof(long int));
  if (data->columns == NULL) {
    perror("Extract Data Malloc");
    return EXIT_FAILURE;
  }
  if (header.data_type == REAL) {
    data->values = (void *)malloc(data->number_of_values * sizeof(double));
    dbl_value_ptr = data->values;
  } else if (header.data_type == INTEGER) {
    data->values = (void *)malloc(data->number_of_values * sizeof(double));
    int_value_ptr = data->values;
  } else if (header.data_type == COMPLEX) {
    data->values = (void *)malloc(2 * data->number_of_values * sizeof(double));
    dbl_value_ptr = data->values;
  }
  if (header.data_type != PATTERN && data->values == NULL) {
    perror("Extract Data Malloc");
    return EXIT_FAILURE;
  }

  /* Extract The Data */
  search_pointer = raw_text;
  temporary_line = strtok(search_pointer, "\n");
  blank_lines = 0;
  for (i = 0; i < data->number_of_values; ++i) {
    if (header.format == COORDINATE) {
      if (header.data_type == REAL) {
        elements_read = sscanf(temporary_line, "%ld %ld %lf", &(data->rows[i]),
                               &(data->columns[i]), &(dbl_value_ptr[i]));
      } else if (header.data_type == INTEGER) {
        elements_read = sscanf(temporary_line, "%ld %ld %d", &(data->rows[i]),
                               &(data->columns[i]), &(int_value_ptr[i]));
      } else if (header.data_type == COMPLEX) {
        elements_read =
            sscanf(temporary_line, "%ld %ld %lf %lf", &(data->rows[i]),
                   &(data->columns[i]), &(dbl_value_ptr[2 * i]),
                   &(dbl_value_ptr[2 * i + 1]));
      } else if (header.data_type == PATTERN) {
        elements_read = sscanf(temporary_line, "%ld %ld", &(data->rows[i]),
                               &(data->columns[i]));
      }
    } else if (header.format == ARRAY) {
      if (header.data_type == REAL) {
        elements_read = sscanf(temporary_line, "%lf", &(dbl_value_ptr[i]));
      } else if (header.data_type == INTEGER) {
        elements_read = sscanf(temporary_line, "%d", &(int_value_ptr[i]));
      } else if (header.data_type == COMPLEX) {
        elements_read =
            sscanf(temporary_line, "%lf %lf", &(dbl_value_ptr[2 * i]),
                   &(dbl_value_ptr[2 * i + 1]));
      }
    }
    temporary_line = strtok(NULL, "\n");
  }

  if (header.format == ARRAY) {
    CalculateIndices(data, header, comm);
  }

  return error_value;
}

/******************************************************************************/
int CalculateIndices(PMM_Data *data, PMM_Header header, MPI_Comm comm) {
  long int *elements_per_process;
  long int index_offset;
  int rank;
  int total_processes;
  long int i;
  long int start_row, start_column;
  int error_value = EXIT_SUCCESS;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &total_processes);

  /* First we figure out how many elements were read by each process */
  elements_per_process = (long int *)malloc(sizeof(long int) * total_processes);
  MPI_Allgather(&(data->number_of_values), 1, MPI_LONG, elements_per_process, 1,
                MPI_LONG, comm);

  /* Now we can accumulate to determine the first entry */
  index_offset = 0;
  for (i = 0; i < rank; ++i) {
    index_offset += elements_per_process[i];
  }

  /* Compute The Matrix Indicies */
  if (header.symmetric == GENERAL) {
    for (i = 0; i < data->number_of_values; ++i) {
      data->rows[i] = (index_offset + i) % header.matrix_columns + 1;
      data->columns[i] = (index_offset + i) / header.matrix_rows + 1;
    }
  } else if (header.symmetric == SYMMETRIC || header.symmetric == HERMITIAN) {
    start_row = 1;
    start_column = 1;
    for (i = 0; i < index_offset; ++i) {
      start_row++;
      if (start_row > header.matrix_rows) {
        start_column++;
        start_row = start_column;
      }
    }
    for (i = 0; i < data->number_of_values; ++i) {
      data->rows[i] = start_row;
      data->columns[i] = start_column;
      start_row++;
      if (start_row > header.matrix_rows) {
        start_column++;
        start_row = start_column;
      }
    }
  } else if (header.symmetric == SKEWSYMMETRIC) {
    start_row = 2;
    start_column = 1;
    for (i = 0; i < index_offset; ++i) {
      start_row++;
      if (start_row > header.matrix_rows) {
        start_column++;
        start_row = start_column + 1;
      }
    }
    for (i = 0; i < data->number_of_values; ++i) {
      data->rows[i] = start_row;
      data->columns[i] = start_column;
      start_row++;
      if (start_row > header.matrix_rows) {
        start_column++;
        start_row = start_column + 1;
      }
    }
  }

  /* Cleanup */
  free(elements_per_process);
  return error_value;
}
