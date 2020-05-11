# Copyright 2020 Yuchen Wong

import numpy as np
import matplotlib.pyplot as plt

logD = [1, 2, 3, 4, 5, 6, 7]
ein = [0.431720,0.396511,0.354146,0.299761,0.206601,0.150660,0.102371]
eout = [0.445720,0.409435,0.367183,0.311409,0.216559,0.159105,0.109655]

def main():
    fig1,ax1 = plt.subplots()
    ax1.plot(np.array(logD), np.array(ein))
    ax1.set(xlabel='log(D)', ylabel='ein', title='ein to log2(D)')
    for logd, Ein in zip(logD, ein):
        ax1.text(logd, Ein, '({}, {})'.format(logd, Ein))
    fig1.savefig('p11.png')

    fig2,ax2 = plt.subplots()
    ax2.plot(np.array(logD), np.array(ein))
    ax2.set(xlabel='log(D)', ylabel='eout', title='eout to log2(D)')
    for logd, Eout in zip(logD, eout):
        ax2.text(logd, Eout, '({}, {})'.format(logd, Eout))
    fig2.savefig('p12.png')

main()
