#include "matrixMath.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>




/* ===Helper Methods=== */

int CrToIdx(int col, int row, matrix* m) {
    return col * m->size[1] + row; 
}

/// @brief checks bounds
/// @param matrix to check
/// @param collum
/// @param row
/// @return true if in bounds, false otherwise.
bool CheckBounds(matrix* m, int col, int row) {
    if (col < 0 || row < 0)
        return false;

    if (col >= m->size[1] || row >= m->size[0]) 
        return false;

    return true;
}

bool CheckNull(matrix* m) {
    if (m==NULL)
        return false;
    if (m->values == NULL)
        return false;
    if (m->size == NULL)
        return false;
    return true;
}

double zero() {
    return 0;
}



/* ===Creation=== */

matrix* Matrix_Create(int colSize, int rowSize) {
   return Matrix_Initialize(colSize, rowSize, &zero);
}

matrix* Matrix_Initialize(int colSize, int rowSize, double (*fprt)()) {
    matrix* m = (matrix*)malloc(sizeof(matrix));
    // allocate memory
    m->size = malloc(sizeof(int)*2);
    m->values = malloc(sizeof(double)*colSize*rowSize);
    // set values
    m->size[0] = rowSize;
    m->size[1] = colSize;

    for (int x = 0; x < rowSize; x++) {
        for(int y = 0; y < colSize; y++) {
           m->values[y*colSize+x] = fprt(); 
        }
    }

    return m;
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


/* ===Printing=== */

int Matrix_Printf(matrix *m, int decimalPlaces) {

   if (CheckNull(m))
       return EXIT_FAILURE;

    double maxValue = 0;
    for (int row = 0; row < m->size[0]; row++) {
        for (int col = 0; col < m->size[1]; col++) {
            if (maxValue < m->values[col * m->size[1] + row]) 
                maxValue = m->values[col * m->size[1] + row];

        }
    }

    int cellWidth = log10(maxValue) + 2 + decimalPlaces;

    for (int row = 0; row < m->size[0]; row++) {
        for (int col = 0; col < m->size[1]; col++) {
            printf("%-*.*f", cellWidth, decimalPlaces, m->values[col * m->size[1] + row]);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}

/* ===Get/Set=== */


double Matrix_Get(matrix *m, int col, int row) {
    if (!CheckBounds(m, col, row)) {
        printf("col and row out of bounds of matrix size");
        return 1;
    }

    return m->values[col * m->size[1] + row];
}


int Matrix_Set(matrix* m, int col, int row, double value) {
   if (!CheckBounds(m, col, row))
       return EXIT_FAILURE;

   m->values[col * m->size[1] + row] = value;

   return EXIT_SUCCESS;
}



