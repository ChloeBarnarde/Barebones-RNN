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
    int activationFunctions[2];
} typedef rnn;

int step(rnn* r) {

}
// one to one
matrix* evaluate(rnn* r, float* x[]) {
    //find h
    // need to step backwards through time (onlt need if you have multiple time steps)
    matrix* h0 = (matrix*)malloc(sizeof(matrix));
    h0->size[0] = r->hiddenSize;
    h0->size[1] = 1;
    // initialize with zeros
    for (int i = 0; i < r->hiddenSize; i++)
    {
        h0->values[i] = 0;
    }
    matrix* h = (matrix*)malloc(sizeof(matrix));
    h->size[0] = r->hiddenSize;
    h->size[1] = 1;

    // before applying activation function
    matrix* a = (matrix*)malloc(sizeof(matrix));
    MAdd(r->Whh, h0, a);
    // apply activation function
    // loop with for loop
    // find y

    matrix* y = (matrix*)malloc(sizeof(matrix));
    y->size[0] = r->outputSize;
    y->size[1] = 1;

    // before activation
    // apply activation


    // return y
    
    return y;
}

int TrainRNN(rnn* r, float* trainX[], float* trainY[]) {

}

int TestRNN(rnn* r, float* testX[], float* testY[]) {

}

// want to be adjust features/data to train model better :p

#endif