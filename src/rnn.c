#include <math.h>
#include "rnn.h"
// a lot of this code takes insporation from Andrej Karpathy's blog https://karpathy.github.io/2015/05/21/rnn-effectiveness/
// in a lot of cases it is a c implementation of the minimal character-level RNN language model in Python/numpy which he made.

gradient_info* LossFunc(rnn* rnn, matrix* input, matrix* target, matrix* hprev) {

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
    // gradient of matrices
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
        matrix* dy = Matrix_VectorRow(ps, t);
        // want to find true label
        for (int coli = 0; coli < dy->size[1]; coli++)
        {
            if (Matrix_Get(target, t, coli) != 1) // only want to update the actual labels
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

    // gradient clipping
    // loop over all values of all gradients and clip between -5 and 5

    // Freeing
    Matrix_Free(hs);
    Matrix_Free(ps);

    // might want to change this to an out variable instead?
    gradient_info* gi = malloc(sizeof(gradient_info));
    gi->loss = loss;
    gi->dWxh = dWxh;
    gi->dWhh = dWhh;
    gi->dWhy = dWhy;
    gi->dbh = dbh;
    gi->dby = dby;
    gi->h = Matrix_VectorRow(hs, rnn->seqLen);

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
    if (epoch->input->size[0] != epoch->output->size[0]) {
        return EXIT_FAILURE;
    }
    if (r->inputSize != epoch->input->size[1]) {
        return EXIT_FAILURE;
    }
    if (r->outputSize != epoch->output->size[1]) {
        return EXIT_FAILURE;
    }

    int batch_size = (int)round(((float)epoch->input->size[0]) / epoch->iterations);
    int batch_position = 0;

    // memory matrices for adagrad
    matrix* mWxh = Matrix_Create(r->Wxh->size[0], r->Wxh->size[1]);
    matrix* mWhh = Matrix_Create(r->Whh->size[0], r->Whh->size[1]);
    matrix* mWhy = Matrix_Create(r->Why->size[0], r->Why->size[1]);
    
    matrix* mbh = Matrix_Create(r->bh->size[0], r->bh->size[1]);
    matrix* mby = Matrix_Create(r->by->size[0], r->by->size[1]);
    

    // sub matrices of the epoch's input/output matrices
    matrix* Xsub = malloc(sizeof(matrix));
    Xsub->size = malloc(sizeof(int) * 2);
    Xsub->size[1] = epoch->input->size[1];
    matrix* Ysub = malloc(sizeof(matrix));
    Ysub->size = malloc(sizeof(int) * 2);
    Ysub->size[1] = epoch->output->size[1];

    matrix* hprev = Matrix_Create(1, r->hiddenSize);

    while (batch_position < epoch->input->size[0]) {
        if (batch_position + batch_position > epoch->input->size[0]) {
            // reduce batch size
            batch_size = epoch->input->size[0] - batch_position;
        }

        // need to reset the num of rows cause of the if above
        Xsub->size[0] = batch_size;
        Ysub->size[0] = batch_size;
        Xsub->values = &epoch->input->values[batch_position];
        Ysub->values = &epoch->output->values[batch_position];
        // do the actual training
        gradient_info* gi = LossFunc(r, Xsub, Ysub, hprev);
        
        // update parameters
        ApplyAdagrad(r->Wxh, gi->dWxh, mWxh, r->learningRate);
        ApplyAdagrad(r->Whh, gi->dWhh, mWhh, r->learningRate);
        ApplyAdagrad(r->Why, gi->dWhy, mWhy, r->learningRate);
        ApplyAdagrad(r->by, gi->dby, mby, r->learningRate);
        ApplyAdagrad(r->bh, gi->dbh, mbh, r->learningRate);


        batch_position += batch_size;
    }

    // manually free matrix
    free(Xsub->size);
    free(Xsub);
    free(Ysub->size);
    free(Ysub);

    Matrix_Free(mWxh);
    Matrix_Free(mWhh);
    Matrix_Free(mWhy);
    Matrix_Free(mby);
    Matrix_Free(mbh);


    Matrix_Free(hprev);

}