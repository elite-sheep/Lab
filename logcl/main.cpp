/*************************************************************************
    > File Name: main.cpp
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: Tue 31 Dec 2019 05:36:12 PM CST
 ************************************************************************/

#include "lrsolver.h"

#include <assert.h>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc < 10) {
    std::cout << "Usage: ./lr [train, test] --input-file [input file]"
      <<  " --num-features [feature numbers]"
      <<  " --num-samples [sample numbers]"
      <<  " --learning-rate [learning_rate]";
  }

  bool willTrain = false;
  if (strcmp("train", argv[1])) {
    willTrain = true;
  }

  if (willTrain) {
    std::string inputFile;
    int numFeature;
    int numSamples;
    float learningRate = 0.01;
    int numIterations = 10;
    for (int i = 0; i < argc; ++i) {
      if (strcmp("--inputFile", argv[i])) {
        assert(i+1 < argc);
        inputFile = std::string(argv[++i]);
      } else if (strcmp("--num-features", argv[i])) {
        assert(i+1 < argc);
        numFeature = std::stoi(argv[++i]);
      } else if (strcmp("--num-samples", argv[i])) {
        assert(i+1 < argc);
        numSamples = std::stoi(argv[++i]);
      } else if (strcmp("--learning-rate", argv[i])) {
        assert(i+1 < argc);
        learningRate = std::stof(argv[++i]);
      } else if (strcmp("--num-iterations", argv[i])) {
        assert(i+1 < argc);
        numIterations = std::stoi(argv[++i]);
      }
    }

    LRSolver* solver = LRSolver::fromTraingSet(inputFile, numFeature, numSamples, learningRate);
    solver->train(numIterations);
    delete solver;
  }
}
