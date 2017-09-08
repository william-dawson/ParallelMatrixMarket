# @file a script to create some test matrices.

from scipy.io import mmwrite
from numpy.random import rand, randint
from numpy import asmatrix
from scipy.sparse import csr_matrix
from sys import argv

if __name__ == "__main__":
    print("Making Test Matrices")
    # Handle Parameters
    matrix_dimension = int(argv[1])
    format_type = argv[2]
    data_type = argv[3]
    symmetry_type = argv[4]
    file_name = argv[5]

    # Create a base matrix
    if data_type == "complex":
        test_matrix = csr_matrix(rand(matrix_dimension, matrix_dimension))
        test_matrix = test_matrix + 1j * test_matrix
    if data_type == "integer":
        test_matrix = csr_matrix(
            randint(100, size=(matrix_dimension, matrix_dimension)))
    if data_type == "real":
        test_matrix = csr_matrix(rand(matrix_dimension, matrix_dimension))
    if data_type == "pattern":
        test_matrix = csr_matrix(
            randint(1, size=(matrix_dimension, matrix_dimension)))

    # Handle symmetry type
    if data_type == "symmetry":
        test_matrix = (test_matrix + test_matrix.T)
    if data_type == "hermitian":
        test_matrix = (test_matrix + test_matrix.H)

    # Handle Format Type
    if format_type == "array":
        test_matrix = asmatrix(test_matrix.todense())

    mmwrite(target=file_name, a=test_matrix,
            field=data_type, symmetry=symmetry_type)
