# Copyright 2020 Yuchen Wong

import numpy as np
import tensorflow as tf

class DenseTranspose(tf.keras.layers.Layer):
    def __init__(self, dense, activation=None, **kwargs):
        self.dense = dense
        self.activation = tf.keras.activations.get(activation)
        super().__init__(**kwargs)

    def build(self, batch_input_shape):
        self.bias = self.add_weight(name="bias", shape = [self.dense.input_shape[-1]],
                initializer=tf.keras.initializers.Ones())
        super().build(batch_input_shape)

    def call(self, inputs):
        z = tf.matmul(inputs, self.dense.weights[0], transpose_b=True)
        return self.activation(z + self.bias)

class MeanSquearError(tf.keras.losses.Loss):
    def call(self, y_true, y_pred):
        error = tf.keras.backend.pow(y_true-y_pred, 2.0)
        return tf.math.reduce_mean(error)

class BasicAutoEncoder():
    def __init__(self, featureNum, denseLayerSize, learningRate=0.1, tieWeights = False, pca=False):
        self._featureNum = featureNum
        self._denseLayerSize = denseLayerSize
        self._learningRate = learningRate
        self._tieWeights = tieWeights
        self._pca = pca
        self._model = self.buildUpModel()
        self._isTrained = False

    def buildUpModel(self):
        model = tf.keras.Sequential()
        denseLayerInitializer = self.getConstantInitializer(self._featureNum, self._denseLayerSize)
        encoder = tf.keras.layers.Dense(
                self._denseLayerSize,
                input_shape=(self._featureNum,),
                activation='tanh',
                kernel_initializer=denseLayerInitializer,
                bias_initializer='ones')
        model.add(encoder)
        if self._tieWeights == True:
            model.add(DenseTranspose(encoder, activation=None))
        else:
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
        if self._pca == True:
            self._pivot = np.mean(X, axis=0)
            print(self._pivot.shape)
            print(X[0].shape)
            for i in range(batchSize):
                X[i] = X[i] - self._pivot
        self._model.fit(x=X, y=X, batch_size = batchSize,
                epochs = batches)
        err = self._model.evaluate(x=X, y=X)
        self.isTrained = True
        print("Ein: %f" %(err))

    def evaluate(self, X):
        if self.isTrained == False:
            print("BasicAutoEncoder: Not trained.")
            return
        if self._pca == True:
            for i in range(X.shape[0]):
                X[i] = X[i] - self._pivot
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
