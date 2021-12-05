# Copyright @yucwang 2021

import numpy as np

class NewtonMethodSolver():
    def __init__(self, function, alpha, beta, A=None, b=None, epsilon=1e-3):
        self.F = function
        self.alpha = alpha
        self.beta = beta
        self.epsilon = epsilon
        self.A = A
        self.b = b

    def run(self, initial_point):
        x = initial_point
        gradient = self.F.gradient(x)
        hessian = self.F.hessian(x)
        iteration = 0
        while self.newton_decrement(gradient, hessian) > self.epsilon:
            gradient = self.F.gradient(x)
            hessian = self.F.hessian(x)
            delta_x = self.newton_step(gradient, hessian)
            t = self.backtrack_search(x, gradient, delta_x)
            x = x + t * delta_x
            iteration += 1
            if iteration % 8 == 0:
                print(delta_x)
                print(t)
                print(self.newton_decrement(gradient, hessian))

        print(iteration)

        return x
    
    def newton_step(self, gradient, hessian):
        if self.A is None:
            return -1. * np.matmul(np.linalg.inv(hessian), gradient)
        else:
            dim = self.A.shape[0] + self.A.shape[1]
            p = self.A.shape[0]
            n = self.A.shape[1]
            B = np.zeros((dim, dim))
            B[0:n, 0:n] = hessian
            B[0:n, n:n+p] = np.transpose(self.A)
            B[n:n+p, 0:n] = self.A
            c = np.zeros(dim)
            c[0:n] = -1. * gradient
            ret = np.linalg.solve(B, c)
            return ret[0:n]

    def newton_decrement(self, gradient, hessian):
        if self.A is None:
            lambda_x = -np.dot(gradient, self.newton_step(gradient, hessian))
            # print(lambda_x)
            return np.absolute(lambda_x / 2.)
        else:
            lambda_x = -np.dot(gradient, self.newton_step(gradient, hessian))
            return lambda_x / 2.

    def backtrack_search(self, cur_point, cur_gradient, delta_x):
        alpha = self.alpha
        beta = self.beta

        t = 1.0
        direction = delta_x
        cur_value = self.F.evaluate(cur_point) + alpha * t * np.dot(cur_gradient, direction)
        iter_value = self.F.evaluate(cur_point + t * direction)
        while iter_value is None or  iter_value > cur_value:
            t = beta * t
            iter_value = self.F.evaluate(cur_point + t * direction)
            cur_value = self.F.evaluate(cur_point) + alpha * t * np.dot(cur_gradient, direction)

        return t
