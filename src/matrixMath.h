#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H
#include <stdio.h>
#include <stdlib.h>

// currently just a 2d matrix :p

/// @brief represents matrix. It holds pointers to values pretaining to it
typedef struct {
    //int* dimensions; // can be added later ig

    //pointer of size 2, holding x and y sizes
    int* size;
    //pointer of to all values
    double* values;
} matrix;


/* ===Creation===*/

/// @brief Allocates memory for matrix of given size and initalizes values to 0. 
/// @param colSize amount of collums
/// @param rowSize amount of rows
/// @return pointer to matrix location in memory
matrix* Matrix_Create(int colSize, int rowSize);

/// @brief Allocates memory for matrix of given size and initalizes values with passed function
/// @param colSize amount of collumns
/// @param rowSize amount of rows
/// @param fprt initalization value function
/// @return pointer to matrix location in memory
matrix* Matrix_Initialize(int colSize, int rowSize, float (*fprt)());

/// @brief frees all the memory related to the passed matrix
/// @param m matrix to free memroy of
void Matrix_Free(matrix* m);

// this needs to be heavily modified
int InitializeValues(matrix* m, float sizeGoal[2]);


/* ===Matrix Math=== */

/// @brief Dot Product of a and b. Mutates a. Doesn't mutate b.
/// @param a matrix* a
/// @param b matrix* b
/// @return Exit status
int Matrix_DotProd(matrix* a, matrix* b, matrix* output);



/// @brief Multiplies all elements in a with b. Mutates a.
/// @param a Matrix pointer
/// @param b Scaler to multiply with
/// @return Exit status (0 = success, 1= faliure)
int Matrix_Scaler(matrix* a, float b);

/// @brief Foreach element in a, the function pointer is applied. Mutates a
/// @param a Initial matrix 
/// @param fptr Function to Apply
/// @return Exit status
int Matrix_ElemetWiseFunc1M(matrix* a, float (*fptr)(float));

/// @brief Foreach element in a, a(ij), the function pointer is 
/// applied with a(ij) and b(ij). Mutates a. Doesn't mutate b.
/// @param a matrix of A 
/// @param b matrix of B
/// @param fptrs Function to apply
/// @return Exit status
int Matrix_ElementWiseFunc2M(matrix* a, matrix* b, float (*fptrs)(float, float));


int Matrix_Add(matrix* a, matrix* b);



int Matrix_Sub(matrix* a, matrix* b, matrix* output);


/* ===Transformation=== */

int Matrix_Transpose(matrix* a);

/* ===Misc=== */

/// @brief sets values from one to the other.
/// @param from matrix to copy from (stays unmodified)
/// @param to matrix to copy to (is modified)
/// @return Exit Status
int Matrix_Copy(matrix* from, matrix* to);

int MVec(matrix* a, int col, matrix* output);


/* ===Printing=== */

int Matrix_Print(matrix* m);


/* ===Get/Set=== */
double Matrix_Get(matrix* m, int col, int row);
double Matrix_Set(matrix* m, int col, int row, double value);
#endif