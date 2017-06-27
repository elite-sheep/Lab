import cv2
import numpy as np
import math
import Complex

## calculate the reserve version of a number
def rev(cur,length):
    res = 0
    for i in range(length):
        res = res << 1
        if(cur & (1<<i)):
            res = res | 1

    return res

## FFT algorithm for an array
def FFT(arr,size,length):
    for i in range(size):
        if(i < rev(i,length+1)):
            t = arr[i]
            arr[i] = arr[rev(i,length+1)]
            arr[rev(i,length+1)] = t

    m = 1
    for i in range(1,length+2,1):
        m = m*2
        wm = Complex.Complex(math.cos(2.0*math.pi/(1.0*m)),math.sin(2.0*math.pi/(1.0*m)))
        for j in range(0,size,m):
            w = Complex.Complex(1.0,0.0)
            for k in range(m/2):
                u = arr[j+k]
                t = w*arr[j+k+m/2]
                arr[j+k] = u+t
                arr[j+k+m/2] = u-t
                w = w*wm
                #print(str(j)+" "+str(k))
                #print(str(u.real)+" "+str(u.image))
    #for i in range(size):
    #    print (str(arr[i].real)+" "+str(arr[i].image))


    return arr

## Inverse FFT algorithm
def FFT_Inverse(arr,size,length):
    for i in range(size):
        if(i < rev(i,length+1)):
            t = arr[i]
            arr[i] = arr[rev(i,length+1)]
            arr[rev(i,length+1)] = t

    for i in range(1,length+2,1):
        m = (1<<i)
        wm = Complex.Complex(math.cos(-2.0*math.pi/m),math.sin(-2.0*math.pi/m))
        for j in range(0,size,m):
            w = Complex.Complex(1.0,0.0)
            for k in range(m/2):
                u = arr[j+k]
                t = w*arr[j+k+m/2]
                arr[j+k] = u+t
                arr[j+k+m/2] = u-t
                w = w*wm

    for i in range(size):
        arr[i].real /= size
        arr[i].image /= size
        
    return arr
