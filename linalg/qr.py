# Copyright @yucwang 2021

import numpy as np

# Input: A matrix \in R^{m*n}
# Output: Two matrics QR, Q \in R^{m*n} and R \in R^{n*n}
def modified_gs(A):
    m = A.shape[0]
    n = A.shape[1]
    V = A.copy().astype(np.float64)
    Q = np.zeros(shape=A.shape, dtype=np.float64)
    R = np.zeros(shape=(n, n), dtype=np.float64)

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

def house(A):
    m = A.shape[0]
    n = A.shape[1]
    R = A.copy().astype(np.float64)
    W = np.zeros(shape=A.shape, dtype=np.float64)
    im = np.identity(m, dtype=np.float64)

    for k in range(n):
        x = R[k:m, k]
        vk = np.sign(x[0]) * np.linalg.norm(x, ord=2) * im[k:m, k] + x
        vk_norm = np.linalg.norm(vk, ord=2)
        vk = vk / vk_norm
        R[k:m,k:n] = R[k:m,k:n] - 2. * np.matmul(np.outer(vk, vk), R[k:m,k:n])
        W[k:m, k] = vk

    return W, R

def form_q(W):
    m = W.shape[0]
    n = W.shape[1]
    Q = np.identity(m, dtype=np.float64)

    for i in range(n):
        wi = W[i:m:,i]
        F = np.identity(m-i) - 2. * np.outer(wi, wi)
        Qi = np.identity(m)
        Qi[i:m,i:m] = F
        Q = np.matmul(Q, Qi)

    return Q

def household(A):
    W, R = house(A)
    Q = form_q(W)
    return Q, R

def qr(A, mode='household'):
    if mode == 'household':
        Q, R = household(A)
        return Q, R
    elif mode == "mgs":
        Q, R = modified_gs(A)
        return Q, R

if __name__ == "__main__":
    A = np.array([[1.0, 2.0, 3.0], [4.0, 5.0, 6.0], [7.0, 8.0, 7.0], [4.0, 2.0, 3.0], [4.0, 2.0, 2.0]])
    # A = np.array([[1.0, 2.0], [0.0, 1.0], [1.0, 0.0]])
    Q, R = qr(A, 'mgs')
    print("*********MGS**********")
    print(Q)
    print(R)
    Q, R = qr(A, 'household')
    print("*********HOUSE**********")
    print(Q)
    print(R)
    Q, R = np.linalg.qr(A)
    print("*********REF**********")
    print(Q)
    print(R)
