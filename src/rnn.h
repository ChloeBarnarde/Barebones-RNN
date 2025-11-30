#ifndef RNN_H
#define RNN_H

#include "matrixMath.h"


// one to many?
struct rnn {
    int inputSize;
    int outputSize;
    int hiddenSize;
    int seqLen;
    double learningRate;
    // weights input to hidden size = input_size x hidden_size
    matrix* Wxh;
    // weights hidden to hidden (matrix) size = hidden_size x hidden_size
    matrix* Whh;
    // weights hidden to output (vector i think?) size = hidden_size x output_size
    matrix* Why;
    // bias hidden (matrix)
    matrix* bh;  
    // bias output (vector)
    matrix* by;
    int timeStep;
    double (*activationFuncstions[2])(double);
} typedef rnn;

// epoch
struct training_data {
    matrix* input; // X
    matrix* output; // Y
    matrix* testInput; // test X
    matrix* testOutput; // test Y
    int iterations;
} typedef training_data;

struct gradient_info {
    double loss;
    // = matrices =
    matrix* dWxh;
    matrix* dWhh;
    matrix* dWhy;
    // = Vectors =
    matrix* dbh;
    matrix* dby;
    matrix* h;
} typedef gradient_info;

/// @brief returns a matrix with the fitted y, rnn seqlen determines size
/// @param r the model
/// @param inputs x values, ensure there's one for each time step
/// @return fitted y
matrix* evaluate(rnn* r, matrix* inputs);


/// @brief Calculates the loss for a function. Only works for classification atm
/// @param rnn rnn
/// @param input input values
/// @param target target values
/// @param hprev previous h
/// @return gradient info struct
gradient_info* LossFunc(rnn* rnn, matrix* input, matrix* target, matrix* hprev);

/// @brief calculates the mean square error loss
/// @param output what the model output
/// @param target what the true values are
/// @return loss value
double MSE_Loss(matrix* output, matrix* target);

/// @brief trains the rnn for the specified amount 'limit'
/// @param r rnn to train
/// @param epoch epoch data
/// @param limit how many times to go through epoch
/// @return exit status
int TrainRNN(rnn* r, training_data* epoch, int limit, void (*onComplete)(double, double));

///@brief tests the rnn on the given epoch and returns the loss
double TestRNN(rnn* r, matrix* input, matrix* target, matrix* hprev);

/// @brief Initailize all the weights in the struct to sample from a normal distribution
/// @param r the rnn to initalize the weights in
/// @return exit status
int InitializeWeights(rnn* r);

/// @brief Free's all the weight matrices
/// @param r the rnn to free the matrices from 
/// @return the exit status
int FreeWeights(rnn* r);

#endif
