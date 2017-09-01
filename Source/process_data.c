////////////////////////////////////////////////////////////////////////////////
#include "process_data.h"
#include <mpi.h>

#include "mmheader.h"
#include "mmdata.h"

////////////////////////////////////////////////////////////////////////////////
void ExtractRawText(char *file_name, MMHeader* header, MMData* data, \
  MPI_Comm comm, char* raw_text) {
  MPI_File fh;
  MPI_Info info;
  
  MPI_File_open(comm, file_name, MPI_MODE_RDONLY, info, &fh);
  MPI_File_close(&fh);
}

////////////////////////////////////////////////////////////////////////////////
int ReadData(char *file_name, MMHeader *header, MMData * data, MPI_Comm comm) {
  // Get The Raw Text

  // Extract The Data
}
