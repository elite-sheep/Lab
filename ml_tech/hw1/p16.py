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

def randomSample(X, Y):
    row, col = X.shape
    arr = np.arange(row)
    np.random.shuffle(arr)
    XTrain = X[arr[1000:]]
    YTrain = Y[arr[1000:]]
    XValid = X[arr[:1000]]
    YValid = Y[arr[:1000]]

    return XTrain, YTrain, XValid, YValid


def main():
    X, Y = loadData('./data/features.train')
    logGamma = [-1, 0, 1, 2, 3]
    positiveMask = Y == 0.0
    negativeMask = Y != 0.0
    Y[positiveMask] = 1.0
    Y[negativeMask] = -1.0

    C = 0.1
    numIteration = 100
    
    selection = np.empty(numIteration)

    for iter_ in range(numIteration):
        XTrain, YTrain, XValid, YValid = randomSample(X, Y)
        curScore = 1.1
        curSelect = -1
        for j in range(5):
            solver = SVMSolver(C=C, kernel='rbf', degree=2, gamma=10**logGamma[j], coef0=0.0, shrinking=True)
            solver.train(XTrain, YTrain)
            score = 1.0 - solver.valid(XValid, YValid)
            if score < curScore:
                curScore = score
                curSelect = j
        selection[iter_] = logGamma[curSelect]
        print('Iteration #{iter}: score={score}, selection={selection}'.format(iter=iter_, score=score, selection=curSelect))

    fig, ax = plt.subplots()
    ax.hist(x=selection, bins=[-1.5+1*i for i in range(6)])
    ax.set(xlabel='log10(Gamma)', ylabel='Selection Times', title='Selection Times to log10(Gamma)')
    fig.savefig("p16.png")

main()
