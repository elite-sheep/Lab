# Copyright 2020 Yuchen Wong

import numpy as np
import pandas as pd
from basicautoencoder import BasicAutoEncoder

def loadData(filename):
    data = pd.read_csv(filename, sep='\s+', header=None)
    row, col = data.shape
    print(data.shape)
    return np.array(data[data.columns[-256:]])

def main():
    autoEncoder = BasicAutoEncoder(featureNum=256,
            denseLayerSize=2,
            learningRate=0.1,
            tieWeights=True)

    X = loadData('./data/zip.train')
    XTest = loadData('./data/zip.test')
    autoEncoder.train(X, batches=5000)
    autoEncoder.evaluate(XTest)

main()
