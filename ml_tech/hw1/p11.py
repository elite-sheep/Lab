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
    logC = [-5, -3, -1, 1, 3]
    positiveMask = Y == 8.0
    negativeMask = Y != 8.0
    Y[positiveMask] = 1.0
    Y[negativeMask] = -1.0
    
    scores = []
    numSupportVectors = []
    for c in logC:
        solver = SVMSolver(C=10**c, kernel='poly', degree=2, gamma=1, coef0=1.0, shrinking=True)
        solver.train(X, Y)
        score = solver.valid(X, Y)
        scores.append(1.0 - score)
        numSupportVectors.append(solver.getSupportVectorNum())
        print(score)
        print(solver.getSupportVectorNum())

    fig, ax = plt.subplots()
    ax.plot(np.array(logC), np.array(scores))
    ax.set(xlabel='log10(C)', ylabel='Ein', title='Ein to log10(C)')
    ax.grid()
    fig.savefig("p11.png")

    fig1, ax1 = plt.subplots()
    ax1.plot(np.array(logC), np.array(numSupportVectors))
    ax1.set(xlabel='log10(C)', ylabel='Number of Support Vector', title='Number of Support Vector to log10(C)')
    ax1.grid()
    fig1.savefig("p12.png")

main()
