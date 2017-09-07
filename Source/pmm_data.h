/*! \file mmdata.h
 \brief This file describes the format of the data this library returns.
*/
#ifndef PMM_DATA_h
#define PMM_DATA_h

/******************************************************************************/
/*!
  \struct PMM_Data
  A data structure containing the files data.
  Whether or not the rows, columns, or values arrays have valid data depends
  on the type of matrix that was read in. For example, if it is a pattern
  data file, the values portion holds no data. You can use the MMHeader to
  determine this.
  When complex values are read in, the real and complex part are stored
  one after another in the values array.
*/
typedef struct {
  /*! The number of values being stored.*/
  long int number_of_values;
  /*! Row values. */
  long int* rows;
  /*! Column values. */
  long int* columns;
  /*! Value at a given row and column.
      If the data is complex, the real and imaginary part are stored one after
      another.
  */
  double* values;
} PMM_Data;

/******************************************************************************/
/* ! Initialize the PMM_data structure.
   \param data the data object to initialize.
*/
void InitializePMM_Data(PMM_Data* data);

/******************************************************************************/
/*! Cleanup the PMM_Data structure by freeing the dynamic memory.
  \param data the data object to cleanup.
*/
void CleanupPMM_Data(PMM_Data* data);

#endif
