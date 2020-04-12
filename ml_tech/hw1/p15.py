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

def main():
    X, Y = loadData('./data/features.train')
    XTest, YTest = loadData('./data/features.test')
    logGamma = [0, 1, 2, 3, 4]
    positiveMask = Y == 0.0
    negativeMask = Y != 0.0
    Y[positiveMask] = 1.0
    Y[negativeMask] = -1.0

    positiveMaskTest = YTest == 0.0
    negativeMaskTest = YTest != 0.0
    YTest[positiveMaskTest] = 1.0
    YTest[negativeMaskTest] = -1.0

    C = 0.1
    
    eouts = []
    for gamma in logGamma:
        solver = SVMSolver(C=C, kernel='rbf', degree=2, gamma=10**gamma, coef0=0.0, shrinking=True)
        solver.train(X, Y)
        score = solver.valid(XTest, YTest)
        eouts.append(1.0 - score)

    fig, ax = plt.subplots()
    ax.plot(np.array(logGamma), np.array(eouts))
    ax.set(xlabel='log10(Gamma)', ylabel='Eout', title='Eout to log10(Gamma)')
    ax.grid()
    fig.savefig("p15.png")

main()
