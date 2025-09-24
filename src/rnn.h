#ifndef RNN_H
#define RNN_H

#define ACT_SIGMOID 1
#define ACT_TANH 2
#define ACT_RELU 3

#define E 2.71828182845904523
// activation functions
#define RELU(x)     (Max(0, (x)))
#define Max(a, b)   ((((a) > (b)) ? (a) : (b))
#define TANH(x)     ((E^(x) - E^(-(x))) / (E^(x) + E^(-(x)))
#define SIGMOID(x)  (1 / (1 + (E^(-(x))))

#include "matrixMath.h"


struct rnn {
    int inputSize;
    int outputSize;
    int hiddenSize;
    // int seqLen;
    double learningRate;
    matrix* Wxh;
    matrix* Whh;
    matrix* Why;
    matrix* bh;
    matrix* by;
    int timeStep;
} typedef rnn;

int step(rnn* r) {

}
// one to one
int evaluate(rnn* r, float* x[]) {
    //find h
    // need to step backwards through time (onlt need if you have multiple time steps)
    
    // find y
    
}

int TrainRNN(rnn* r, float* trainX[], float* trainY[]) {

}

int TestRNN(rnn* r, float* testX[], float* testY[]) {

}

// want to be adjust features/data to train model better :p

#endif