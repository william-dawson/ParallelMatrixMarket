/*! \file mmheader.h
 \brief This file describes the format of the header info read by this library.
*/
#ifndef MMHEADER_h
#define MMHEADER_h

#include <stdlib.h>
#include <mpi.h>

/*!
  \enum MMFormat
  Whether or not the matrix is a dense array or sparse coordinate file.
*/
typedef enum { COORDINATE, ARRAY } MMFormat;
/*!
  \enum MMDataType
  What type of values this matrix holds.
*/
typedef enum { REAL, INTEGER, COMPLEX, PATTERN } MMDataType;
/*!
  \enum MMSymmetry
  Whether this is a symmetric matrix or not.
*/
typedef enum { GENERAL, SYMMETRIC, SKEWSYMMETRIC, HERMITIAN } MMSymmetry;

/* \struct MMHeader
   A data structure containing the header information from a file.
*/
typedef struct {
  MMFormat format;
  MMDataType data_type;
  MMSymmetry symmetric;
  int matrix_rows;
  int matrix_columns;
  long int total_elements;
  size_t header_length;
} MMHeader;

void PrintHeader(MMHeader header);

#endif
