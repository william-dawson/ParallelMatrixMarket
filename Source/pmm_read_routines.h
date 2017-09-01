/*! \file pmm_read_routines.h
 \brief The core routines you need to call to read the matrix market file.
*/
#ifndef READROUTINES_h
#define READROUTINES_h

#include "pmm_data.h"
#include "pmm_header.h"
#include <stdlib.h>

/*! PMM_ReadHeader
  Read in the header information of the matrix market file.
  \param file_name the name of the file to read.
  \param header the matrix market header data structure to store the result.
  \param comm the mpi communicator which we're using.
*/
int PMM_ReadHeader(char *file_name, MPI_Comm comm, PMM_Header *header);

/*! PMM_ReadData 
  Read in the full data from a matrix market file.
  \param file_name the name of the file to read.
  \param header the matrix market header data computed by ReadHeader.
  \param data the matrix market data which we'll read in.
  \param comm the mpi communicator which we're using.
*/
int PMM_ReadData(char *file_name, PMM_Header header, MPI_Comm comm, \
  PMM_Data* data);

#endif
