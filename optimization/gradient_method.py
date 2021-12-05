# Copyright @yucwang 2021

import numpy as np

class GradientMethodSolver():
    def __init__(self, function, alpha, beta, epsilon=1e-3):
        self.F = function
        self.alpha = alpha
        self.beta = beta
        self.epsilon = epsilon

    def run(self, initial_point):
        x = initial_point
        gradient = self.F.gradient(x)
        iteration = 0
        while np.linalg.norm(gradient, 2) > self.epsilon:
            gradient = self.F.gradient(x)
            t = self.backtrack_search(x, gradient)
            x = x - t * gradient
            iteration += 1
            if iteration % 8 == 0:
                print(t)
                print(x)

        print(iteration)

        return x

    def backtrack_search(self, cur_point, cur_gradient):
        alpha = self.alpha
        beta = self.beta

        t = 1.0
        direction = -cur_gradient
        cur_value = self.F.evaluate(cur_point) + alpha * t * np.dot(cur_gradient, t * direction)
        iter_value = self.F.evaluate(cur_point + t * direction)
        while iter_value == None or  iter_value > cur_value:
            t = beta * t
            iter_value = self.F.evaluate(cur_point + t * direction)
            cur_value = self.F.evaluate(cur_point) + alpha * t * np.dot(cur_gradient, direction)

        return t
