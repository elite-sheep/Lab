## This file includes some basic definitions of complex numbers

import math

class Complex:
    ## Construct a new complex number

    def __init__(self):
        self.real = 0.0
        self.image = 0.0

    def __init__(self,real,image):
        self.real = 1.0*real
        self.image = 1.0*image

    ## Some basic operations of complex number

    def __add__(self,other):
        return Complex(self.real+other.real,self.image+other.image)

    def __sub__(self,other):
        return Complex(self.real-other.real,self.image-other.image)

    def __mul__(self,other):
        newreal = self.real*other.real - self.image*other.image
        newimage = self.real*other.image + self.image*other.real
        return Complex(newreal,newimage)

    def __eq__(self,other):
        if self.image == other.image and self.real == other.real:
            return True
        else:
            return False
