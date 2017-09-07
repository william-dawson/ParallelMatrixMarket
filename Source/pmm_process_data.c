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
int PMM_ExtractData(char *raw_text, PMM_Header header,
                    PMM_Data *data);

/******************************************************************************/
int PMM_ReadData(char *file_name, PMM_Header header, MPI_Comm comm,
                 PMM_Data *data) {
  char *raw_text;
  int error_value = EXIT_SUCCESS;
  /* Get The Raw Text */
  PMM_ExtractRawText(file_name, header, comm, &raw_text);

  /* Extract The Data */
  PMM_ExtractData(raw_text, header, data);

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
  int error_value = EXIT_SUCCESS;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &total_processes);

  MPI_File_open(comm, file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  MPI_File_get_size(fh, &total_file_size);

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
  } else if (local_start_read + local_read_size_plus_buffer > total_file_size) {
    local_read_size = total_file_size - local_start_read;
    local_read_size_plus_buffer = local_read_size;
  }

  read_buffer = (char *)malloc(local_read_size_plus_buffer * sizeof(char));
  if (read_buffer == NULL) {
    perror("Extract Raw Text Buffer Malloc");
    return EXIT_FAILURE;
  }

  /* Perform Actual Read */
  MPI_File_read_at_all(fh, local_start_read, read_buffer,
                       local_read_size_plus_buffer, MPI_CHAR, &status);

  MPI_File_close(&fh);

  /* Remove Duplicate Data */
  start_char = 0;
  end_char = 0;
  if (local_read_size > 0) {
    if (rank > 0) {
      while (read_buffer[start_char] != '\n') {
        start_char = start_char + 1;
      }
    }

    end_char = local_read_size;
    while (end_char < local_read_size_plus_buffer &&
           read_buffer[end_char] != '\n') {
      end_char = end_char + 1;
    }
  }

  /* Copy The String Out */
  *raw_text = (char *)malloc((end_char - start_char) * sizeof(char));
  if (raw_text == NULL) {
    perror("Extract Raw Text Buffer Malloc");
    return EXIT_FAILURE;
  }
  strncpy(*raw_text, read_buffer, end_char - start_char);

  free(read_buffer);

  return error_value;
}

/******************************************************************************/
int PMM_ExtractData(char * raw_text, PMM_Header header,
                    PMM_Data *data) {
  char *temporary_line;
  char *search_pointer;
  int error_value = EXIT_SUCCESS;
  long int i;

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
  if (header.data_type == REAL || header.data_type == INTEGER) {
    data->values = (double *)malloc(data->number_of_values * sizeof(double));
  } else if (header.data_type == COMPLEX) {
    data->values =
        (double *)malloc(2 * data->number_of_values * sizeof(double));
  }
  if (header.data_type != PATTERN && data->values == NULL) {
    perror("Extract Data Malloc");
    return EXIT_FAILURE;
  }

  /* Extract The Data */
  search_pointer = raw_text;
  temporary_line = strtok(search_pointer, "\n");
  for (i = 0; i < data->number_of_values; ++i) {
    if (header.format == COORDINATE) {
      if (header.data_type == REAL || header.data_type == INTEGER) {
        sscanf(temporary_line, "%ld %ld %lf", &(data->rows[i]),
               &(data->columns[i]), &(data->values[i]));
      } else if (header.data_type == COMPLEX) {
        sscanf(temporary_line, "%ld %ld %lf %lf", &(data->rows[i]),
               &(data->columns[i]), &(data->values[2 * i]),
               &(data->values[2 * i + 1]));
      } else if (header.data_type == PATTERN) {
        sscanf(temporary_line, "%ld %ld", &(data->rows[i]),
               &(data->columns[i]));
      }
    } else if (header.format == ARRAY) {
      if (header.data_type == REAL || header.data_type == INTEGER) {
        sscanf(temporary_line, "%lf", &(data->values[i]));
      } else if (header.data_type == COMPLEX) {
        sscanf(temporary_line, "%lf %lf", &(data->values[2 * i]),
               &(data->values[2 * i + 1]));
      }
    }
    temporary_line = strtok(NULL, "\n");
  }

  return error_value;
}
