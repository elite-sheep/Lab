# Copyright @yucwang 2021

import numpy as np
from gradient_method import GradientMethodSolver
from newton_method import NewtonMethodSolver

class InequalityCenterFunction:
    def __init__(self, n):
        self.n = n
        self.A = np.identity(n)

    def evaluate(self, x):
        if x.shape[0] != self.n:
            print("Warning: Input shape not matching")
            return np.zeros(self.n)
        n = self.n
        # check if x is in the domain
        term2 = 0.0
        for i in range(n):
            if np.absolute(x[i]) >= 1:
                return None
            else:
                term2 = term2 + np.log(1 - x[i]**2)

        Ax = np.matmul(self.A, x)
        for i in range(n):
            if np.absolute(Ax[i]) >= 1:
                return None

        term1 = np.log(1 - Ax)
        term1 = term1.sum()
        return -1.0 * term1 - term2

    def gradient(self, x):
        n = self.n
        m = n
        Ax = np.matmul(self.A, x)
        for i in range(n):
            if np.absolute(Ax[i]) >= 1 or np.absolute(x[i]) >= 1:
                return None

        ret = np.zeros(n)
        for i in range(n):
            ret[i] = ret[i] + (2. * x[i] / (1. - x[i]**2))
            for j in range(m):
                ret[i] = ret[i] + self.A[j][i] / (1. - np.dot(self.A[j], x))

        return ret

    def hessian(self, x):
        n = self.n
        m = n

        Ax = np.matmul(self.A, x)
        for i in range(n):
            if np.absolute(Ax[i]) >= 1 or np.absolute(x[i]) >= 1:
                return None

        ret = np.zeros((n, n))
        for i in range(n):
            for j in range(n):
                for k in range(m):
                    ret[i][j] = ret[i][j] + (self.A[k][i] * self.A[k][j] / (1 - np.dot(self.A[k], x))**2)
                    if i == j:
                        ret[i][j] = ret[i][j] + (self.A[k][i]**2 / (1 - x[i])**2)
                        ret[i][j] = ret[i][j] + (self.A[k][i]**2 / (1 + x[i])**2)

        return ret


f = InequalityCenterFunction(3)
solver1 = NewtonMethodSolver(f, 0.1, 0.5, epsilon=1e-6)
xx = solver1.run(np.zeros(3))
print(xx)
h = f.hessian(np.zeros(3))
print(h)
print(np.matmul(np.linalg.inv(h), f.gradient(np.zeros(3))))
