#include "matrixMath.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>




/* ===Helper Methods=== */

int CrToIdx(int col, int row, matrix* m) {
    return col * m->size[0] + row; 
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

/// @brief makes sure pointers of matrix have been allocated
/// @param m matrix to check
/// @return true if all checks pass. False otherwise.
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
double Sub(double a, double b) {
    return a - b;
}
double Add(double a, double b) {
    return a + b;
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
           m->values[y*rowSize+x] = fprt(); 
        }
    }

    return m;
}


void Matrix_Free(matrix* m) {
    free(m->values);
    free(m->size);
    free(m);
}


int Matrix_DotProd(matrix* a, matrix* b) {
    if (a->size[1] != b->size[0]) {
        printf("Passed Matrices don't have sizes that can be multiplied together a: %d, %d\n", a->size[0], a->size[1]);
        return EXIT_FAILURE;
    }

    matrix* temp = Matrix_Create(a->size[0], a->size[1]);
    Matrix_Copy(a, temp);
    a->size[1] = b->size[1];

    // just gonna use naive implementation for now
    for (int i = 0; i < temp->size[0]; i++) {
        for (int j = 0; j < b->size[1]; j++) {
            for (int p = 0; p < temp->size[1]; p++) {
                a->values[CrToIdx(i, j, a)] += temp->values[CrToIdx(i, p, temp)] * b->values[CrToIdx(p, j, b)]; // need to double check this ig
            }
        }
    }

    Matrix_Free(temp);
    return EXIT_SUCCESS;
}



int Matrix_Scaler(matrix* a, double b) {

    if (!CheckNull(a))
        return EXIT_FAILURE;
    
    for (int col = 0; col < a->size[1]; col++)
    {
        for (int row = 0; row < a->size[0]; row++) {
            a->values[col * a->size[0] + row] = a->values[col * a->size[0] + row] * b;
        }
    }
 
    return EXIT_SUCCESS;
}



int Matrix_ElemetWiseFunc1M(matrix* a, double (*fptr)(double)) {
    if (!CheckNull(a))
        return EXIT_FAILURE;
    
    for (int col = 0; col < a->size[1]; col++)
    {
        for (int row = 0; row < a->size[0]; row++) {
            a->values[col * a->size[0] + row] =  fptr(a->values[col * a->size[0] + row]);
        }
    }

    return EXIT_SUCCESS;
}



int Matrix_ElementWiseFunc2M(matrix* a, matrix* b, double (*fptrs)(double, double)) {
    if (!CheckNull(a) || !CheckNull(b))
        return EXIT_FAILURE;
    if (a->size[0] != b->size[0] || a->size[1] != b->size[1])
        return EXIT_FAILURE;
    
    for (int col = 0; col < a->size[1]; col++)
    {
        for (int row = 0; row < a->size[0]; row++) {
            a->values[col * a->size[0] + row] = fptrs(a->values[col * a->size[0] + row], 
                b->values[col * b->size[0] + row]);
        }
    }

    return EXIT_SUCCESS;
}


int Matrix_Add(matrix* a, matrix* b) {
    return Matrix_ElementWiseFunc2M(a, b, &Add);
}



int Matrix_Sub(matrix* a, matrix* b) {
    return Matrix_ElementWiseFunc2M(a, b, &Sub);
}



/* ===Transformation=== */

int Matrix_Transpose(matrix* a) {
    if (!CheckNull(a))
        return EXIT_FAILURE;
    
    //switch size
    matrix* temp = Matrix_Create(a->size[0], a->size[1]); 
    Matrix_Copy(a, temp);
    int tempSize = a->size[0];
    a->size[0] = a->size[1];
    a->size[1] = tempSize;

    // switch values???
    for (int col = 0; col < temp->size[1]; col++)
    {
        for (int row = 0; row < temp->size[0]; row++)
        {
            a->values[col * a->size[0] + row] = temp->values[row * temp->size[0] + col];
        }
        
    }

    return EXIT_SUCCESS;
    
}


/* ===Misc=== */

int Matrix_Copy(matrix* from, matrix* to) {
    if (!CheckNull(from) || CheckNull(to))
        return EXIT_FAILURE;
    
    if (to->size[0] != from->size[0] || to->size[1] != from->size[1]) {
        // need to free values and reassign to correct size;
        free(to->values);
        
        to->size[0] = from->size[0];
        to->size[1] = from->size[1];

        to->values = malloc(sizeof(double) * from->size[0] * from->size[1]);
    }

    for (int col = 0; col < from->size[1]; col++)
    {
        for (int row = 0; row < from->size[0]; row++)
        {
            to->values[col * from->size[0] + row] = from->values[col * from->size[0] + row];
        }
    }
    
    return EXIT_SUCCESS;
}


matrix* Matrix_VectorCol(matrix* m, int col) {
    if (!CheckNull(m))
        return NULL;
    
    if (m->size[1] >= col)
        return NULL;
    
    matrix* new = Matrix_Create(col, m->size[0]);

    for (int i = 0; i < m->size[0]; i++)
    {
        new->values[i] = m->values[col * m->size[0] + i];
    }
    
    return new;
}

matrix* Matrix_VectorRow(matrix* m, int row) {
    if (!CheckNull(m))
        return NULL;
    
    if (m->size[0] >= row)
        return NULL;
    
    matrix* new = Matrix_Create(m->size[1], row);

    for (int i = 0; i < m->size[1]; i++)
    {
        new->values[i] = m->values[i * m->size[0]];
    }
    
    return new;
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

    int cellWidth = (round(log10(maxValue))) + 2 + decimalPlaces;

    for (int row = 0; row < m->size[0]; row++) {
        for (int col = 0; col < m->size[1]; col++) {
            printf("%-*.*f", cellWidth, decimalPlaces, m->values[col * m->size[1] + row]);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}

int Matrix_Print(matrix* m) {
    return Matrix_Printf(m, 2);
}



/* ===Get/Set=== */

double Matrix_Get(matrix *m, int col, int row) {
    if (!CheckBounds(m, col, row)) {
        printf("col and row out of bounds of matrix size");
        return 1;
    }

    return m->values[col * m->size[0] + row];
}



int Matrix_Set(matrix* m, int col, int row, double value) {
   if (!CheckBounds(m, col, row))
       return EXIT_FAILURE;

   m->values[col * m->size[0] + row] = value;

   return EXIT_SUCCESS;
}