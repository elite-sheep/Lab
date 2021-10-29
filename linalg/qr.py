# Copyright @yucwang 2021

import numpy as np

# Input: A matrix \in R^{m*n}
# Output: Two matrics QR, Q \in R^{m*n} and R \in R^{n*n}
def modified_gs(A):
    m = A.shape[0]
    n = A.shape[1]
    V = A.copy().astype(np.float32)
    Q = np.zeros(shape=A.shape, dtype=np.float32)
    R = np.zeros(shape=(n, n), dtype=np.float32)

    for i in range(n):
        vi = V[:,i]
        vi_norm = np.linalg.norm(vi, ord=2)
        R[i, i] = vi_norm
        Q[:,i] = vi / vi_norm
        qi = Q[:,i]
        for j in range(i+1, n, 1):
            vj = V[:,j]
            R[i, j] = np.dot(qi, vj)
            vj = np.subtract(vj, qi * R[i, j])
            V[:,j] = vj

    return Q, R

if __name__ == "__main__":
    A = np.array([[0.7000, 0.70711], [0.70001, 0.70711]])
    Q, R = modified_gs(A)
    ret = np.matmul(Q.transpose(), Q) - np.identity(2, dtype=np.float32)
    print(Q)
    print(np.linalg.norm(ret, ord=2))
    Q, R = np.linalg.qr(A)
    ret = np.matmul(Q.transpose(), Q) - np.identity(2, dtype=np.float32)
    print(Q)
    print(np.linalg.norm(ret, ord=2))
