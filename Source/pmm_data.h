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
  int number_of_values;
  /*! Row values. */
  int* rows;
  /*! Column values. */
  int* columns;
  /*! Value at a given row and column.
      If the data is complex, the real and imaginary part are stored one after
      another.
  */
  float* values;
} PMM_Data;

#endif
