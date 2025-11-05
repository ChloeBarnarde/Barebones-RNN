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
#include <math.h>


// one to many
struct rnn {
    int inputSize;
    int outputSize;
    int hiddenSize;
    int seqLen;
    double learningRate;
    // input size is 1 x input_size
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

double mult(double a, double b) { return a * b; }
double minusOne(double a) {return 1 - a; };

// probably need a struct to store all data in for returning it
int LossFunc(rnn* rnn, matrix* input, matrix* target, matrix* hprev) {
    // using MSE
    // L(\hat y, y) = \sum^{Y_y}_{t=1}L(\hat y_t, y_t)

    // target and input is currently one hot encoded

    matrix* hs = Matrix_Create(rnn->seqLen + 1, rnn->hiddenSize);
    for (int coli = 0; coli < hs->size[1]; coli++)
    {
        Matrix_Set(hs, 0, coli, Matrix_Get(hprev, 0, coli));
    }
    
    matrix* ys = Matrix_Create(rnn->seqLen, rnn->outputSize);
    matrix* ps = Matrix_Create(rnn->seqLen, rnn->outputSize);
    float loss;

    for (int t = 0; t < rnn->seqLen; t++)
    {
        // matricies to free:
        //  - Wxh - done
        //  - Whh- done
        //  - inputRowT - done
        //  - hsRowTminusOne - done
        //  - Why - done
        //  - hsRowT - done

        matrix* Wxh= Matrix_Create(rnn->Wxh->size[0], rnn->Wxh->size[1]);
        Matrix_Copy(rnn->Wxh, Wxh);
        matrix* Whh = Matrix_Create(rnn->Whh->size[0], rnn->Whh->size[1]);
        Matrix_Copy(rnn->Whh, Whh);


        // evaluating 

        // function: tanh(Wxh * xs[t] + Whh * hs[t-1] + bh)
        matrix* inputRowT = Matrix_VectorRow(input, t);
        Matrix_DotProd(Wxh, inputRowT);
        Matrix_Free(inputRowT);

        matrix* hsRowTMinusOne = Matrix_VectorRow(hs, t); // t = t-1 becauwse length = seqLen+1
        Matrix_DotProd(Whh, hsRowTMinusOne);
        Matrix_Free(hsRowTMinusOne);
        Matrix_Add(Wxh, Whh);
        Matrix_Free(Whh);
        Matrix_Add(Wxh, rnn->bh);
        for (int coli = 0; coli < hs->size[1]; coli++)
        {
            Matrix_Set(hs, t+1, coli, tanh(Matrix_Get(Wxh, 0, coli)));
        }
        Matrix_Free(Wxh);

        //function: Why * hs[t] + by
        matrix* Why = Matrix_Create(rnn->Why->size[0], rnn->Why->size[1]);
        matrix* hsRowT = Matrix_VectorRow(hs, t+1);
        Matrix_DotProd(Why, hsRowT);
        Matrix_Free(hsRowT);
        Matrix_Add(Why, rnn->by); // Why = ys
        Matrix_Copy(Why, ys);
        Matrix_Free(Why);
        
        // for probobilities
        double sum = 0;
        for (int coli = 0; coli < ys->size[1]; coli++)
        {
            Matrix_Set(ys, t, coli, exp(Matrix_Get(ys, t, coli)));
            sum += Matrix_Get(ys, t, coli);
        }

        for (int coli = 0; coli < ps->size[1]; coli++)
        {
            Matrix_Set(ps, t, coli, Matrix_Get(ys, t, coli) / sum);
        }

        // softmax loss
        for (int coli = 0; coli < ps->size[1]; coli++)
        {
            loss -= Matrix_Get(target, t, coli) * log(Matrix_get(ps, t, coli)); 
        }
    }
    

    // === gradient ===
    // gradient of matricies
    matrix* dWxh = Matrix_create(rnn->Wxh->size[0], rnn->Wxh->size[1]);
    matrix* dWhy = Matrix_create(rnn->Why->size[0], rnn->Why->size[1]);
    matrix* dWhh = Matrix_create(rnn->Whh->size[0], rnn->Whh->size[1]);
    // gradient of vectors
    matrix* dbh = Matrix_create(rnn->bh->size[0], rnn->bh->size[1]);
    matrix* dby = Matrix_create(rnn->by->size[0], rnn->by->size[1]);
    //gradietn of hidden vector
    matrix* dhnext = Matrix_Create(1, rnn->hiddenSize);

    for (int t = rnn->seqLen - 1; t >= 0; t--)
    {
        // Matricies to free:
        //  - dy - done
        //  - dyCopy - done
        //  - hsRowT - done
        //  - dh - done
        //  - dhraw - done
        //  - xsRowT - done
        //  - hsRowTMinusOne - done
        //  - Whh - done
        
        // dE/dy[j] = y[j] - t[j]
        matrix* dy = Matrix_VectorRow(ps, t);
        // want to find true label
        for (int coli = 0; coli < dy->size[1]; coli++)
        {
            if (Matrix_Get(target, t, coli) != 1)
                continue;
            Matrix_Set(dy, t, coli, Matrix_Get(dy, t, coli) - 1);
            break;
        }
        
        // dE/dy[j]*dy[j]/dWhy[j,k] = dE/dy[j] * h[k]
        matrix* dyCopy = Matrix_Create(1, 1);
        Matrix_Copy(dy, dyCopy);
        matrix* hsRowT = Matrix_VectorRow(hs, t+1);
        Matrix_Transpose(hsRowT);
        Matrix_DotProd(dyCopy, hsRowT);
        Matrix_Add(dWhy, dyCopy);
        Matrix_Free(dyCopy);
        Matrix_Add(dby, dy);

        //backprop into h
        matrix* dh = Matrix_Create(1, 1);
        Matrix_Copy(rnn->Why, dh);
        Matrix_Transpose(dh);
        Matrix_DotProd(dh, dy);
        Matrix_Free(dy);
        Matrix_Add(dh, dhnext);
        //backprop thourhg tanh
        matrix* hsRowT = Matrix_VectorRow(hs, t + 1);
        Matrix_ElementWiseFunc2M(hsRowT, hsRowT, mult);
        Matrix_ElementWiseFunc1M(hsRowT, minusOne);
        Matrix_ElementWiseFunc2M(hsRowT, dh, mult);
        Matrix_Free(dh);

        Matrix_Add(dbh, hsRowT);

        // input layer and hidden layer
        matrix* dhraw= Matrix_Create(1, 1);
        Matrix_Copy(hsRowT, dhraw);
        matrix * xsRowT = Matrix_VectorRow(input, t);
        Matrix_Transpose(xsRowT);
        Matrix_DotProd(dhraw, xsRowT);
        Matrix_Free(xsRowT);
        Matrix_Add(dWxh, dhraw);

        Matrix_Copy(hsRowT, dhraw);

        matrix* hsRowTMinusOne = Matrix_VectorRow(hs, t);
        Matrix_Transpose(hsRowTMinusOne);
        Matrix_DotProd(dhraw, hsRowTMinusOne);
        Matrix_Free(hsRowTMinusOne);
        Matrix_Add(dWhh, dhraw);

        //dhnext
        matrix* Whh = Matrix_Create(1 , 1);
        Matrix_Transpose(Whh);
        Matrix_Copy(rnn->Whh, Whh);
        Matrix_Copy(hsRowT, dhraw);
        Matrix_Free(hsRowT);
        Matrix_DotProd(Whh, dhraw);
        Matrix_Free(dhraw);
        Matrix_Copy(Whh, dhnext);
        Matrix_Free(Whh);
    }

    // gradient clipping
    // loop over all values of all gradients and clip between -5 and 5

    // Freeing
    Matrix_Free(hs);
    Matrix_Free(ps);

    // return loss, dWxh, dWhh, dWhy, dbh, dby, hs[len(inputs)], ???
}

double MSE_Loss(matrix* output, matrix* target) {
    if (output->size[0] != target->size[0])
        return -1.;
    if (output->size[1] != target->size[1])
        return -1.;
    
    double sum = 0;
    for (int rowi = 0; rowi < output->size[0]; rowi++)
    {
        for (int coli = 0; coli < output->size[1]; coli++)
        {
            sum += pow(output->values[rowi*output->size[1]+coli] - target->values[rowi*output->size[1]+coli], 2);
        }
    }

    sum /= output->size[0] * output->size[1];
    return sum;
}

int TrainRNN(rnn* r, float* trainX[], float* trainY[]) {

}

int TestRNN(rnn* r, float* testX[], float* testY[]) {

}

// want to be adjust features/data to train model better :p

#endif