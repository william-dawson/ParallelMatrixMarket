# @file a script to compare to matrix market files.

from sys import argv, exit, stderr
from scipy.io import mmread
from scipy.linalg import norm

if __name__ == "__main__":
    matrix1 = mmread(argv[1])
    matrix2 = mmread(argv[2])

    if norm(matrix1.todense() - matrix2.todense()) > 1e-10:
        stderr.write("Comparison failed.\n")
        stderr.write(argv[1]+"\n")
        stderr.write(argv[2]+"\n")
        exit(1)
