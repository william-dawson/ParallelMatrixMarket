# @file a script to create some test matrices.

from scipy.io import mmwrite
from numpy.random import rand
from scipy.sparse import csr_matrix
from sys import argv

if __name__ == "__main__":
    matrix_dimension = int(argv[1])
    matrix_type = argv[2]
    path = "@CMAKE_BINARY_DIR@/scratch/test-"
    test_matrix = csr_matrix(rand(matrix_dimension, matrix_dimension))
    mmwrite(target=path + matrix_type + ".mtx",
            a=test_matrix, field=matrix_type)
