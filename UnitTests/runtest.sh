# Get Parameters
set -o xtrace
set -e

if [ "$#" -ne 1 ]
then
  echo "Illegal number of parameters"
  exit
fi
export PROCESSES="$1"

matrix_size=4

parameters=(\
  "coordinate real general" \
  "coordinate complex general" \
  "coordinate integer general" \
  "coordinate pattern general" \
  "coordinate real symmetric" \
  "coordinate real skew-symmetric" \
  "coordinate complex hermitian" \
  "array real general" \
  "array real symmetric" \
)

for param in "${parameters[@]}"
do
  paramarray=($param)
  ft=${paramarray[0]}
  dt=${paramarray[1]}
  st=${paramarray[2]}

  @PYTHON_EXECUTABLE@ -W ignore make_test_data.py $matrix_size \
    ${ft} ${dt} ${st} \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-in.mtx
  sleep 1
  @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ $PROCESSES @TESTEXEC@ \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-in.mtx \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-out.mtx
  sleep 1
  @PYTHON_EXECUTABLE@ -W ignore compare_matrices.py \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-in.mtx \
    @CMAKE_BINARY_DIR@/scratch/test-${PROCESSES}-${ft}-${dt}-${st}-out.mtx
done
