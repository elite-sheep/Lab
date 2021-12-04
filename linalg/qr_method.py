# Copyright @yucwang 2021

import numpy as np
import scipy.linalg
import matplotlib.pyplot as plt

from matrix import hilb
from qr import qr

def hessenberg(A):
    m = A.shape[0]
    A_copy = A.copy().astype(np.float64)
    im = np.identity(m, dtype=np.float64)

    for k in range(m-2):
        x = A_copy[k+1:m, k]
        vk = np.sign(x[0]) * np.linalg.norm(x, ord=2) * im[k+1:m, k+1] + x
        vk_norm = np.linalg.norm(vk, ord=2)
        vk = vk / vk_norm
        vk = vk.reshape((vk.shape[0], 1))
        vk_transpose = np.transpose(vk)
        A_copy[k+1:m, k:m] = A_copy[k+1:m, k:m] - 2.0 * np.matmul(vk, np.matmul(vk_transpose, A_copy[k+1:m, k:m]))
        A_copy[0:m,k+1:m] = A_copy[0:m,k+1:m] - 2.0 * np.matmul(np.matmul(A_copy[0:m,k+1:m], vk), vk_transpose)

    return A_copy

def qralg(T, eps=1e-12, save_sawtooth=True):
    m = T.shape[0]
    T_new = T.copy().astype(np.float64)
    precision_error = 1e12
    precision_error_record = []

    while precision_error >= eps:
        Q, R = qr(T_new)
        T_new = np.matmul(R, Q)
        precision_error = np.abs(T_new[m-1, m-2])
        precision_error_record.append(precision_error)

    if save_sawtooth == True:
        x = np.linspace(0, len(precision_error_record), len(precision_error_record))
        plt.plot(x, precision_error_record)
        plt.title('Sawtooth plot when m = ' + str(m))
        plt.xlabel('iteration')
        plt.ylabel('|T[m, m-1]|')
        plt.savefig('m'+str(m)+'.png')
        plt.clf()

    return T_new

def eigen_values(T, eps=1e-12):
    m = T.shape[0]
    T_new = T.copy()
    eigens = np.zeros(m)

    for k in range(m-1):
        T_new = qralg(T_new[0:m-k, 0:m-k])
        eigens[m-k-1] = T_new[-1, -1]

    eigens[0] = T_new[0, 0]
    return np.array(eigens)

if __name__ == '__main__':
    A = hilb(4)
    # B = scipy.linalg.hessenberg(A)
    H = hessenberg(A)
    T = qralg(H)
    # H = qralg(A)
    h, v = np.linalg.eig(A)
    h1 = eigen_values(hessenberg(A))
    # print(B)
    #print(H)
    print(T)
    print(h)
    print(h1)
