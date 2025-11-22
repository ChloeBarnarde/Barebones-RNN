#include <math.h>
#include "rnn.h"
// a lot of this code takes insporation from Andrej Karpathy's blog https://karpathy.github.io/2015/05/21/rnn-effectiveness/
// in a lot of cases it is a c implementation of the minimal character-level RNN language model in Python/numpy which he made.

double mult(double a, double b) { return a * b; }
double minusOne(double a) {return 1 - a; }

gradient_info* LossFunc(rnn* rnn, matrix* input, matrix* target, matrix* hprev) {

    // target and input is currently one hot encoded

    matrix* hs = Matrix_Create(rnn->hiddenSize, rnn->seqLen + 1);
    for (int rowi = 0; rowi < hs->size[0]; rowi++)
    {
        // printf("getting matrix hprev\n");
        int result = Matrix_Set(hs, rowi, 0, Matrix_Get(hprev, rowi, 0));
        if (result == EXIT_FAILURE) {
            printf("Issue with hprev get\n");
        }
    }

    printf("setting ys and ps\n");
    matrix* ys = Matrix_Create(rnn->outputSize, rnn->seqLen);
    matrix* ps = Matrix_Create(rnn->outputSize, rnn->seqLen);
    double loss;
    
    for (int t = 0; t < rnn->seqLen; t++)
    {
        // printf("sequence: %d\n", t);
        // matrices to free:
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
        // printf("asdfasdfasdf\n");
        matrix* inputRowT = Matrix_VectorCol(input, t);
        Matrix_DotProd(Wxh, inputRowT);
        Matrix_Free(inputRowT);
        
        // printf("asdfasdfasdf\n");
        matrix* hsRowTMinusOne = Matrix_VectorCol(hs, t); // t = t-1 becauwse length = seqLen+1
        Matrix_DotProd(Whh, hsRowTMinusOne);
        Matrix_Free(hsRowTMinusOne);
        Matrix_Add(Wxh, Whh);
        Matrix_Free(Whh);
        Matrix_Add(Wxh, rnn->bh);
        for (int rowi = 0; rowi < hs->size[0]; rowi++)
        {
            // printf("setting values\n");
            Matrix_Set(hs, rowi, t+1, tanh(Matrix_Get(Wxh, rowi, 0)));
        }
        Matrix_Free(Wxh);

        //function: Why * hs[t] + by
        matrix* Why = Matrix_Create(rnn->Why->size[0], rnn->Why->size[1]);
        matrix* hsRowT = Matrix_VectorCol(hs, t+1);
        Matrix_DotProd(Why, hsRowT);
        Matrix_Free(hsRowT);
        Matrix_Add(Why, rnn->by); // Why = ys
        // fix here. set col t of ys to why.
        //  i.e. ys[t] = Why
        for (int rowi = 0; rowi < Why->size[0]; rowi++)
        {
            Matrix_Set(ys, rowi, t, Matrix_Get(Why, rowi, 0));
        }
        // Matrix_Copy(Why, ys);
        Matrix_Free(Why);
        
        // for probobilities
        double sum = 0;
        for (int rowi = 0; rowi < ys->size[0]; rowi++)
        {
            // printf("current y: %lf\n", Matrix_Get(ys, rowi, t));
            Matrix_Set(ys, rowi, t, exp(Matrix_Get(ys, rowi, t)));
            // printf("updated y: %lf\n", Matrix_Get(ys, rowi, t));
            sum += Matrix_Get(ys, rowi, t);
        }
        // printf("%lf\n", sum);
        
        for (int rowi = 0; rowi < ps->size[0]; rowi++)
        {
            Matrix_Set(ps, rowi, t, Matrix_Get(ys, rowi, t) / sum);
            // if (Matrix_Get(ps, rowi, t) == 0) {
            //     printf("ps contains a zero\n");
            // }
        }
        // softmax loss
        for (int rowi = 0; rowi < ps->size[0]; rowi++)
        {
            if (Matrix_Get(ps, rowi, t) == 0) {
                // printf("zerooo\n");
            }
            loss -= Matrix_Get(target, rowi, t) * log(Matrix_Get(ps, rowi, t)); 
        }
        // printf("%lf\n", loss);
        int n;
        // scanf("%d", &n);
    }

    printf("finished eval\n");

    // === gradient ===
    // gradient of matrices
    matrix* dWxh = Matrix_Create(rnn->Wxh->size[0], rnn->Wxh->size[1]);
    matrix* dWhy = Matrix_Create(rnn->Why->size[0], rnn->Why->size[1]);
    matrix* dWhh = Matrix_Create(rnn->Whh->size[0], rnn->Whh->size[1]);
    // gradient of vectors
    matrix* dbh = Matrix_Create(rnn->bh->size[0], rnn->bh->size[1]);
    matrix* dby = Matrix_Create(rnn->by->size[0], rnn->by->size[1]);
    //gradietn of hidden vector
    matrix* dhnext = Matrix_Create(rnn->hiddenSize, 1);

    printf("start gd\n");
    for (int t = rnn->seqLen - 1; t >= 0; t--)
    {
        // printf("gradient: %d\n", t);
        // matrices to free:
        //  - dy - done
        //  - dyCopy - done
        //  - hsRowT - done
        //  - dh - done
        //  - dhraw - done
        //  - xsRowT - done
        //  - hsRowTMinusOne - done
        //  - Whh - done
        
        // dE/dy[j] = y[j] - t[j]
        matrix* dy = Matrix_VectorCol(ps, t);
        // want to find true label
        for (int rowi = 0; rowi < dy->size[0]; rowi++)
        {
            if (Matrix_Get(target, rowi, t) != 1)  {
                // only want to update the actual labels
                continue;
            }
            // printf("setting values\n");
            Matrix_Set(dy, rowi, 0, Matrix_Get(dy, rowi, 0) - 1);
            break;
        }
        
        // dE/dy[j]*dy[j]/dWhy[j,k] = dE/dy[j] * h[k]
        matrix* dyCopy = Matrix_Create(1, 1);
        Matrix_Copy(dy, dyCopy);
        matrix* hsRowT = Matrix_VectorCol(hs, t+1);
        Matrix_Transpose(hsRowT);
        Matrix_DotProd(dyCopy, hsRowT);
        Matrix_Add(dWhy, dyCopy);
        Matrix_Free(dyCopy);
        Matrix_Add(dby, dy);
        Matrix_Free(hsRowT);

        //backprop into h
        matrix* dh = Matrix_Create(1, 1);
        Matrix_Copy(rnn->Why, dh);
        Matrix_Transpose(dh);
        Matrix_DotProd(dh, dy);
        Matrix_Free(dy);
        Matrix_Add(dh, dhnext);
        //backprop thourhg tanh
        hsRowT = Matrix_VectorCol(hs, t + 1);
        Matrix_ElementWiseFunc2M(hsRowT, hsRowT, mult);
        Matrix_ElemetWiseFunc1M(hsRowT, minusOne);
        Matrix_ElementWiseFunc2M(hsRowT, dh, mult);
        Matrix_Free(dh);

        Matrix_Add(dbh, hsRowT);

        // input layer and hidden layer
        matrix* dhraw= Matrix_Create(1, 1);
        Matrix_Copy(hsRowT, dhraw);
        matrix * xsRowT = Matrix_VectorCol(input, t);
        Matrix_Transpose(xsRowT);
        Matrix_DotProd(dhraw, xsRowT);
        Matrix_Free(xsRowT);
        Matrix_Add(dWxh, dhraw);

        Matrix_Copy(hsRowT, dhraw);

        matrix* hsRowTMinusOne = Matrix_VectorCol(hs, t);
        Matrix_Transpose(hsRowTMinusOne);
        Matrix_DotProd(dhraw, hsRowTMinusOne);
        Matrix_Free(hsRowTMinusOne);
        Matrix_Add(dWhh, dhraw);

        //dhnext
        matrix* Whh = Matrix_Create(1 , 1);
        // Matrix_Copy(rnn->Whh, Whh);
        Matrix_Copy(rnn->Whh, Whh);
        Matrix_Transpose(Whh);
        Matrix_Copy(hsRowT, dhraw);
        Matrix_Free(hsRowT);
        Matrix_DotProd(Whh, dhraw);
        Matrix_Free(dhraw);
        Matrix_Copy(Whh, dhnext);
        Matrix_Free(Whh);
    }
    printf("finished gd\n");

    // gradient clipping
    // loop over all values of all gradients and clip between -5 and 5

    // Freeing
    Matrix_Free(ps);
    
    // might want to change this to an out variable instead?
    gradient_info* gi = malloc(sizeof(gradient_info));
    gi->loss = loss;
    // printf("Loss: %lf", gi->loss);
    gi->dWxh = dWxh;
    gi->dWhh = dWhh;
    gi->dWhy = dWhy;
    gi->dbh = dbh;
    gi->dby = dby;
    printf("asdfasdfasdf\n");
    gi->h = Matrix_VectorCol(hs, rnn->seqLen-1);
    printf("asdfasdfasdf\n");
    Matrix_Free(hs);
    
    return gi;
}

double AddTiny(double a) {
    return a + 1e-8;
}
double Sqrt(double a) {
    return sqrt(a);
}
double Divide(double a, double b) {
    return a/b;
}

int ApplyAdagrad(matrix* parameter, matrix* dparameter, matrix* memParameter, double learningRate) {
    //updating values with adagrad
    matrix* copy = Matrix_Create(1, 1);
    matrix* memCopy = Matrix_Create(1, 1);
    Matrix_Copy(dparameter, copy);
    Matrix_DotProd(copy, copy);
    Matrix_Add(memParameter, copy);
    Matrix_Copy(dparameter, copy);
    // param += -learning_rate * dparam / np.sqrt(mem + 1e-8) # adagrad update
    Matrix_Copy(memParameter, memCopy);
    Matrix_ElemetWiseFunc1M(memCopy, &AddTiny);
    Matrix_ElemetWiseFunc1M(memCopy, &Sqrt); // could potentially just use the math library function instead??
    Matrix_Scaler(copy, -learningRate);
    Matrix_ElementWiseFunc2M(copy, memCopy, &Divide);
    Matrix_Add(parameter, copy);
    
    Matrix_Free(copy);
    Matrix_Free(memCopy);
}

int TrainRNN(rnn* r, training_data* epoch) {
    // checks
    if (epoch->input->size[1] != epoch->output->size[1]) {
        printf("input cols and output cols don't match\n");
        return EXIT_FAILURE;
    }
    if (r->inputSize != epoch->input->size[0]) {
        printf("epoch input size and input size don't match\n");
        printf("epoch: %d, r-inputsize: %d\n", epoch->input->size[0], r->inputSize);
        return EXIT_FAILURE;
    }
    if (r->outputSize != epoch->output->size[0]) {
        printf("output epoch issues\n");
        return EXIT_FAILURE;
    }

    int batch_size = (int)round(((float)epoch->input->size[1]) / epoch->iterations);
    int batch_position = 0;
    double smoothLoss = -log((double)1/epoch->input->size[0])*batch_size;

    // memory matrices for adagrad
    matrix* mWxh = Matrix_Create(r->Wxh->size[0], r->Wxh->size[1]);
    matrix* mWhh = Matrix_Create(r->Whh->size[0], r->Whh->size[1]);
    matrix* mWhy = Matrix_Create(r->Why->size[0], r->Why->size[1]);
    
    matrix* mbh = Matrix_Create(r->bh->size[0], r->bh->size[1]);
    matrix* mby = Matrix_Create(r->by->size[0], r->by->size[1]);
    

    // sub matrices of the epoch's input/output matrices
    matrix* Xsub;
    matrix* Ysub;

    matrix* hprev = Matrix_Create(r->hiddenSize, 1);

    while (batch_position < epoch->input->size[1]) {
        printf("epoch iteration started\n");
        if (batch_position + batch_position > epoch->input->size[1]) {
            // reduce batch size
            printf("have to reduce batch size\n");
            batch_size = epoch->input->size[1] - batch_position;
        }
        r->seqLen=batch_size;
        // need to reset the num of rows cause of the if above
        Xsub = Matrix_SubMatrix(epoch->input, 0, epoch->input->size[0], batch_position, batch_position + batch_size);
        Ysub = Matrix_SubMatrix(epoch->output, 0, epoch->output->size[0], batch_position, batch_position + batch_size);
        // do the actual training
        printf("loss func started\n");
        gradient_info* gi = LossFunc(r, Xsub, Ysub, hprev);
        printf("loss func finished\n");
        
        smoothLoss = smoothLoss * 0.999 + gi->loss * 0.001;
        // update parameters
        ApplyAdagrad(r->Wxh, gi->dWxh, mWxh, r->learningRate);
        ApplyAdagrad(r->Whh, gi->dWhh, mWhh, r->learningRate);
        ApplyAdagrad(r->Why, gi->dWhy, mWhy, r->learningRate);
        ApplyAdagrad(r->by, gi->dby, mby, r->learningRate);
        ApplyAdagrad(r->bh, gi->dbh, mbh, r->learningRate);


        batch_position += batch_size;

        // free matricies
        Matrix_Free(gi->dWxh);
        Matrix_Free(gi->dWhh);
        Matrix_Free(gi->dWhy);
        Matrix_Free(gi->dby);
        Matrix_Free(gi->dbh);
        // Matrix_Free(gi->h);
        Matrix_Free(hprev);
        hprev = gi->h;
        
        Matrix_Free(Xsub);
        Matrix_Free(Ysub);
        printf("Loss: %lf\n", gi->loss);
        free(gi);
        printf("bathc position: %d batch size: %d\n", batch_position, batch_size);
        int n;
        scanf("%d", &n);
    }

    Matrix_Free(mWxh);
    Matrix_Free(mWhh);
    Matrix_Free(mWhy);
    Matrix_Free(mby);
    Matrix_Free(mbh);

    Matrix_Free(hprev);

    return EXIT_SUCCESS;
}

double SampleNormalDistribution() {
    double u1 = 0;
    while (u1 == 0) {
        u1 = (double)rand() / RAND_MAX;
    }
    double u2 = (double)rand() / RAND_MAX;
    const double TOW_PI = 2. * M_PI;
    
    double mag = sqrt(-2. * log(u1));
    double z1 = cos(TOW_PI * u2);
    // double z2 = cos(TOW_PI * u2);

    return z1;
}

int InitializeWeights(rnn* r) {
    r->Wxh = Matrix_Initialize(r->hiddenSize, r->inputSize, &SampleNormalDistribution);
    r->Whh = Matrix_Initialize(r->hiddenSize, r->hiddenSize, &SampleNormalDistribution);
    r->Why = Matrix_Initialize(r->outputSize, r->hiddenSize, &SampleNormalDistribution);
    r->by = Matrix_Initialize(r->outputSize, 1, &SampleNormalDistribution);
    r->bh = Matrix_Initialize(r->hiddenSize, 1, &SampleNormalDistribution);

    return EXIT_SUCCESS;
}

int FreeWeights(rnn* r) {
    Matrix_Free(r->Wxh);
    Matrix_Free(r->Whh);
    Matrix_Free(r->Why);
    Matrix_Free(r->by);
    Matrix_Free(r->bh);
    return EXIT_SUCCESS;
}