# Copyright 2020 Yuchen Wong

from sklearn.svm import SVC
import numpy as np

class SVMSolver:
    def __init__(self, C, kernel, degree=3, gamma='scale', coef0=0.0, shrinking=False):
        self.C = C
        self.kernel = kernel
        self.degree = degree
        self.gamma = gamma
        self.coef0 = coef0
        self.shrinking = shrinking
        self.model = SVC(C=C, kernel=kernel, degree=degree, 
                gamma=gamma, coef0=coef0, shrinking=shrinking)
        self.isTrained = False

    def train(self, X, Y):
        self.model.fit(X, Y)
        self.isTrained = True

    def valid(self, X, Y):
        return self.model.score(X, Y)

    def getW(self):
        if self.isTrained == False:
            return 0.0

        if self.kernel == 'linear':
            return self.model.coef_
        elif self.kernel == 'poly':
            w = np.zeros(2)
            numSupportVectors = self.getSupportVectorNum()
            sv = self.model.support_vectors_
            coefs = self.model.dual_coef_
            for i in range(numSupportVectors):
                w += coefs[0][i] * sv[i]
            return w

    def getB(self):
        return self.model.intercept_

    def getSupportVectorNum(self):
        if self.isTrained == False:
            return 0

        return np.sum(self.model.n_support_)

    def getSupportIndices(self):
        return self.model.support_

    def getDualCoefs(self):
        return self.model.dual_coef_
