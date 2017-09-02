/*! \file pmm_write_routines.h
 \brief The core routines you need to call to write the matrix market file.
*/
#ifndef WRITEROUTINES_h
#define WRITEROUTINES_h

#include "pmm_header.h"
#include "pmm_data.h"
#include <mpi.h>

/*************************************************************************/
/*! Write out a matrix market file.
  \param file_name the name of the file to write to.
  \param header the matrix market header data.
  \param data the matrix market data.
  \param comm the mpi communicator which we're using.
*/
int PMM_WriteData(char *file_name, PMM_Header header, MPI_Comm comm, \
  PMM_Data data);

#endif
