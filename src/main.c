#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "matrixMath.h"

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

int main() {
    // limiting how much we read so that it doesn't eat up memory
    int limit = 20000;

    FILE* fp;
    char row[200];
    char* token;

    char* fileName = "data/TheExaminer_SpamClickbaitCatalog/examiner_date_tokens_short_onehot.csv";
    
    int one_hot_size = -1;
    int dataset_size = 0;

    // only for linux :p
    if (access(fileName, F_OK) != 0) {
        // file doesn't exist;
        printf("Formatted data file hasn't been created yet, creating it now\n");
        int status = system("python3 data/TheExaminer_SpamClickbaitCatalog/FormatData.py");
        if (status == EXIT_SUCCESS) {
            printf("Finished creating file\n");
        } else {
            printf("Error creating file, exiting now\n");
            return EXIT_FAILURE;
        }
    }

    printf("Getting dataset size\n");
    // get datset size
    fp = fopen(fileName, "r");
    while(fgets(row, 200, fp)) {
        dataset_size++;
        if (dataset_size >= limit) {
            break; // ensure it doesn't get too big
        }
        if (one_hot_size != -1) {
            continue;
        }
        token = strtok(row, "|"); // get the first token, since all of them should be the same size
        one_hot_size = strlen(token);
    }
    fclose(fp);
    printf("Dataset size: %d\n", dataset_size);
    
    printf("Starting on allocating matrix\n");
    // x size of n-1 x char_types size
    // y size of n-1 x char_types size
    matrix* X = Matrix_Create(dataset_size-1, one_hot_size);
    matrix* Y = Matrix_Create(dataset_size-1, one_hot_size);
    double* values;
    int i = 0;
    fp = fopen(fileName, "r");
    // populate X and Y matricies
    while(fgets(row, 200, fp)) {
        if (i >= limit) {
            break;
        }
        
        char* token = strtok(row, "|");
        // setting x value
        values = GetValuesFromString(token, one_hot_size);
        for (int j = 0; j < one_hot_size; j++)
        {
            Matrix_Set(X, i, j, values[j]);
        }
        
        token = strtok(NULL, "|");
        // setting y value
        values = GetValuesFromString(token, one_hot_size);
        for (int j = 0; j < one_hot_size; j++)
        {
            Matrix_Set(Y, i, j, values[j]);
        }

        free(values);
        i++;
    }

    printf("done assign values to X and Y matricies\n");

    Matrix_Free(X);
    Matrix_Free(Y);

    return EXIT_SUCCESS;
}