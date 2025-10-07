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
bool CheckBounds(matrix* m, int row, int col) {
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

matrix* Matrix_Create(int rows, int cols) {
   return Matrix_Initialize(rows, cols, &zero);
}

matrix* Matrix_Initialize(int rows, int cols, double (*fprt)()) {
    matrix* m = (matrix*)malloc(sizeof(matrix));
    // allocate memory
    m->size = malloc(sizeof(int)*2);
    m->values = malloc(sizeof(double)*rows*cols);
    // set values
    m->size[0] = rows;
    m->size[1] = cols;

    for (int y = 0; y < rows; y++) {
        for(int x = 0; x < cols; x++) {
           m->values[y*cols+x] = fprt(); 
        }
    }

    return m;
}

matrix* Matrix_InitializeVarArg(int rows, int cols, ...) {
    va_list args;
    va_start(args, cols);  
    
    matrix* m = (matrix*)malloc(sizeof(matrix));
    // allocate memory
    m->size = malloc(sizeof(int)*2);
    m->values = malloc(sizeof(double)*rows*cols);
    // set values
    m->size[0] = rows;
    m->size[1] = cols;

    for (int rowi = 0; rowi < rows; rowi++)
    {
        for (int coli = 0; coli < cols; coli++)
        {
            m->values[rowi*cols+coli] = va_arg(args, int); 
        }
    }
    va_end(args);

    return m;
}


void Matrix_Free(matrix* m) {
    free(m->values);
    free(m->size);
    free(m);
}


int Matrix_DotProd(matrix* a, matrix* b) {
    if (!CheckNull(a) || !CheckNull(b))
        return EXIT_FAILURE;

    if (a->size[1] != b->size[0]) {
        printf("Passed Matrices don't have sizes that can be multiplied together a: %d, %d\n", a->size[0], a->size[1]);
        return EXIT_FAILURE;
    }

    matrix* temp = Matrix_Create(a->size[0], a->size[1]);
    Matrix_Copy(a, temp);
    
    //resize/reset values of a 
    if (a->size[1] != b->size[1]) {
        free(a->values);
        a->values = malloc(a->size[0] * b->size[1] * sizeof(double));
        for (int rowi = 0; rowi < a->size[0]; rowi++)
        {
            for (int coli = 0; coli < b->size[1]; coli++)
            {
                a->values[rowi*b->size[1]+coli] = 0; 
            }
        }
    }
    a->size[1] = b->size[1];
    
    // just gonna use naive implementation for now
    for (int i = 0; i < temp->size[0]; i++) {
        for (int j = 0; j < b->size[1]; j++) {
            a->values[CrToIdx(i, j, a)] = 0;
            for (int p = 0; p < b->size[0]; p++) {
                a->values[CrToIdx(i, j, a)] += (temp->values[CrToIdx(i, p, temp)] * b->values[CrToIdx(p, j, b)]); // need to double check this ig
            }
        }
    }

    Matrix_Free(temp);
    return EXIT_SUCCESS;
}



int Matrix_Scaler(matrix* a, double b) {

    if (!CheckNull(a))
        return EXIT_FAILURE;
    
    for (int rowi = 0; rowi < a->size[0]; rowi++)
    {
        for (int coli = 0; coli < a->size[1]; coli++) {
            a->values[rowi * a->size[1] + coli] = a->values[rowi * a->size[1] + coli] * b;
        }
    }
 
    return EXIT_SUCCESS;
}



int Matrix_ElemetWiseFunc1M(matrix* a, double (*fptr)(double)) {
    if (!CheckNull(a))
        return EXIT_FAILURE;

    for (int rowi = 0; rowi < a->size[0]; rowi++)
    {
        for (int coli = 0; coli < a->size[1]; coli++) {
            a->values[rowi * a->size[1] + coli] = fptr(a->values[rowi * a->size[1] + coli]);
        }
    }
    
    return EXIT_SUCCESS;
}



int Matrix_ElementWiseFunc2M(matrix* a, matrix* b, double (*fptrs)(double, double)) {
    if (!CheckNull(a) || !CheckNull(b))
        return EXIT_FAILURE;
    if (a->size[0] != b->size[0] || a->size[1] != b->size[1])
        return EXIT_FAILURE;
    
    for (int rowi = 0; rowi < a->size[0]; rowi++)
    {
        for (int coli = 0; coli < a->size[1]; coli++) {
            a->values[rowi * a->size[1] + coli] = fptrs(a->values[rowi * a->size[1] + coli],
                b->values[rowi * b->size[1] + coli]);
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
    for (int rowi = 0; rowi < a->size[0]; rowi++)
    {
        for (int coli = 0; coli < a->size[1]; coli++) {
            a->values[rowi * a->size[1] + coli] = temp->values[coli * temp->size[1] + rowi];
        }
    }

    return EXIT_SUCCESS;
}


/* ===Misc=== */

int Matrix_Copy(matrix* from, matrix* to) {
    if (!CheckNull(from) || !CheckNull(to))
        return EXIT_FAILURE;
    
    if (to->size[0] != from->size[0] || to->size[1] != from->size[1]) {
        // need to free values and reassign to correct size;
        free(to->values);
        
        to->size[0] = from->size[0];
        to->size[1] = from->size[1];

        to->values = malloc(sizeof(double) * from->size[0] * from->size[1]);
    }

    for (int rowi = 0; rowi < from->size[0]; rowi++)
    {
        for (int coli = 0; coli < from->size[1]; coli++) {
            to->values[rowi * from->size[1] + coli] = from->values[rowi * from->size[1] + coli];
        }
    }
    return EXIT_SUCCESS;
}


matrix* Matrix_VectorRow(matrix* m, int row) {
    if (!CheckNull(m))
        return NULL;
    
    if (m->size[0] >= row)
        return NULL;
    
    matrix* new = Matrix_Create(1, m->size[1]);

    for (int i = 0; i < m->size[1]; i++)
    {
        new->values[i] = m->values[row * m->size[1] + i];
    }
    
    return new;
}

matrix* Matrix_VectorCol(matrix* m, int col) {
    if (!CheckNull(m))
        return NULL;
    
    if (m->size[1] >= col)
        return NULL;
    
    matrix* new = Matrix_Create(m->size[0], 1);

    for (int i = 0; i < m->size[0]; i++)
    {
        new->values[i] = m->values[i * m->size[1]];
    }
    
    return new;
}

int Matrix_Equals(matrix* a, matrix* b) {
    if (!CheckNull(a) || !CheckNull(b))
        return false;
    
    if (a->size[0] != b->size[0] || a->size[1] != b->size[1])
        return false;
    
    for (int rowi = 0; rowi < a->size[0]; rowi++)
    {
        for (int coli = 0; coli < a->size[1]; coli++)
        {
            if (a->values[rowi * a->size[1] + coli] != b->values[rowi * b->size[1] + coli])
                return false;
        }
    }
    
    return true;
}

/* ===Printing=== */

int Matrix_Printf(matrix *m, int decimalPlaces) {

   if (!CheckNull(m))
       return EXIT_FAILURE;

    double maxValue = 0;
    for (int rowi = 0; rowi < m->size[0]; rowi++)
    {
        for (int coli = 0; coli < m->size[1]; coli++) {
            if (maxValue < m->values[rowi * m->size[1] + coli])
                maxValue = m->values[rowi * m->size[1] + coli];
        }
    }
    
    int cellWidth = (ceil(log10(maxValue))) + 2 + decimalPlaces;    

    for (int rowi = 0; rowi < m->size[0]; rowi++) {    
        for (int coli = 0; coli < m->size[1]; coli++) {
            printf("%*.*f", cellWidth, decimalPlaces, m->values[rowi * m->size[1] + coli]);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}

int Matrix_Print(matrix* m) {
    return Matrix_Printf(m, 2);
}



/* ===Get/Set=== */

double Matrix_Get(matrix *m, int row, int col) {
    if (!CheckBounds(m, row, col)) {
        printf("col and row out of bounds of matrix size");
        return NAN;
    }

    return m->values[row * m->size[1] + col];
}



int Matrix_Set(matrix* m, int row, int col, double value) {
   if (!CheckBounds(m, row, col))
       return EXIT_FAILURE;

   m->values[row * m->size[1] + col] = value;

   return EXIT_SUCCESS;
}
