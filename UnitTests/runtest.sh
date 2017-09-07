# Get Parameters
if [ "$#" -ne 1 ]
then
  echo "Illegal number of parameters"
  exit
fi
export PROCESSES="$1"

@PYTHON_EXECUTABLE@ make_test_data.py 16 real
@MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ $PROCESSES @TESTEXEC@ \
  @CMAKE_BINARY_DIR@/scratch/test-real.mtx \
  @CMAKE_BINARY_DIR@/scratch/test-real-out.mtx
@PYTHON_EXECUTABLE@ compare_matrices.py \
  @CMAKE_BINARY_DIR@/scratch/test-real.mtx \
  @CMAKE_BINARY_DIR@/scratch/test-real-out.mtx

# @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ $PROCESSES @TESTEXEC@ \
#   scratch/test-complex.mtx scratch/test-complex-out.mtx
# @PYTHON_EXECUTABLE@ compare_matrices.py \
#   scratch/test-complex.mtx scratch/test-complex-out.mtx
#
# @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ $PROCESSES @TESTEXEC@ \
#   scratch/test-integer.mtx scratch/test-integer-out.mtx
# @PYTHON_EXECUTABLE@ compare_matrices.py \
#   scratch/test-integer.mtx scratch/test-integer-out.mtx
#
# @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ $PROCESSES @TESTEXEC@ \
#   scratch/test-pattern.mtx scratch/test-pattern-out.mtx
# @PYTHON_EXECUTABLE@ compare_matrices.py \
#   scratch/test-pattern.mtx scratch/test-pattern-out.mtx
