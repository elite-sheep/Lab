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
    positiveMask = Y == 0.0
    negativeMask = Y != 0.0
    Y[positiveMask] = 1.0
    Y[negativeMask] = -1.0
    
    w_len = []
    for c in logC:
        solver = SVMSolver(C=10**c, kernel='linear')
        solver.train(X, Y)
        w = solver.getW()
        w_len.append(np.linalg.norm(w))
        print(w)

    fig, ax = plt.subplots()
    ax.plot(np.array(logC), np.array(w_len))
    ax.set(xlabel='log10(C)', ylabel='||w||', title='||w|| to log10(C)')
    ax.grid()
    fig.savefig("p10.png")

main()
