# Project Overview

This is a minimal library which can be used for reading and writing the matrix
market format in parallel using MPI.

## How To Build

The requirements for this library are:

* A C Compiler.
* An MPI Implementation.
* CMake.

Compilation is performed using CMake. Create a build directory, change
directories to it, and use the cmake command to setup the build.

> mkdir Build

> cd Build

> cmake ..

You can then compile with

> make

And build documentation with

> make doc

## How To Use

There are two data structures and four subroutines you need to know to use
this library. First, there is the PMM_Header() data structure, which is used to
store a general description of the file you are reading or writing. If you
are reading a file, you first must fill this data structure using the
PMM_ReadHeader() command. If you are writing a file, you must first fill this
data structure out using the PMM_CreateHeader() routine.

After the header is read/setup, you can now handle the data. The matrix data
is stored in an PMM_Data struct. You can fill this from file using the
PMM_ReadData() command, and write out this data using the PMM_WriteData()
command.
