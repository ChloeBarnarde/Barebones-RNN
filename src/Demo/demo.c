#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "matrixMath.h"
#include "rnn.h"

FILE* lossFile;
void Sample(double smoothLoss, double test_smoothLoss) {
   fprintf(lossFile, "%lf,%lf\n", smoothLoss, test_smoothLoss); 
}

void CreateConfusionMatrix(rnn* r, rnn_epcoh* epoch) {
    lossFile = fopen("Output/ConfusionMatrixValues.csv", "w");
    fprintf(lossFile, "TR,PR,TW,PW\n");

    r->seqLen = epoch->testInput->size[1];
    printf("Evaluating confusion matrix on test data with seqLen: %d\n", r->seqLen);
    matrix* results = rnn_evaluate(r, epoch->testInput);
    // confusion matrix values
    int true_red = 0;
    int pred_red = 0; // wrong prediction
    int true_white = 0;
    int pred_white = 0; // wrong prediction

    for (int t = 0; t < r->seqLen; t++)
    {
        // find predicted class
        int pred_class = -1;
        double max_prob = 0;
        for (int rowi = 0; rowi < results->size[0]; rowi++)
        {
            if (matrix_get(results, rowi, t) > max_prob) {
                max_prob = matrix_get(results, rowi, t);
                pred_class = rowi;
            }
        }

        // find true class
        int true_class = -1;
        for (int rowi = 0; rowi < epoch->testOutput->size[0]; rowi++)
        {
            if (matrix_get(epoch->testOutput, rowi, t) == 1) {
                true_class = rowi;
                break;
            }
        }

        if (true_class == 0) {
            // true red
            if (pred_class == 0) {
                true_red++;
            } else {
                pred_white++;
            }
        } else {
            // true white
            if (pred_class == 1) {
                true_white++;
            } else {
                pred_red++;
            }
        }
    }
    fprintf(lossFile, "%d,%d,%d,%d\n", true_red, pred_red, true_white, pred_white);


    matrix_free(results);

    fclose(lossFile);
}
int main() {
    // limiting amount of observations
    int limit = 10000;

    FILE* fp;
    char row[200];
    char* token;

    char* fileName = "data/train_data.csv";
    char* fileNameTest = "data/test_data.csv";

    int one_hot_size = -1;
    int dataset_size = 0;

    // only for linux :p
    if (access(fileName, F_OK) != 0 || access(fileNameTest, F_OK) != 0) {
       // files don't exist
       printf("Formatted data file hasn't been created yet, creating it now\n");
       int status = system("python3 data/FormatData.py");
       if (status == EXIT_SUCCESS) {
           printf("Finished creating file\n");
       } else {
           printf("Error creating file, exiting now\n");
           return EXIT_FAILURE;
       }
    }

    printf("Getting dataset size\n");
    // get training dataset size
    fp = fopen(fileName, "r");
    fgets(row, 200, fp); // because the first row are the column names 
    while(fgets(row, 200, fp)) {
        dataset_size++;
        if (dataset_size >= limit) {
            break; // ensure it doesn't get too big
        }
        if (one_hot_size != -1) {
            continue;
        }
        token = strtok(row, ",");
        // one_hot_size = 0;
        while (token != NULL) {
            one_hot_size++;
            token = strtok(NULL, ",");
        }
    }
    fclose(fp);
    
    printf("Dataset size: %d\n", dataset_size);
    printf("Input size: %d\n", one_hot_size);
    
    printf("Starting on allocating matrix\n");
    // x size of n-1 x char_types size
    // y size of n-1 x char_types size
    matrix* X = matrix_create(one_hot_size, dataset_size);
    matrix* Y = matrix_create(2, dataset_size); 
    int i = 0;
    fp = fopen(fileName, "r");
    // populate X and Y matricies
    fgets(row, 200, fp); // because the first row are the column names  
    char* eptr;
    while(fgets(row, 200, fp)) {
        if (i > limit) {
            break;
        }
        char* token = strtok(row, ",");
        int rowi = 0;
        while (token != NULL) { 
            if (rowi > 10) {
                // could probably rewrite this to use that value as the row
                // set Y
                if (strtod(token, &eptr) == 0) {
                    // red wine
                    Matrix_Set(Y, 0, i, 1);
                } else {
                    // white wine
                    Matrix_Set(Y, 1, i, 1);
                }
            } else {
                Matrix_Set(X, rowi, i, strtod(token, &eptr));
            }
                    
            rowi++;
            token = strtok(NULL, ",");
        }
        
        i++;
    }
    fclose(fp);

    // get test dataset size
    int test_dataset_size = 0;
    int test_one_hot_size = -1;
    fp = fopen(fileNameTest, "r");
    fgets(row, 200, fp); // because the first row are the column names 
    while(fgets(row, 200, fp)) {
        test_dataset_size++;
        if (test_dataset_size >= limit) {
            break; // ensure it doesn't get too big
        }
        if (test_one_hot_size != -1) {
            continue;
        }
        token = strtok(row, ","); // get the first token, since all of them should be the same size
        // one_hot_size = 0;
        while (token != NULL) {
            test_one_hot_size++;
            token = strtok(NULL, ",");
        }
    }
    fclose(fp);
    matrix* testX = matrix_create(test_one_hot_size, test_dataset_size);
    matrix* testY = matrix_create(2, test_dataset_size); 
    i = 0;
    fp = fopen(fileNameTest, "r");
    // populate X and Y matricies
    fgets(row, 200, fp); // because the first row are the column names  
    while(fgets(row, 200, fp)) {
        if (i > limit) {
            break;
        }
        char* token = strtok(row, ",");
        int rowi = 0;
        while (token != NULL) { 
            if (rowi > 10) {
                // could probably rewrite this to use that value as the row
                // set Y
                if (strtod(token, &eptr) == 0) {
                    // red wine
                    Matrix_Set(testY, 0, i, 1);
                } else {
                    // white wine
                    Matrix_Set(testY, 1, i, 1);
                }
            } else {
                Matrix_Set(testX, rowi, i, strtod(token, &eptr));
            }
                    
            rowi++;
            token = strtok(NULL, ",");
        }
        
        i++;
    }
    fclose(fp);
    
    rnn* r = malloc(sizeof(rnn));
    r->inputSize = X->size[0];
    r->hiddenSize = 50;
    r->outputSize = Y->size[0];
    r->learningRate = 1e-1;
    rnn_initialize_weights(r);
    
    rnn_epcoh* epoch = malloc(sizeof(rnn_epcoh));
    epoch->iterations = dataset_size/50;
    epoch->input = X;
    epoch->output = Y;
    epoch->testInput = testX;
    epoch->testOutput = testY;
    
    
    lossFile = fopen("Output/output.csv", "w");
    fprintf(lossFile, "TrainLoss,TestLoss\n"); 
    printf("======= training started =======\n");
    int result = rnn_train(r, epoch, 200, &Sample);
    printf("======= training finished with exit code: %d =======\n", result);
    fclose(lossFile);

    CreateConfusionMatrix(r, epoch);    

    system("python3 Output/createGraphs.py");

    printf("Graphs have been saved to ./Output\n");
    
    rnn_free_weights(r);
    free(r);
    matrix_free(X);
    matrix_free(Y);

    return EXIT_SUCCESS;
}
