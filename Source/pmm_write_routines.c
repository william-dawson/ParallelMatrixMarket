/*! \file pmm_write_routines.c
 \brief Implementation of write to file routines.
*/
#include "pmm_write_routines.h"

#include <mpi.h>
#include <stdlib.h>

/******************************************************************************/
int PMM_CreateHeaderText(PMM_Header header, char *header_text);
int PMM_CreateDataText(PMM_Header header, PMM_Data data, char *data_text);
int PMM_PerformWrite(char *file_name, char *header_text, char *data_text,
                      MPI_Comm comm);

/******************************************************************************/
int PMM_WriteData(char *file_name, PMM_Header header, MPI_Comm comm,
                  PMM_Data data) {
  char *header_text;
  char *data_text;
  int error_value = EXIT_SUCCESS;

  PMM_CreateHeaderText(header, header_text);

  PMM_CreateDataText(header, data, data_text);

  PMM_PerformWrite(file_name, header_text, data_text, comm);

  return error_value;
}

/******************************************************************************/
int PMM_CreateHeaderText(PMM_Header header, char *header_text) {}

/******************************************************************************/
int PMM_CreateDataText(PMM_Header header, PMM_Data data, char *data_text) {}

/******************************************************************************/
int PMM_PerformWrite(char *file_name, char *header_text, char *data_text,
                      MPI_Comm comm) {
  MPI_File fh;
  MPI_Info info;
  /* Compute Offsets*/

  /* Actual Write */
  MPI_File_open(comm, file_name, MPI_MODE_WRONLY | MPI_MODE_CREATE, info, &fh);

  MPI_File_close(&fh);
}
