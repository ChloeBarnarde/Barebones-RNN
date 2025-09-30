#include "matrixMath.h"

matrix* Matrix_Create(int sizeX, int sizeY) {

}

matrix* Matrix_Initialize(int sizeX, int sizeY, float (*fprt)()) {

}


// this needs to be heavily modified
int InitializeValues(matrix* m, float sizeGoal[2]) {
    if (m->values == NULL) {
        m->values = (float *)malloc(sizeGoal[0] * sizeof(float) * sizeGoal[1] * sizeof(float));
        return EXIT_SUCCESS;
    }

    if (m->size == NULL) {
        // don't know size sooo??? do above
        m->size[0] = sizeGoal[0];
        m->size[1] = sizeGoal[1];
        free(m->values);
        m->values = (float *)malloc(sizeGoal[0] * sizeof(float) * sizeGoal[1] * sizeof(float));
        return EXIT_SUCCESS;
    }


    if (m->size[0] == sizeGoal[0] && m->size[1] == sizeGoal[2]) {
        return EXIT_SUCCESS;
    }

    m->size[0] = sizeGoal[0];
    m->size[1] = sizeGoal[1];
    free(m->values);
    m->values = (float *)malloc(sizeGoal[0] * sizeof(float) * sizeGoal[1] * sizeof(float));

}

int Matrix_DotProd(matrix* a, matrix* b, matrix* output) {
    if (a->size[1] != b->size[0]) {
        printf("Passed Matrices don't have sizes that can be multiplied together a: %d, %d\n", a->size[0], a->size[1]);
        return EXIT_FAILURE;
    }
    
    int size[2] = {a->size[0], b->size[1]}; // might need to rewrite this

    InitializeValues(output, size);
    
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


/* ===Helper Methods=== */

int CrToIdx(int col, int row, matrix* m) {
    return col * m->size[1] + row; 
}