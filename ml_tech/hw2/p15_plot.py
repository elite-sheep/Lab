# Copyright 2020 Yuchen Wong

import numpy as np
import matplotlib.pyplot as plt

logD = [1, 2, 3, 4, 5, 6, 7]
ein = [0.398328,0.360617,0.285644,0.223900,0.175442,0.132026,0.098142]
eout = [0.410327,0.372012,0.298467,0.235299,0.184866,0.139808,0.104717]

def main():
    fig1,ax1 = plt.subplots()
    ax1.plot(np.array(logD), np.array(ein))
    ax1.set(xlabel='log(D)', ylabel='ein', title='ein to log2(D)')
    for logd, Ein in zip(logD, ein):
        ax1.text(logd, Ein, '({}, {})'.format(logd, Ein))
    fig1.savefig('p15.png')

    fig2,ax2 = plt.subplots()
    ax2.plot(np.array(logD), np.array(ein))
    ax2.set(xlabel='log(D)', ylabel='eout', title='eout to log2(D)')
    for logd, Eout in zip(logD, eout):
        ax2.text(logd, Eout, '({}, {})'.format(logd, Eout))
    fig2.savefig('p16.png')

main()
