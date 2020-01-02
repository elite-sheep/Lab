/*************************************************************************
    > File Name: lrsolver.cpp
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: Thu 02 Jan 2020 05:12:00 PM CST
 ************************************************************************/

#include "lrsolver.h"

#include <cstring>
#include <cmath>
#include <fstream>
#include <iostream>

LRSolver* LRSolver::fromTraingSet(const std::string& inputFile,
    int numFeature,
    int numTrainingSample,
    float learningRate) {

  std::cout << "LRSolver: Build from training set: " << inputFile << std::endl;

  std::ifstream ifs;
  ifs.open(inputFile, std::ifstream::in);
  if (!ifs.good()) {
    std::cerr << "LRSolver: File I/O error." << std::endl;
    ifs.close();
    return nullptr;
  }

  float* y = new float[numTrainingSample];
  float* x = new float[numTrainingSample * (numFeature + 1)];
  for (int i = 0; i < numTrainingSample; ++i) {
    x[i*(numFeature+1)] = 1.0;
    for (int j = 0; j < numFeature; ++j) {
      ifs >> x[i*(numFeature+1) + j+1];
    }
    ifs >> y[i];
    x[i*(numFeature+1)] = y[i];
  }

  return new LRSolver(x, y, numFeature, numTrainingSample, learningRate);
}

LRSolver::LRSolver(float* x,
    float* y,
    int numFeature,
    int numTrainingSample,
    float learningRate): _x(x),
  _y(y),
  _numFeature(numFeature),
  _numTrainingSamples(numTrainingSample),
  _learningRate(learningRate),
  _threshHold(0.5) {
    weights = new float[numFeature + 1];
  }

LRSolver::~LRSolver() {
  delete [] _y;
  delete [] _x;
}

void LRSolver::train(int numInterations) {

  weights[0] = 1;
  for (int i = 1; i < _numFeature+1; ++i) {
    weights[i] = 0.33;
  }

  float* tmpX = new float[_numFeature+1];
  float* tmpG = new float[_numFeature+1];
  float tmpWDotX;
  for (int iter = 0; iter < numInterations; ++iter) {
    memset(tmpG, 0, (_numFeature+1)*sizeof(float));
    for (int i = 0; i < _numTrainingSamples; ++i) {
      memcpy(tmpX, &_x[i*(_numFeature+1)], (_numFeature+1)*sizeof(float));
      tmpWDotX = 0.0f;
      for (int j = 0; j < _numFeature; ++j) {
        tmpWDotX += _y[i] * weights[j] * tmpX[j];
        tmpX[j] *= _y[i];
      }
      for (int j = 0; j < _numFeature; ++j) {
        tmpG[j] += tmpX[j] / (1+exp(tmpWDotX));
      }
    }

    float loss = 0.0f;
    for (int j = 0; j < _numFeature; ++j) {
      weights[j] += _learningRate * tmpG[j] / _numTrainingSamples;
      loss += tmpG[j];
    }

    std::cout << "Iter #" << iter << ": " << loss << std::endl;
  }

  return;
}
