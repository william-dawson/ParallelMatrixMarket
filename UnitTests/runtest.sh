# Get Parameters
set -o xtrace
set -e

if [ "$#" -ne 1 ]
then
  echo "Illegal number of parameters"
  exit
fi
export PROCESSES="$1"

parameters=(\
  "2 coordinate real general" \
  "4 coordinate real general" \
  "8 coordinate real general" \
  "128 coordinate real general" \
  "128 coordinate complex general" \
  "128 coordinate integer general" \
  "128 coordinate pattern general" \
  "128 coordinate real symmetric" \
  "128 coordinate real skew-symmetric" \
  "128 coordinate complex hermitian" \
  "128 array real general" \
  "128 array real symmetric" \
)

for param in "${parameters[@]}"
do
  paramarray=($param)
  matrix_size=${paramarray[0]}
  ft=${paramarray[1]}
  dt=${paramarray[2]}
  st=${paramarray[3]}

  @PYTHON_EXECUTABLE@ -W ignore make_test_data.py $matrix_size \
    ${ft} ${dt} ${st} \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-in.mtx
  @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ $PROCESSES @TESTEXEC@ \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-in.mtx \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-out.mtx
  @PYTHON_EXECUTABLE@ -W ignore compare_matrices.py \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-in.mtx \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-out.mtx
done
