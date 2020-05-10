# Copyright 2020 Yuchen Wong

import numpy as np
import tensorflow as tf

class MeanSquearError(tf.keras.losses.Loss):
    def call(self, y_true, y_pred):
        error = tf.keras.backend.pow(y_true-y_pred, 2.0)
        return tf.math.reduce_mean(error)

class BasicAutoEncoder():
    def __init__(self, featureNum, denseLayerSize, learningRate=0.1):
        self._featureNum = featureNum
        self._denseLayerSize = denseLayerSize
        self._learningRate = learningRate
        self._model = self.buildUpModel()
        self._isTrained = False

    def buildUpModel(self):
        model = tf.keras.Sequential()
        denseLayerInitializer = self.getConstantInitializer(self._featureNum, self._denseLayerSize)
        model.add(tf.keras.layers.Dense(
            self._denseLayerSize, 
            input_shape=(self._featureNum,), 
            kernel_initializer=denseLayerInitializer,
            bias_initializer='ones'))
        outputLayerInitializer = self.getConstantInitializer(self._denseLayerSize, self._featureNum)
        model.add(tf.keras.layers.Dense(
            self._featureNum, 
            kernel_initializer=outputLayerInitializer,
            bias_initializer='ones'))
        optimizer = tf.keras.optimizers.SGD(learning_rate=self._learningRate)
        model.compile(optimizer=optimizer,
                loss=MeanSquearError())

        return model

    def train(self, X, batches=1000):
        batchSize = X.shape[0]
        self._model.fit(x=X, y=X, batch_size = batchSize,
                epochs = batches)
        err = self._model.evaluate(x=X, y=X)
        self.isTrained = True
        print("Ein: %f" %(err))

    def evaluate(self, X):
        if self.isTrained == False:
            print("BasicAutoEncoder: Not trained.")
            return
        eout = self._model.evaluate(x=X, y=X)
        print("Eout: %f" %(eout))
        return eout

    def getConstantInitializer(self, previousLayerSize, layerSize):
        res = np.random.rand(layerSize, previousLayerSize)
        u = np.sqrt(6.0 / (1.0 + previousLayerSize + layerSize))
        for i in range(layerSize):
            for j in range(previousLayerSize):
                res[i,j] = -u + res[i,j] * 2.0 * u
        return tf.constant_initializer(res)
