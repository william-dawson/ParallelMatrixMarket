/*! \file mmheader.h
 \brief This file describes the format of the header info read by this library.
*/
#ifndef PMM_HEADER_h
#define PMM_HEADER_h

#include <stdlib.h>
#include <mpi.h>

/*!
  \enum PMM_Format
  Whether or not the matrix is a dense array or sparse coordinate file.
*/
typedef enum { COORDINATE, ARRAY } PMM_Format;
/*!
  \enum PMM_DataType
  What type of values this matrix holds.
*/
typedef enum { REAL, INTEGER, COMPLEX, PATTERN } PMM_DataType;
/*!
  \enum PMM_Symmetry
  Whether this is a symmetric matrix or not.
*/
typedef enum { GENERAL, SYMMETRIC, SKEWSYMMETRIC, HERMITIAN } PMM_Symmetry;

/* \struct PMM_Header
   A data structure containing the header information from a file.
*/
typedef struct {
  /*! > Dense or sparse matrix. */
  PMM_Format format;
  /*! > Kind of data stored in the matrix. */
  PMM_DataType data_type;
  /*! > Is the matrix symmetric? */
  PMM_Symmetry symmetric;
  /*! > Number of rows of the matrix. */
  int matrix_rows;
  /*! > Number of columns of the matrix. */
  int matrix_columns;
  /*! > Total number of elements in the matrix. */
  long int total_elements;
  /*! > Internal use only.*/
  size_t header_length;
} PMM_Header;

#endif
