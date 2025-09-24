#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H
#include <stdio.h>
#include <stdlib.h>

// currently just a 2d matrix :p
struct {
    //int* dimensions; // can be added later ig

    int* size;
    float* values;
} typedef matrix;


int CrToIdx(int col, int row, matrix* m) {
    return col * m->size[1] + row; 
}

int MDotProd(matrix* a, matrix* b, matrix* output) {
    // need to make sure a->size[1] == b->size[0]
    if (a->size[1] != b->size[0]) {
        printf("Passed Matrices don't have sizes that can be multiplied together a: %d, %d\n", a->size[0], a->size[1]);
        return EXIT_FAILURE;
    }
    
    int size[2] = {a->size[0], b->size[1]}; // might need to rewrite this
    float* values = (float *)malloc(size[0] * sizeof(float) * size[1] * sizeof(float));
    
    
    output->size = &size[0];
    output->values = values;
    
    // just gonna use naive implementation for now
    for (int i = 0; i < a->size[0]; i++) {
        for (int j = 0; j < b->size[1]; j++) {
            for (int p = 0; p < a->size[1]; p++) {
                output->values[CrToIdx(i, j, output)] += a->values[CrToIdx(i, p, a)] * b->values[CrToIdx(p, j, b)]; // need to double check this ig
            }
        }
    }
    return EXIT_SUCCESS;
}

int MScaler(matrix* a, float b, matrix* output) {
    int size[2] = {a->size[0], a->size[1]};
    output->size = &size[0];

    for (int i = 0; i < a->size[0]; i++) {
        for (int j = 0; j < a->size[1]; j++)
        {
            output->values[CrToIdx(i, j, output)] = output->values[CrToIdx(i, j, output)] * b;
        }
    }
    return EXIT_SUCCESS;
}

int MAdd(matrix* a, matrix* b, matrix* output) {
    if (a->size[0] != b->size[0] && a->size[1] != b->size[1]) {
        printf("Passed matrices didn't have matching sizes! \nA size: %d, %d\nB size: %d, %d", 
            a->size[0], a->size[1], b->size[0], b->size[1]);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < a->size[0]; i++) {
        for (int j = 0; j < a->size[1]; j++)
        {
            output->values[CrToIdx(i, j, output)] = a->values[CrToIdx(i, j, a)] + b->values[CrToIdx(i, j, b)];
        }
    }

    return EXIT_SUCCESS;
}

// wanna rewrite this so that its one function, cause atm its going through the matrix twice ;-;
int MSub(matrix* a, matrix* b, matrix* output) {
    matrix* c = malloc(sizeof(matrix));
    MScaler(b, -1, c);
    MAdd(a, c, output);

    return EXIT_SUCCESS;
}


int MVec(matrix* a, int col, matrix* output) {
    if (col >= a->size[0]) {
        // error
        printf("Passed collumn (%d) is greater than matrix columns (%d)", col, a->size[0]);
        return EXIT_FAILURE;
    }

    output->size[0] = 1;
    output->size[1] = a->size[1]; 

    return EXIT_SUCCESS;
}

#endif