# @file a script to compare to matrix market files.

from sys import argv, exit, stderr
from scipy.io import mmread
from scipy.linalg import norm

if __name__ == "__main__":
    print("Comparing Matrices")
    matrix1 = mmread(argv[1])
    matrix2 = mmread(argv[2])

    try:
        matrix1 = matrix1.todense()
    except:
        matrix1 = matrix1
    try:
        matrix2 = matrix2.todense()
    except:
        matrix2 = matrix2

    if norm(matrix1 - matrix2) > 1e-10:
        stderr.write("Comparison failed.\n")
        stderr.write("Error value:"+str(norm(matrix1-matrix2)))
        stderr.write(argv[1]+"\n")
        stderr.write(argv[2]+"\n")
        print(matrix1)
        print(matrix2)
        exit(-1)
