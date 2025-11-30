#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "matrixMath.h"
#include "rnn.h"

double* GetValuesFromString(char* str, int len) {
    double* values = malloc(sizeof(double) * len);
    char* eptr;
    char* substringValue = malloc(sizeof(char) * 2);
    for (int i = 0; i < len; i++)
    {
        strncpy(substringValue, str+i, 1);
        substringValue[1]='\0';
        values[i] = strtod(substringValue, &eptr);
    }
    free(substringValue);
    return values;
}

char ixToChar[1]; // will reinitalize the array later

void Sample(matrix* results) {
    //printf("Sample from model: \n");
    //for (int i = 0; i < results->size[0]; i++)
    //{
    //    printf("%c", ixToChar[(int)Matrix_Get(results, i, 0)]);
    //}
    //printf("\n");
}

int main() {
    // limiting how much we read so that it doesn't eat up memory
    int limit = 10000;

    FILE* fp;
    char row[200];
    char* token;

    char* fileName = "data/winequality-white.csv";
    char* fileName2 = "data/winequality-red.csv";
    //char* onehotFileName = "data/TheExaminer_SpamClickbaitCatalog/oneHotRep.csv";

    int one_hot_size = -1;
    int dataset_size = 0;

    // only for linux :p
    //if (access(fileName, F_OK) != 0) {
    //    // file doesn't exist;
    //    printf("Formatted data file hasn't been created yet, creating it now\n");
    //    int status = system("python3 data/TheExaminer_SpamClickbaitCatalog/FormatData.py");
    //    if (status == EXIT_SUCCESS) {
    //        printf("Finished creating file\n");
    //    } else {
    //        printf("Error creating file, exiting now\n");
    //        return EXIT_FAILURE;
    //    }
    //}

    printf("Getting dataset size\n");
    // get datset size
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
        token = strtok(row, ";"); // get the first token, since all of them should be the same size
        one_hot_size = 0;
        while (token != NULL) {
            one_hot_size++;
            token = strtok(NULL, ";");
        }
    }
    fclose(fp);
    
    fp = fopen(fileName2, "r");
    fgets(row, 200, fp); // because the first row are the column names 
    while(fgets(row, 200, fp)) {
        dataset_size++;
        if (dataset_size >= limit) {
            break; // ensure it doesn't get too big
        } 
    }
    fclose(fp);

    printf("Dataset size: %d\n", dataset_size);
    printf("Input size: %d\n", one_hot_size);

    //ixToChar[one_hot_size];
    //fp = fopen(onehotFileName, "r");
    //int i = 0;
    //while(fgets(row, 200, fp)) {
    //    char* token = strtok(row, ",");
    //    ixToChar[i] = token[0];
    //    i++;
    //}
    //fclose(fp);

    
    printf("Starting on allocating matrix\n");
    // x size of n-1 x char_types size
    // y size of n-1 x char_types size
    matrix* X = Matrix_Create(one_hot_size-1, dataset_size-1);
    matrix* Y = Matrix_Create(2, dataset_size-1);
    double* values;
    int i = 0;
    fp = fopen(fileName, "r");
    // populate X and Y matricies
    fgets(row, 200, fp); // because the first row are the column names  
    char* eptr;
    while(fgets(row, 200, fp)) {
        if (i > limit) {
            break;
        }
        
        char* token = strtok(row, ";");
        // setting x value
        //values = GetValuesFromString(token, one_hot_size);
        //for (int j = 0; j < one_hot_size; j++)
        //{
        //    Matrix_Set(X, j, i, values[j]);
        //}
        
        //token = strtok(NULL, "|");
        // setting y value
        //values = GetValuesFromString(token, one_hot_size);
        //for (int j = 0; j < one_hot_size; j++)
        //{
        //    Matrix_Set(Y, j, i, values[j]);
        //}

        //free(values);
        int rowi = 0;
        while (token != NULL) { 
            Matrix_Set(X, rowi, i, strtod(token, &eptr));        
            rowi++;
            token = strtok(NULL, ";");
        }
        Matrix_Set(Y, 0, 0, 1);
        i++;
    }
    fp = fopen(fileName2, "r");
    // populate X and Y matricies
    fgets(row, 200, fp); // because the first row are the column names  
    while(fgets(row, 200, fp)) {
        if (i > limit) {
            break;
        }
        
        char* token = strtok(row, ";");
 
        int rowi = 0;
        while (token != NULL) { 
            Matrix_Set(X, rowi, i, strtod(token, &eptr));        
            rowi++;
            token = strtok(NULL, ";");
        }
        Matrix_Set(Y, 1, 0, 1);
        i++;
    }
    printf("%lf\n", Matrix_Get(Y, 0, 0));
   

    rnn* r = malloc(sizeof(rnn));
    printf("input size: %d\n", one_hot_size);
    r->inputSize = X->size[0];
    r->hiddenSize = 100;
    r->outputSize = Y->size[0];
    r->learningRate = 1e-1;
    InitializeWeights(r);

    training_data* epoch = malloc(sizeof(epoch));
    epoch->iterations = dataset_size/50;
    epoch->input = X;
    epoch->output = Y;

    printf("======= training started =======\n");
    int result = TrainRNN(r, epoch, 100, &Sample);
    printf("======= training finished with exit code: %d =======\n", result);

    matrix* results = evaluate(r);
    // convert results to string
    //printf("Sample from model: \n");
    //for (int i = 0; i < results->size[0]; i++)
    //{
    //    printf("%c", ixToChar[(int)Matrix_Get(results, i, 0)]);
    //}
    // 100, 50 = 87z77677x77z7777777777x7777z7777777777x7777x77777
    // 275, 30 = uv5vvuuuuuuuuuuuuuu 
    //printf("\n");
    
    
    FreeWeights(r);
    free(r);
    Matrix_Free(X);
    Matrix_Free(Y);

    return EXIT_SUCCESS;
}
