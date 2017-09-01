/*! \file pmm_process_data.c
 \brief Implementation of data processing routines.
*/
#include "pmm_read_routines.h"
#include <mpi.h>

#include "pmm_header.h"
#include "pmm_data.h"

/******************************************************************************/
void ExtractRawText(char *file_name, PMM_Header header, \
  MPI_Comm comm, char* raw_text);
void ExtractData(char *raw_text, PMM_Header header, PMM_Data * data);

/******************************************************************************/
int PMM_ReadData(char *file_name, PMM_Header header,  MPI_Comm comm,
  PMM_Data * data) {
  char *raw_text;
  /* Get The Raw Text */
  ExtractRawText(file_name, header, comm, raw_text);

  /* Extract The Data */
  ExtractData(raw_text, header, data);
}

/******************************************************************************/
void ExtractRawText(char *file_name, PMM_Header header, MPI_Comm comm, \
  char* raw_text) {
  MPI_File fh;
  MPI_Info info;
  MPI_Offset file_size;
  MPI_Offset local_read_size;
  MPI_Offset local_read_size_plus_buffer;
  int total_processes;
  int rank;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &total_processes);

  MPI_File_open(comm, file_name, MPI_MODE_RDONLY, info, &fh);
  MPI_File_get_size(fh, &file_size);

  MPI_File_close(&fh);
}

/******************************************************************************/
void ExtractData(char *raw_text, PMM_Header header, PMM_Data * data)
{

}
