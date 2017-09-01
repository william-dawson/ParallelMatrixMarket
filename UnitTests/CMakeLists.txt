################################################################################
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(TestDriver test_driver.c)
target_link_libraries (TestDriver ParallelMatrixMarket ${MPI_LIBRARIES})

set_target_properties(TestDriver PROPERTIES COMPILE_FLAGS
  "${MPI_COMPILE_FLAGS}")

set_target_properties(TestDriver PROPERTIES LINK_FLAGS
  "${MPI_LINK_FLAGS}")
