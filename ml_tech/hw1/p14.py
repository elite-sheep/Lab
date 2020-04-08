# Copyright 2020 Yuchen Wong

import numpy as np
import pandas as pd
from svm import SVMSolver
import matplotlib.pyplot as plt

def loadData(filename):
    data = pd.read_csv(filename, sep='\s+', header=None)
    row, col = data.shape
    print(data.dtypes)
    print(data.shape)
    X = np.empty([row, 2])
    Y = np.empty([row])
    for i in range(row):
        Y[i] = data.iat[i, 0]
        X[i][0] = data.iat[i, 1]
        X[i][1] = data.iat[i, 2]

    return X, Y

def gaussainKernel(x1, x2, gamma):
    x = x1-x2
    return np.exp(-1.0*gamma*np.linalg.norm(x)**2)
    

def main():
    X, Y = loadData('./data/features.train')
    logC = [-3, -2, -1, 0, 1]
    positiveMask = Y == 0.0
    negativeMask = Y != 0.0
    Y[positiveMask] = 1.0
    Y[negativeMask] = -1.0
    gamma = 80.0

    distances = []
    for c in logC:
        solver = SVMSolver(C=10**c, kernel='rbf', degree=2, gamma=gamma, coef0=0.0, shrinking=True)
        solver.train(X, Y)
        svIndices = solver.getSupportIndices()
        dualCoefs = solver.getDualCoefs()
        index = -1
        b = solver.getB()
        for i in range(svIndices.shape[0]):
            if abs(dualCoefs[0][i]) < 10**c:
                index = svIndices[i]
                break

        print(index)
        up = 0.0
        for i in range(svIndices.shape[0]):
            up += dualCoefs[0][i] * gaussainKernel(X[index], X[svIndices[i]], gamma)
        up += b

        print(svIndices.shape[0])
        down = 0.0
        for i in range(svIndices.shape[0]):
            for j in range(svIndices.shape[0]):
                down += dualCoefs[0][i] * dualCoefs[0][j] * gaussainKernel(X[i], X[j], gamma)

        distances.append(abs(up)/np.sqrt(down))
        print(down)
        print(up)

    fig, ax = plt.subplots()
    ax.plot(np.array(logC), np.array(distances))
    ax.set(xlabel='log10(C)', ylabel='Distance between free SV and hyperplane', title='Distance to log10(C)')
    ax.grid()
    fig.savefig("p14.png")

main()
