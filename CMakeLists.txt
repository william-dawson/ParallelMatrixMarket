################################################################################
## Basic Setup
cmake_minimum_required (VERSION 3.8)
project(ParallelMatrixMarket)
set(CMAKE_VERBOSE_MAKEFILE ON)
find_package(MPI REQUIRED)

################################################################################
## Output Locations
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

################################################################################
## Documentation
find_package(Doxygen REQUIRED)
configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in
               ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)

################################################################################
## Get at the source files
subdirs(Source)
subdirs(UnitTests)
