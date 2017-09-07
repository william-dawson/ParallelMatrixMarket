/*! \file pmm_data.c
 \brief Implementation of PMM_Data data structure helpers.
*/

#include "pmm_data.h"
#include <stdlib.h>

/******************************************************************************/
void InitializePMM_Data(PMM_Data* data)
{
  data->rows    = 0;
  data->columns = 0;
  data->values  = 0;
}

/******************************************************************************/
void CleanupPMM_Data(PMM_Data* data) {
  if (data->rows    != NULL) free(data->rows);
  if (data->columns != NULL) free (data->columns);
  if (data->values  != NULL) free (data->values);
}
