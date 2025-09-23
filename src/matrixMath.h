#ifndef MATRIX_MATH
#define MATRIX_MATH

#include <stdio.h>
#include <stdlib.h>

// currently just a 2d matrix :p
struct {
    //int* dimensions; // can be added later ig

    int* size;
    float* values;
} typedef matrix;

int DotProd(matrix* a, matrix* b, matrix* output) {
    int size[2] = {a->size[0], b->size[1]}; // might need to rewrite this
    // need to make sure a->size[1] == b->size[0]
    if (a->size[1] == b->size[0]) {
        printf("Passed Matrices don't have sizes that can be multiplied together \n");
        return -1;
    }
    
    float* values = (float *)malloc(size[0] * sizeof(float) * size[1] * sizeof(float));
    output->size = size;
    output->values = values;
    // just gonna use naive implementation for now

    for (int i = 0; i < a->size[0]; i++) {
        for (int j = 0; j < b->size[1]; i++) {
            for (int p = 0; p < a->size[1]; p++) {

                output->values[i*size[1] + i] += a->values[i*a->size[0] + p] * b->values[p*a->size[1] + j]; // need to double check this ig
            }
        }
    }


    return 1;
}

#endif