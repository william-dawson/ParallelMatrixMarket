////////////////////////////////////////////////////////////////////////////////
#include "process_data.h"
#include <mpi.h>

#include "mmheader.h"
#include "mmdata.h"

////////////////////////////////////////////////////////////////////////////////
void ExtractRawText(char *file_name, MMHeader header, \
  MPI_Comm comm, char* raw_text);
void ExtractData(char *raw_text, MMHeader header, MMData * data);

////////////////////////////////////////////////////////////////////////////////
int ReadData(char *file_name, MMHeader header, MMData * data, MPI_Comm comm) {
  char *raw_text;
  // Get The Raw Text
  ExtractRawText(file_name, header, comm, raw_text);

  // Extract The Data
  ExtractData(raw_text, header, data);
}

////////////////////////////////////////////////////////////////////////////////
void ExtractRawText(char *file_name, MMHeader header, \
  MPI_Comm comm, char* raw_text) {
  MPI_File fh;
  MPI_Info info;
  MPI_Offset file_size;
  MPI_Offset local_read_size;
  MPI_Offset local_read_size_plus_buffer;

  MPI_File_open(comm, file_name, MPI_MODE_RDONLY, info, &fh);
  MPI_File_Get_size(fh, &file_size);

  MPI_File_close(&fh);
}

////////////////////////////////////////////////////////////////////////////////
void ExtractData(char *raw_text, MMHeader header, MMData * data)
{

}
