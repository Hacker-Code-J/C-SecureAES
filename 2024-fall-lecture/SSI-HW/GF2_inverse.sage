# Define the finite field GF(2)
F = GF(2)

# Define the 8x8 binary matrix A
A = Matrix(F, [
    [1, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 0, 0, 0, 1, 1, 1],
    [1, 1, 1, 0, 0, 0, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 1],
    [1, 1, 1, 1, 1, 0, 0, 0],
    [0, 1, 1, 1, 1, 1, 0, 0],
    [0, 0, 1, 1, 1, 1, 1, 0],
    [0, 0, 0, 1, 1, 1, 1, 1]
])

# Check if the matrix is invertible
if A.is_invertible():
    # Compute the inverse of A
    A_inv = A.inverse()
    print("Inverse matrix A_inv:")
    print(A_inv)
else:
    print("Matrix A is not invertible.")