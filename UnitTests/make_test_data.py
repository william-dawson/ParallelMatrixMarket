# @file a script to create some test matrices.

from scipy.io import mmwrite
from numpy.random import rand
from scipy.sparse import csr_matrix
from sys import argv

if __name__ == "__main__":
    matrix_dimension = int(argv[1])
    format_type = argv[2]
    data_type = argv[3]
    symmetry_type = argv[4]
    file_name = argv[5]

    test_matrix = csr_matrix(rand(matrix_dimension, matrix_dimension))

    mmwrite(target=file_name, a=test_matrix, field=data_type)
