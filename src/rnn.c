#include "rnn.h"

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


int TrainRNN(rnn* r, training_data* epoch) {
    
}