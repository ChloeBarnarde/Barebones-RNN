#include <math.h>
#include "rnn.h"
#include "matrixMath.h"

// a lot of this code takes insporation from Andrej Karpathy's blog https://karpathy.github.io/2015/05/21/rnn-effectiveness/
// in a lot of cases it is a c implementation of the minimal character-level RNN language model in Python/numpy which he made.

int ContainsNan(matrix* m) {
    for (int rowi = 0; rowi < m->size[0]; rowi++)
    {
        for (int coli = 0; coli < m->size[1]; coli++)
        {
            if (!isfinite(matrix_get(m, rowi, coli))) {
                return 1;
            }
        }
        
    }
    return 0;
}


void Clip(matrix* m, double min, double max) {
    for (int rowi = 0; rowi < m->size[0]; rowi++)
    {
        for (int coli = 0; coli < m->size[1]; coli++)
        {
            if (matrix_get(m, rowi, coli) < min) {
                Matrix_Set(m, rowi, coli, min);
            }
            
            if (matrix_get(m, rowi, coli) > max) {
                Matrix_Set(m, rowi, coli, max);
            }
        }
        
    }
    
}

double mult(double a, double b) { return a * b; }
double minusOne(double a) {return 1 - a; }

gradient_info* rnn_calc_gradient(rnn* rnn, matrix* input, matrix* target, matrix* hprev) {

    // target and input is currently one hot encoded
    matrix* hs = matrix_create(rnn->hiddenSize, rnn->seqLen + 1);
    for (int rowi = 0; rowi < hs->size[0]; rowi++)
    {
        Matrix_Set(hs, rowi, 0, matrix_get(hprev, rowi, 0));
    }

    matrix* ys = matrix_create(rnn->outputSize, rnn->seqLen);
    matrix* ps = matrix_create(rnn->outputSize, rnn->seqLen);
    double loss = 0;
    
    // for testing
    int correct_count = 0;
    for (int t = 0; t < rnn->seqLen; t++)
    {
        // matrices to free:
        //  - Wxh - done
        //  - Whh- done
        //  - inputRowT - done
        //  - hsRowTminusOne - done
        //  - Why - done
        //  - hsRowT - done
        
        matrix* Wxh= matrix_create(rnn->Wxh->size[0], rnn->Wxh->size[1]);
        matrix_copy(rnn->Wxh, Wxh);
        matrix* Whh = matrix_create(rnn->Whh->size[0], rnn->Whh->size[1]);
        matrix_copy(rnn->Whh, Whh);
        
        
        // evaluating 
        
        // function: tanh(Wxh * xs[t] + Whh * hs[t-1] + bh)
        matrix* inputRowT = matrix_get_vector_col(input, t);
        matrix_dot_prod(Wxh, inputRowT);
        matrix_free(inputRowT);
        
        matrix* hsRowTMinusOne = matrix_get_vector_col(hs, t); // t = t-1 becauwse length = seqLen+1
        matrix_dot_prod(Whh, hsRowTMinusOne);
        matrix_free(hsRowTMinusOne);
        matrix_add(Wxh, Whh);
        matrix_free(Whh);
        matrix_add(Wxh, rnn->bh);
        for (int rowi = 0; rowi < hs->size[0]; rowi++)
        {
            Matrix_Set(hs, rowi, t+1, tanh(matrix_get(Wxh, rowi, 0)));
        }
        matrix_free(Wxh);

        //function: Why * hs[t] + by
        matrix* Why = matrix_create(rnn->Why->size[0], rnn->Why->size[1]);
        matrix* hsRowT = matrix_get_vector_col(hs, t+1);
        matrix_dot_prod(Why, hsRowT);
        matrix_free(hsRowT);
        matrix_add(Why, rnn->by); // Why = ys
        // fix here. set col t of ys to why.
        //  i.e. ys[t] = Why
        for (int rowi = 0; rowi < Why->size[0]; rowi++)
        {
            Matrix_Set(ys, rowi, t, matrix_get(Why, rowi, 0));
        }
        // matrix_copy(Why, ys);
        matrix_free(Why);
        
        // for probobilities
        double sum = 0;
        for (int rowi = 0; rowi < ys->size[0]; rowi++)
        {
            Matrix_Set(ys, rowi, t, exp(matrix_get(ys, rowi, t)));
            sum += matrix_get(ys, rowi, t);
        }
        
        for (int rowi = 0; rowi < ps->size[0]; rowi++)
        {
            Matrix_Set(ps, rowi, t, matrix_get(ys, rowi, t) / sum);
        }

        if (ContainsNan(ps)) {
            printf("NaN detected in ps at time %d\n", t);
        }
        // testing method
        int ix=0;
        double max = 0;
        for (int i = 0; i < ps->size[0]; i++)
        {
            if (max >= matrix_get(ps, i, t)) {continue;}
            max = matrix_get(ps, i, 0);
            ix = i;
        }
        
        if (ix == 0) {
            correct_count++;
        }
        // correct_count += matrix_get(target, ix, t);

        // softmax loss
        for (int rowi = 0; rowi < ps->size[0]; rowi++)
        {
            loss -= matrix_get(target, rowi, t) * log(matrix_get(ps, rowi, t)); 
        }
    }
    // printf("\033[KCorrect count: %d/%d\n\033[1A", correct_count, rnn->seqLen);

    // === gradient ===
    // gradient of matrices
    matrix* dWxh = matrix_create(rnn->Wxh->size[0], rnn->Wxh->size[1]);
    matrix* dWhy = matrix_create(rnn->Why->size[0], rnn->Why->size[1]);
    matrix* dWhh = matrix_create(rnn->Whh->size[0], rnn->Whh->size[1]);
    // gradient of vectors
    matrix* dbh = matrix_create(rnn->bh->size[0], rnn->bh->size[1]);
    matrix* dby = matrix_create(rnn->by->size[0], rnn->by->size[1]);
    //gradietn of hidden vector
    matrix* dhnext = matrix_create(rnn->hiddenSize, 1);

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
        matrix* dy = matrix_get_vector_col(ps, t);
        // want to find true label
        for (int rowi = 0; rowi < dy->size[0]; rowi++)
        {
            if (matrix_get(target, rowi, t) != 1)  {
                // only want to update the actual labels
                continue;
            }
            Matrix_Set(dy, rowi, 0, matrix_get(dy, rowi, 0) - 1);
            break;
        }
        
        // dE/dy[j]*dy[j]/dWhy[j,k] = dE/dy[j] * h[k]
        matrix* dyCopy = matrix_create(1, 1);
        matrix_copy(dy, dyCopy);
        matrix* hsRowT = matrix_get_vector_col(hs, t+1);
        matrix_transpose(hsRowT);
        matrix_dot_prod(dyCopy, hsRowT);
        matrix_add(dWhy, dyCopy);
        matrix_free(dyCopy);
        matrix_add(dby, dy);
        matrix_free(hsRowT);

        //backprop into h
        matrix* dh = matrix_create(1, 1);
        matrix_copy(rnn->Why, dh);
        matrix_transpose(dh);
        matrix_dot_prod(dh, dy);
        matrix_free(dy);
        matrix_add(dh, dhnext);
        //backprop thourhg tanh
        hsRowT = matrix_get_vector_col(hs, t + 1);
        matrix_elementwise_func2m(hsRowT, hsRowT, &mult);
        matrix_elemetwise_func1m(hsRowT, &minusOne);
        matrix_elementwise_func2m(hsRowT, dh,&mult);
        matrix_free(dh);

        matrix_add(dbh, hsRowT);

        // input layer and hidden layer
        matrix* dhraw= matrix_create(1, 1);
        matrix_copy(hsRowT, dhraw);
        matrix * xsRowT = matrix_get_vector_col(input, t);
        matrix_transpose(xsRowT);
        matrix_dot_prod(dhraw, xsRowT);
        matrix_free(xsRowT);
        matrix_add(dWxh, dhraw);

        matrix_copy(hsRowT, dhraw);

        matrix* hsRowTMinusOne = matrix_get_vector_col(hs, t);
        matrix_transpose(hsRowTMinusOne);
        matrix_dot_prod(dhraw, hsRowTMinusOne);
        matrix_free(hsRowTMinusOne);
        matrix_add(dWhh, dhraw);

        //dhnext
        matrix* Whh = matrix_create(1 , 1);
        // matrix_copy(rnn->Whh, Whh);
        matrix_copy(rnn->Whh, Whh);
        matrix_transpose(Whh);
        matrix_copy(hsRowT, dhraw);
        matrix_free(hsRowT);
        matrix_dot_prod(Whh, dhraw);
        matrix_free(dhraw);
        matrix_copy(Whh, dhnext);
        matrix_free(Whh);
    }

    // gradient clipping
    // loop over all values of all gradients and clip between -5 and 5
    Clip(dWxh, -5, 50);
    Clip(dWhh, -5, 50);
    Clip(dWhy, -5, 50);
    Clip(dby, -5, 50);
    Clip(dbh, -5, 50);

    // Freeing
    matrix_free(ps);
    matrix_free(ys); 

    gradient_info* gi = malloc(sizeof(gradient_info));
    gi->loss = loss;
    gi->dWxh = dWxh;
    gi->dWhh = dWhh;
    gi->dWhy = dWhy;
    gi->dbh = dbh;
    gi->dby = dby;
    gi->h = matrix_get_vector_col(hs, rnn->seqLen);
    matrix_free(hs);
    matrix_free(dhnext); 
    return gi;
}

double AddTiny(double a) {
    return a + 1e-8;
}
double Sqrt(double a) {
    if (a < 0) {
        return 0;
    }
    return sqrt(a);
}
double Divide(double a, double b) {
    if (b == 0) {
        return 0;
    }
    return a/b;
}

int ApplyAdagrad(matrix* parameter, matrix* dparameter, matrix* memParameter, double learningRate) {
    //updating values with adagrad
    matrix* copy = matrix_create(1, 1);
    matrix* copy2 = matrix_create(1, 1);
    matrix* memCopy = matrix_create(1, 1);
    matrix_copy(dparameter, copy);
    matrix_copy(dparameter, copy2);
    matrix_dot_prod(copy, copy2);
    matrix_add(memParameter, copy);
    matrix_copy(dparameter, copy);
    // param += -learning_rate * dparam / np.sqrt(mem + 1e-8) # adagrad update
    matrix_copy(memParameter, memCopy);
    matrix_elemetwise_func1m(memCopy, &AddTiny);
    matrix_elemetwise_func1m(memCopy, &Sqrt); // could potentially just use the math library function instead??
    matrix_scaler(copy, -learningRate);
    matrix_elementwise_func2m(copy, memCopy, &Divide);
    matrix_add(parameter, copy);
    
    matrix_free(copy);
    matrix_free(copy2);
    matrix_free(memCopy);

    return EXIT_SUCCESS;
}

int rnn_train(rnn* r, rnn_epcoh* epoch, int limit, void (*onComplete)(double, double)) {
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

    int epochIter = 0;
    int batch_size = (int)round(((float)epoch->input->size[1]) / epoch->iterations);
    int batch_position = 0;
    double smoothLoss = -log((double)1/epoch->input->size[0])*batch_size;
    double test_smoothLoss = -log((double)1/epoch->input->size[0])*batch_size;

    // memory matrices for adagrad
    matrix* mWxh = matrix_create(r->Wxh->size[0], r->Wxh->size[1]);
    matrix* mWhh = matrix_create(r->Whh->size[0], r->Whh->size[1]);
    matrix* mWhy = matrix_create(r->Why->size[0], r->Why->size[1]);
    
    matrix* mbh = matrix_create(r->bh->size[0], r->bh->size[1]);
    matrix* mby = matrix_create(r->by->size[0], r->by->size[1]);
    

    // sub matrices of the epoch's input/output matrices
    matrix* Xsub;
    matrix* Ysub;

    matrix* hprev = matrix_create(r->hiddenSize, 1);

    // setup for progressBar
    int batchBarSize = 40;
    char batchBar[batchBarSize + 1];
    batchBar[batchBarSize] = '\0';
    for (int i = 0; i < batchBarSize; i++) {
        batchBar[i] = '_';
    }

    for (epochIter = 0; epochIter < limit; epochIter ++) {
        
        for (int i = 0; i < batchBarSize; i++) {
            batchBar[i] = '_';
        }
        printf("epoch: %d/%d Train Loss: %lf Test Loss: %lf\n", 
            epochIter, limit, smoothLoss, test_smoothLoss);
        int batchIter = 0;

        while (batch_position < epoch->input->size[1]) {
            if (batch_position + batch_size > epoch->input->size[1]) {
                // reduce batch size
                batch_size = epoch->input->size[1] - batch_position;
            }
            r->seqLen=batch_size;
            // need to reset the num of rows cause of the if above
            Xsub = matrix_submatrix(epoch->input, 0, epoch->input->size[0], batch_position, batch_position + batch_size);
            Ysub = matrix_submatrix(epoch->output, 0, epoch->output->size[0], batch_position, batch_position + batch_size);
            // do the actual training
            gradient_info* gi = rnn_calc_gradient(r, Xsub, Ysub, hprev);
    
            smoothLoss = smoothLoss * 0.999 + gi->loss * 0.001;     
    
            // update parameters
            ApplyAdagrad(r->Wxh, gi->dWxh, mWxh, r->learningRate);
            ApplyAdagrad(r->Whh, gi->dWhh, mWhh, r->learningRate);
            ApplyAdagrad(r->Why, gi->dWhy, mWhy, r->learningRate);
            ApplyAdagrad(r->by, gi->dby, mby, r->learningRate);
            ApplyAdagrad(r->bh, gi->dbh, mbh, r->learningRate);
    
    
            batch_position += batch_size;
    
            // free matricies
            matrix_free(gi->dWxh);
            matrix_free(gi->dWhh);
            matrix_free(gi->dWhy);
            matrix_free(gi->dby);
            matrix_free(gi->dbh);
            matrix_free(hprev);
            hprev = gi->h;
    
            matrix_free(Xsub);
            matrix_free(Ysub);
            free(gi);

            batchIter++;
            if (batchIter > epoch->iterations) {
                batchIter = epoch->iterations;
            }
            for (int i = 0; i < (int)round(((float)batchIter/epoch->iterations)*batchBarSize); i++)
            {
                batchBar[i]='#';
            }
            printf("\033[KbatchIter: %d [%s] SmoothLoss: %lf\n\033[1A", batchIter, batchBar, smoothLoss);
        }
        printf("\033[1A\033[K");
        // test
        matrix_free(hprev);
        hprev = matrix_create(r->hiddenSize, 1);
        batch_position = 0;
        batch_size = (int)round(((float)epoch->input->size[1]) / epoch->iterations);

        while (batch_position < epoch->testInput->size[1]) {
            if (batch_position + batch_size > epoch->testInput->size[1]) {
                // reduce batch size
                batch_size = epoch->testInput->size[1] - batch_position;
            }
            r->seqLen=batch_size;
            // need to reset the num of rows cause of the if above
            Xsub = matrix_submatrix(epoch->testInput, 0, epoch->testInput->size[0], batch_position, batch_position + batch_size);
            Ysub = matrix_submatrix(epoch->testOutput, 0, epoch->testOutput->size[0], batch_position, batch_position + batch_size);
            // do the actual testing
            double loss = rnn_test(r, Xsub, Ysub, hprev);
    
            test_smoothLoss = test_smoothLoss * 0.999 + loss * 0.001;
    
            batch_position += batch_size;
    
            // free matrices
            matrix_free(Xsub);
            matrix_free(Ysub);
        }    

        // reset values
        batch_position = 0;
        batch_size = (int)round(((float)epoch->input->size[1]) / epoch->iterations);
        
        // matrix* results = rnn_evaluate(r, epoch->testInput);
        onComplete(smoothLoss, test_smoothLoss);
        // matrix_free(results);
        
        matrix_free(hprev);
        hprev = matrix_create(r->hiddenSize, 1);

        }

    matrix_free(mWxh);
    matrix_free(mWhh);
    matrix_free(mWhy);
    matrix_free(mby);
    matrix_free(mbh);

    matrix_free(hprev);

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
    double z1 = mag * cos(TOW_PI * u2);
    // double z2 = sin(TOW_PI * u2); // don't need a second random number

    return z1;
}

int rnn_initialize_weights(rnn* r) {
    r->Wxh = matrix_initialize(r->hiddenSize, r->inputSize, &SampleNormalDistribution);
    r->Whh = matrix_initialize(r->hiddenSize, r->hiddenSize, &SampleNormalDistribution);
    r->Why = matrix_initialize(r->outputSize, r->hiddenSize, &SampleNormalDistribution);
    r->by = matrix_initialize(r->outputSize, 1, &SampleNormalDistribution);
    r->bh = matrix_initialize(r->hiddenSize, 1, &SampleNormalDistribution);
    return EXIT_SUCCESS;
}

int rnn_free_weights(rnn* r) {
    matrix_free(r->Wxh);
    matrix_free(r->Whh);
    matrix_free(r->Why);
    matrix_free(r->by);
    matrix_free(r->bh);
    return EXIT_SUCCESS;
}


matrix* rnn_evaluate(rnn* r, matrix* inputs) {

    if (inputs->size[0] != r->inputSize) {
        printf("Input size does not match rnn input size\n");
        return NULL;
    }
    if (inputs->size[1] != r->seqLen) {
        printf("Input seqLen does not match rnn seqLen %d %d\n", inputs->size[1], r->seqLen);
        return NULL;
    }

    matrix* h = matrix_create(r->hiddenSize, 1);
    matrix* result = matrix_create(r->outputSize, r->seqLen);

    for (int t = 0; t < r->seqLen; t++)
    {
        matrix* x = matrix_get_vector_col(inputs, t);

        matrix* Wxh = matrix_create(1, 1);
        matrix_copy(r->Wxh, Wxh);
        matrix* Whh = matrix_create(1, 1);
        matrix_copy(r->Whh, Whh);
        matrix_dot_prod(Wxh, x);
        matrix_dot_prod(Whh, h);
        matrix_add(Wxh, Whh);
        matrix_add(Wxh, r->bh);
        for (int rowi = 0; rowi < h->size[0]; rowi++)
        {
            Matrix_Set(h, rowi, 0, tanh(matrix_get(Wxh, rowi, 0)));
        }

        matrix* Why = matrix_create(1, 1);
        matrix_copy(r->Why, Why);
        matrix_dot_prod(Why, h);
        matrix_add(Why, r->by);

        // Why is size of outputSize x 1
        matrix* p = matrix_create(Why->size[0], Why->size[1]);

        
        // for probobilities
        double sum = 0;
        for (int rowi = 0; rowi < Why->size[0]; rowi++)
        {
            Matrix_Set(Why, rowi, 0, exp(matrix_get(Why, rowi, 0)));
            sum += matrix_get(Why, rowi, 0);
        }
        
        for (int rowi = 0; rowi < p->size[0]; rowi++)
        {
            Matrix_Set(p, rowi, 0, matrix_get(Why, rowi, 0) / sum);
        }

        
        int ix=0;
        double max = 0;
        for (int i = 0; i < p->size[0]; i++)
        {
            if (max >= matrix_get(p, i, 0) ) {
                continue;
            }
            max = matrix_get(p, i, 0);
            ix = i;
        }

        Matrix_Set(result, ix, t, 1);
        matrix_free(x);
        
        matrix_free(p);
        matrix_free(Why);
        matrix_free(Whh);
        matrix_free(Wxh);
    }
    
    matrix_free(h);
    return result;
}

double rnn_calc_mse_loss(matrix* output, matrix* target) 
{
    // using MSE
    // L(\hat y, y) = \sum^{Y_y}_{t=1}L(\hat y_t, y_t)

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


double rnn_test(rnn* rnn, matrix* input, matrix* target, matrix* hprev) {
    // target and input is currently one hot encoded
    matrix* hs = matrix_create(rnn->hiddenSize, rnn->seqLen + 1);

    for (int rowi = 0; rowi < hs->size[0]; rowi++)
    {
        Matrix_Set(hs, rowi, 0, matrix_get(hprev, rowi, 0));
    }

    matrix* ys = matrix_create(rnn->outputSize, rnn->seqLen);
    matrix* ps = matrix_create(rnn->outputSize, rnn->seqLen);
    double loss = 0;
    
    for (int t = 0; t < rnn->seqLen; t++)
    {
        // matrices to free:
        //  - Wxh - done
        //  - Whh- done
        //  - inputRowT - done
        //  - hsRowTminusOne - done
        //  - Why - done
        //  - hsRowT - done
        
        matrix* Wxh= matrix_create(rnn->Wxh->size[0], rnn->Wxh->size[1]);
        matrix_copy(rnn->Wxh, Wxh);
        matrix* Whh = matrix_create(rnn->Whh->size[0], rnn->Whh->size[1]);
        matrix_copy(rnn->Whh, Whh);
        
        
        // evaluating 
        
        // function: tanh(Wxh * xs[t] + Whh * hs[t-1] + bh)
        matrix* inputRowT = matrix_get_vector_col(input, t);
        matrix_dot_prod(Wxh, inputRowT);
        matrix_free(inputRowT);
        
        matrix* hsRowTMinusOne = matrix_get_vector_col(hs, t); // t = t-1 because length = seqLen+1
        matrix_dot_prod(Whh, hsRowTMinusOne);
        matrix_free(hsRowTMinusOne);
        matrix_add(Wxh, Whh);
        matrix_free(Whh);
        matrix_add(Wxh, rnn->bh);
        for (int rowi = 0; rowi < hs->size[0]; rowi++)
        {
            Matrix_Set(hs, rowi, t+1, tanh(matrix_get(Wxh, rowi, 0)));
        }
        matrix_free(Wxh);

        //function: Why * hs[t] + by
        matrix* Why = matrix_create(rnn->Why->size[0], rnn->Why->size[1]);
        matrix* hsRowT = matrix_get_vector_col(hs, t+1);
        matrix_dot_prod(Why, hsRowT);
        matrix_free(hsRowT);
        matrix_add(Why, rnn->by); // Why = ys
        // fix here. set col t of ys to why.
        //  i.e. ys[t] = Why
        for (int rowi = 0; rowi < Why->size[0]; rowi++)
        {
            Matrix_Set(ys, rowi, t, matrix_get(Why, rowi, 0));
        }
        // matrix_copy(Why, ys);
        matrix_free(Why);
        
        // for probobilities
        double sum = 0;
        for (int rowi = 0; rowi < ys->size[0]; rowi++)
        {
            Matrix_Set(ys, rowi, t, exp(matrix_get(ys, rowi, t)));
            sum += matrix_get(ys, rowi, t);
        }
        
        for (int rowi = 0; rowi < ps->size[0]; rowi++)
        {
            Matrix_Set(ps, rowi, t, matrix_get(ys, rowi, t) / sum);
        }

        // softmax loss
        for (int rowi = 0; rowi < ps->size[0]; rowi++)
        {
            loss -= matrix_get(target, rowi, t) * log(matrix_get(ps, rowi, t)); 
        }
    }

    for (int rowi = 0; rowi < hs->size[0]; rowi++)
    {
        Matrix_Set(hprev, rowi, 0, matrix_get(hs, rowi, rnn->seqLen));
    }

    matrix_free(ys);
    matrix_free(ps);
    matrix_free(hs);

    return loss;
}
