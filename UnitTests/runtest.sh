# Get Parameters
if [ "$#" -ne 1 ]
then
  echo "Illegal number of parameters"
  exit
fi
export PROCESSES="$1"

matrix_size=16

formats=("coordinate", "array")
types=("real" "complex" "integer" "pattern")
symmetry=("general", "symmetric", "skewsymmetric", "hermitian")

parameters=("coordinate real general" \
            "coordinate complex general" \
            "coordinate integer general" \
            "coordinate pattern general")

for param in "${parameters[@]}"
do
  paramarray=($param)
  ft=${paramarray[0]}
  dt=${paramarray[1]}
  st=${paramarray[2]}

  rm @CMAKE_BINARY_DIR@/scratch/test-${ft}-${dt}-${st}-in.mtx
  rm @CMAKE_BINARY_DIR@/scratch/test-${ft}-${dt}-${st}-out.mtx
  @PYTHON_EXECUTABLE@ -W ignore make_test_data.py $matrix_size \
    {$ft} ${dt} ${st} \
    @CMAKE_BINARY_DIR@/scratch/test-${ft}-${dt}-${st}-in.mtx
  @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ $PROCESSES @TESTEXEC@ \
    @CMAKE_BINARY_DIR@/scratch/test-${ft}-${dt}-${st}-in.mtx \
    @CMAKE_BINARY_DIR@/scratch/test-${ft}-${dt}-${st}-out.mtx
  @PYTHON_EXECUTABLE@ -W ignore compare_matrices.py \
    @CMAKE_BINARY_DIR@/scratch/test-${ft}-${dt}-${st}-in.mtx \
    @CMAKE_BINARY_DIR@/scratch/test-${ft}-${dt}-${st}-out.mtx
done
