# @file a script to create some test matrices.

from scipy.io import mmwrite
from numpy.random import rand, randint
from numpy import asmatrix
from scipy.sparse import coo_matrix, csr_matrix
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
        test_matrix = coo_matrix(rand(matrix_dimension, matrix_dimension))
        test_matrix = test_matrix + 1j * test_matrix
    if data_type == "integer":
        test_matrix = coo_matrix(
            randint(100, size=(matrix_dimension, matrix_dimension)))
    if data_type == "real":
        test_matrix = coo_matrix(rand(matrix_dimension, matrix_dimension))
    if data_type == "pattern":
        test_matrix = coo_matrix(
            randint(2, size=(matrix_dimension, matrix_dimension)))

    # Handle symmetry type
    if symmetry_type == "symmetry":
        test_matrix = (test_matrix + test_matrix.T)
    elif symmetry_type == "hermitian":
        test_matrix = (test_matrix + test_matrix.H)
    elif symmetry_type == "skew-symmetric":
        test_matrix = coo_matrix(test_matrix + test_matrix.T)
        for i in range(0, len(test_matrix.data)):
            if test_matrix.row[i] == test_matrix.col[i]:
                test_matrix.data[i] = 0
            elif test_matrix.row[i] < test_matrix.col[i]:
                test_matrix.data[i] = -1.0 * test_matrix.data[i]
        test_matrix = csr_matrix(test_matrix)
        test_matrix.eliminate_zeros()
        test_matrix = coo_matrix(test_matrix)

    # Handle Format Type
    if format_type == "array":
        test_matrix = asmatrix(test_matrix.todense())

    mmwrite(target=file_name, a=test_matrix, precision=10,
            field=data_type, symmetry=symmetry_type)
