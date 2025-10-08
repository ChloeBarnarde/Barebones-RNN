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
    // weights input to hidden
    matrix* Wxh;
    // weights hidden to hidden (matrix)
    matrix* Whh;
    // weights hidden to output (vector i think?)
    matrix* Why;
    // bias hidden (matrix)
    matrix* bh;  
    // bias output (vector)
    matrix* by;
    int timeStep;
    double (*activationFuncstions[2])(double);
} typedef rnn;

int step(rnn* r) {

}

double one() {
    return 1;
}
// one to one
matrix* evaluate(rnn* r, double* x) {
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


    // assuming we have all the values set up
    matrix* input = Matrix_Create(1, r->inputSize);
    input->values = x;
    matrix* h0 = Matrix_Initialize(r->hiddenSize, r->outputSize, &one); // hidden state at timestep 0 with all 1's, should probably be random lol
    // hidden layers
    // assuming dot product works
    matrix* result1= Matrix_Create(2, 2);// set sizes correctly
    Matrix_Copy(r->Whh, result1);
    Matrix_DotProd(result1, h0);
    matrix* result2 = Matrix_Create(2, 2);
    Matrix_Copy(r->Wxh, result2);
    Matrix_DotProd(result2, input);
    Matrix_Add(result1, result2);
    Matrix_Add(result1, r->bh);
    // need to apply activation function
    Matrix_ElemetWiseFunc1M(result1, r->activationFuncstions[0]);

    // output layer
    Matrix_Copy(r->Why, result2);
    Matrix_DotProd(result2, result1);
    Matrix_Add(result2, r->by);
    Matrix_ElemetWiseFunc1M(result2, r->activationFuncstions[1]);

    return result2; // output

    // hidden layer time state
    //matrix* ht = 

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