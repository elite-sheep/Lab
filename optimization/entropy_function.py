# Copyright @yucwang 2021

import numpy as np
from autograd import grad
from newton_method import NewtonMethodSolver

class CrossEntropy():
    def __init__(self, n):
        self.n = n

    def evaluate(self, x):
        for i in range(self.n):
            if x[i] <= 0.:
                return None

        return np.dot(x, np.log(x))

    def gradient(self, x):
        for i in range(self.n):
            if x[i] <= 0.:
                return None

        return np.log(x) + 1.

    def hessian(self, x):
        for i in range(self.n):
            if x[i] <= 0.:
                return None

        ret = np.zeros((self.n, self.n))
        for i in range(self.n):
            ret[i][i] = 1. / x[i]

        return ret

class CrossEntropyLagrange():
    def __init__(self, A, b, n, p):
        self.A = A
        self.b = b
        self.n = n
        self.p = p

    def evaluate(self, v):
        A_T = np.transpose(A)
        ret = -np.dot(v, self.b)
        for i in range(self.n):
            ret = ret - np.exp(np.dot(A_T[i], -v) - 1.)

        return -1. * ret

    def gradient(self, v):
        A_T = np.transpose(A)
        ret = np.zeros(self.p)
        for i in range(self.p):
            ret[i] = -self.b[i]
            for j in range(self.n):
                ret[i] = ret[i] + A_T[j][i] * np.exp(np.dot(A_T[j], -v) - 1.)

        return -1. * ret

    def hessian(self, v):
        A_T = np.transpose(A)
        ret = np.zeros((self.p, self.p))
        for i in range(self.p):
            for j in range(self.p):
                for k in range(self.n):
                    ret[i][j] = ret[i][j] - A_T[k][i] * A_T[k][j] * np.exp(np.dot(A_T[k], -v) - 1.)

        return -1. * ret

class CrossEntropyDual():
    def __init__(self, A, v, n, p):
        self.base = CrossEntropy(n)
        self.A = A
        self.v = v
        self.n = n
        self.p = p

    def evaluate(self, x):
        ret = self.base.evaluate(x)
        if ret is None:
            return None
        for i in range(self.p):
            ret = ret + np.dot(self.A[i], x) * self.v[i]

        return ret

    def gradient(self, x):
        ret = self.base.gradient(x)
        if ret is None:
            return None
        for i in range(self.n):
            for j in range(self.p):
                ret = ret + self.A[j][i] * self.v[j]

        return ret

    def hessian(self, x):
        return self.base.hessian(x)

with open("./q2_1.npy", "rb") as f:
    A = np.load(f)
    x = np.load(f)
    b = np.load(f)
    func_1 = CrossEntropyLagrange(A, b, 100, 30)
    solver1 = NewtonMethodSolver(func_1, 0.1, 0.5, epsilon=1e-6)
    v = solver1.run(np.zeros(30))

    xx = np.zeros(100)
    for i in range(100):
        xx[i] = -1.
        for j in range(30):
            xx[i] = xx[i] - A[j][i] * v[j]
    xx = np.exp(xx)

    # print(v)
    # f_2 = CrossEntropyDual(A, v, 100, 30)
    # solver2 = NewtonMethodSolver(f_2, 0.1, 0.5, epsilon=1e-6)
    # xx = solver2.run(x)
    func_2 = CrossEntropy(100)
    # print(f.evaluate(x))
    # print(f.gradient(x))
    solver1 = NewtonMethodSolver(func_2, 0.1, 0.5, A=A, b=b, epsilon=1e-6)
    xx2 = solver1.run(x)
    # print(x)
    print(xx)
    print(func_2.evaluate(xx))
    print(xx2)

    print(np.matmul(A, xx))
    # print(xx2)
    print(b)
