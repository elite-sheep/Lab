import cv2
import numpy as np
import math
import FFT
import Complex

## calculate distance between 2 points
def dis(x1,y1,x2,y2):
    return math.sqrt(1.0*(x1-x2)*(x1-x2)+1.0*(y1-y2)*(y1-y2))

## Open a new image with given file name
def InputFromFile(filename):
    res = cv2.imread(filename)
    res = cv2.cvtColor(res,cv2.COLOR_RGBA2GRAY)
    return res

## Write the image to a file with given file name
def Output(filename,Image):
    cv2.imwrite(filename, Image)

## Tranpose the whole matrix
def Transpose(Matrix):
    res = [[] for i in range(len(Matrix[0]))]
    for i in range(len(Matrix)):
        for j in range(len(Matrix[i])-1,-1,-1):
            res[j].append(Matrix[i][j])
    return res

## Convert a real matrix to a complex matrix
def Real2Complex(Matrix):
    res = [[] for i in range(len(Matrix))]
    for i in range(len(Matrix)):
        for j in range(len(Matrix[i])-1,-1,-1):
            res[i].append(Complex.Complex(Matrix[i][j],0.0))
    return res

## Convert a complex matrix to a real matrix
def Complex2Real(Matrix):
    res = [[] for i in range(len(Matrix))]
    for i in range(len(Matrix)):
        for j in range(len(Matrix[i])-1,-1,-1):
            res[i].append(int(Matrix[i][j].real + 0.5))
    return res

## Standarise a image
def standard(Image):
    height = 1
    while(height < len(Image)):
        height = height*2
    height = height*2
    for i in range(len(Image),height,1):
        Image.append([])
    print len(Image)

    weight = 1
    while(weight < len(Image[0])):
        weight = weight*2
    weight = weight*2
    for i in range(height):
        for j in range(len(Image[i]),weight,1):
            Image[i].append(Complex.Complex(0.0,0.0))
    return Image

## Process FFT to an origin image
def Image_FFT(Image):
    height = 0
    width = 0
    while((1<<height)<len(Image)):
        height = height + 1
    while((1<<width)<len(Image[0])):
        width = width + 1

    for i in range(len(Image)):
        Image[i] = FFT.FFT(Image[i],len(Image[i]),width-1)
    Image = Transpose(Image)

    for i in range(len(Image)):
        Image[i] = FFT.FFT(Image[i],len(Image[i]),height-1)
    Image = Transpose(Image)
    return Image

def ImCopy(Matrix):
    res = [[] for i in range(len(Matrix))]
    for i in range(len(Matrix)):
        for j in range(0,len(Matrix[i]),1):
            res[i].append(Matrix[i][j])
    return res


## Processing inverse-fft to the origin image
def Image_FFTInverse(Image):
    height = 0
    width = 0
    while((1<<height)<len(Image)):
        height = height + 1
    while((1<<width)<len(Image[0])):
        width = width + 1

    for i in range(len(Image)):
        Image[i] = FFT.FFT_Inverse(Image[i],len(Image[i]),width-1)
    Image = Transpose(Image)

    for i in range(len(Image)):
        Image[i] = FFT.FFT_Inverse(Image[i],len(Image[i]),height-1)
    Image = Transpose(Image)
    return Image

## Do the low bandwidth filter
def lowbandwidth(Image):
    height = len(Image)
    width = len(Image[0])
    height /= 2
    width /= 2
    processImage = ImCopy(Image)

    Image_origin = Complex2Real(processImage)

    for i in range(height*2):
        for j in range(width*2):
            if(dis(i,j,height,width) < 500.0):
                processImage[i][j].real = dis(i,j,height,width)/500.0*processImage[i][j].real

    Image_trans = Image_FFTInverse(processImage)
    Image_res = Complex2Real(Image_trans)
    for i in range(height*2):
        for j in range(width*2):
            if((i+j)&1 > 0):
                Image_res[i][j] = -1*Image_res[i][j]
    Output("lowband.jpg",np.array(Image_res))

def highbandwidth(Image):
    height = len(Image)
    width = len(Image[0])
    height /= 2
    width /= 2
    processImage = ImCopy(Image)

    Image_origin = Complex2Real(processImage)

    for i in range(height*2):
        for j in range(width*2):
            if(dis(i,j,height,width) < 500.0):
                processImage[i][j].real = (500.0-dis(i,j,height,width))/500.0*processImage[i][j].real
            else:
                processImage[i][j].real = 0.0

    Image_trans = Image_FFTInverse(processImage)
    Image_res = Complex2Real(Image_trans)
    for i in range(height*2):
        for j in range(width*2):
            if((i+j)&1 > 0):
                Image_res[i][j] = -1*Image_res[i][j]
    Output("highband.jpg",np.array(Image_res))


def main(filename):
    Image = InputFromFile(filename)
    Image = Real2Complex(Image)
    Image_tans = standard(Image)
    Image_res = Image_FFT(Image_tans)
    Image_out = Complex2Real(Image_res)
    Output("filtered.jpg",np.array(Image_out))
    lowbandwidth(Image_res)
    highbandwidth(Image_res)
    Image_out = Complex2Real(Image_res)

main("./image.jpg")
