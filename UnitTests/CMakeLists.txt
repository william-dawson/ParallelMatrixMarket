if(BUILD_TESTING)
################################################################################
  find_package(MPI REQUIRED)
  include_directories(${MPI_INCLUDE_PATH})

  include_directories(${CMAKE_SOURCE_DIR}/Source/)
  add_executable(TestDriver test_driver.c)
  target_link_libraries (TestDriver ParallelMatrixMarket ${MPI_LIBRARIES})
  target_link_libraries(TestDriver m)
  set_target_properties(TestDriver PROPERTIES COMPILE_FLAGS
    "${MPI_COMPILE_FLAGS}")

  set_target_properties(TestDriver PROPERTIES LINK_FLAGS
    "${MPI_LINK_FLAGS}")

################################################################################
  set(TESTEXEC ${CMAKE_BINARY_DIR}/bin/TestDriver)
  configure_file(runtest.sh
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/runtest.sh @ONLY)
  configure_file(compare_matrices.py compare_matrices.py @ONLY)
  configure_file(make_test_data.py make_test_data.py @ONLY)

################################################################################
  find_program(BASH_PROGRAM bash)
  add_test(test1 ${BASH_PROGRAM} ${CMAKE_BINARY_DIR}/bin/runtest.sh 1)
  add_test(test2 ${BASH_PROGRAM} ${CMAKE_BINARY_DIR}/bin/runtest.sh 2)
  add_test(test3 ${BASH_PROGRAM} ${CMAKE_BINARY_DIR}/bin/runtest.sh 3)
  add_test(test4 ${BASH_PROGRAM} ${CMAKE_BINARY_DIR}/bin/runtest.sh 4)
  add_test(test5 ${BASH_PROGRAM} ${CMAKE_BINARY_DIR}/bin/runtest.sh 5)
  add_test(test6 ${BASH_PROGRAM} ${CMAKE_BINARY_DIR}/bin/runtest.sh 6)
  add_test(test7 ${BASH_PROGRAM} ${CMAKE_BINARY_DIR}/bin/runtest.sh 7)

################################################################################
endif(BUILD_TESTING)
