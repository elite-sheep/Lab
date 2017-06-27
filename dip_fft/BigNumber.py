from __future__ import print_function 
import numpy as np
import math
import Complex
import FFT

## Transfor the string to a complex number
def trans(arr):
    res = []
    for i in range(len(arr)-1,-1,-1):
        res.append(Complex.Complex(int(arr[i]),0.0))
    return res

## Output the given number
def Output(number):
    i = len(number)-1
    while(number[i] == 0 and i > 0):
        i = i-1

    while(i>=0):
        print(int(number[i]),end='')
        i = i-1

## Do the number multiply
def Mul(numbera,numberb,size,length):
    transfora = FFT.FFT(numbera,size,length)
    transforb = FFT.FFT(numberb,size,length)

    for i in range(size):
        transfora[i] = transfora[i]*transforb[i]
        #print(str(transfora[i].real))
    res = [0.0 for i in range(size*2)]
    resa = FFT.FFT_Inverse(transfora,size,length)


    for i in range(size):
        #print(resa[i].real)
        res[i] = int(resa[i].real + 0.5)

    for i in range(2*size):
        if(res[i] >= 10):
            res[i+1] = res[i+1] + res[i]/10
            res[i] = res[i]%10
    return res

def main():
    anum = raw_input("Please input the first number\n")
    bnum = raw_input("please input the second number\n")
    a = list(anum)
    b = list(bnum)
    size = max(len(a),len(b))
    i = 0
    s = 1
    while(s < size):
        i = i+1
        s = s*2
    s = s*2
    length = i
    anumber = []
    bnumber = []
    for i in range(len(a),s,1):
        anumber.append(Complex.Complex(0.0,0.0))
    for i in range(len(b),s,1):
        bnumber.append(Complex.Complex(0.0,0.0))
    #anumber = anumber + trans(a)
    #bnumber = bnumber + trans(b)
    anumber = trans(a) + anumber
    bnumber = trans(b) + bnumber
    res = Mul(anumber,bnumber,s,length)
    Output(res)

main()
