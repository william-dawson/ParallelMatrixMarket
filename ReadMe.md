# Project Overview

This is a minimal library which can be used for reading and writing the matrix
market format in parallel using MPI.

## Feature Set

Warning, this project is currently in an alpha release stage. When using this
library, please perform some independent verification of the results.

Currently we do not support writing the array matrix type, only the coordinate
type. 

## How To Build

The requirements for this library are:

* A C Compiler.
* An MPI Implementation.
* CMake.
* Python + Scipy (Optional, For Unit Tests).

Compilation is performed using CMake. Create a build directory, change
directories to it, and use the cmake command to setup the build.

> mkdir Build

> cd Build

> cmake ..

You can then compile with

> make

And build documentation with

> make doc

## How To Read Data

One first needs two data structures to read from a file. First, a `PMM_Header`
type to store information about the matrix. Second, a `PMM_Data` type to
store the contents of the matrix file. First we read the header information
in from file, by calling `PMM_ReadHeader()`. Next, we setup the `PMM_Data`
type by calling `InitializePMM_Data()`. We can then fill that structure from
the file by calling `PMM_ReadData()`. When the calculation is finished, you
should call `CleanupPMM_Data()` to delete the dynamically allocated memory in
the `PMM_Data` type.

## How To Write Data

To write, just fill in the data and header information into a `PMM_Header`
and `PMM_Data` structure, and then call `PMM_WriteData()` to write out to file.

## Where does the data end up after reading?
After a read operation, each process will hold an independent subset of
the matrix data. There is no guarantee about which matrix row/column will
be stored on which MPI process. After the read operation, it is up to the
user to then redistribute the data according to their application specific
purposes.

## Where does the data need to be before writing?
Similar to reading, for writing each process should hold an independent
subset of the matrix data. As long as there is no duplication, any
process can hold any matrix entry, and the write will be performed.

## Accessing The Data

The data for the matrix is stored in a `PMM_Data` type of a file. The `PMM_Data`
structure has three array members: rows, columns, and values. The rows and
columns store the expected row and column entry. The values is a `void` pointer
array storing the values at a given row and column. Make sure to cast to
a suitable data type based on the information in the `PMM_Header`. For complex
matrices, the real and complex value are stored one after another, so make sure
to access the values pointer as `[2*index]` and `[2*index+1]` to get the real
and complex value.
