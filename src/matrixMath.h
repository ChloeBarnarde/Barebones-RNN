#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H
#include <stdio.h>
#include <stdlib.h>

// currently just a 2d matrix :p

/// @brief represents matrix. It holds pointers to values pretaining to it
typedef struct {
    //int* dimensions; // can be added later ig

    //pointer of size 2, holding row and col sizes respectivaley
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
matrix* Matrix_Initialize(int colSize, int rowSize, double (*fprt)());

/// @brief frees all the memory related to the passed matrix
/// @param m matrix to free memroy of
void Matrix_Free(matrix* m);


/* ===Matrix Math=== */

/// @brief Dot Product of a and b. Mutates a. Doesn't mutate b.
/// @param a matrix* a
/// @param b matrix* b
/// @return Exit status
int Matrix_DotProd(matrix* a, matrix* b);

/// @brief Multiplies all elements in a with b. Mutates a.
/// @param a Matrix pointer
/// @param b Scaler to multiply with
/// @return Exit status (0 = success, 1= faliure)
int Matrix_Scaler(matrix* a, double b);

/// @brief Foreach element in a, the function pointer is applied. Mutates a
/// @param a Initial matrix 
/// @param fptr Function to Apply
/// @return Exit status
int Matrix_ElemetWiseFunc1M(matrix* a, double (*fptr)(double));

/// @brief Foreach element in a, a(ij), the function pointer is 
/// applied with a(ij) and b(ij). Mutates a. Doesn't mutate b.
/// @param a matrix of A 
/// @param b matrix of B
/// @param fptrs Function to apply
/// @return Exit status
int Matrix_ElementWiseFunc2M(matrix* a, matrix* b, double (*fptrs)(double, double));

/// @brief Adds the second matrix to the first. Mutates a. Doesn't mutate b
/// @param a matrix to add to
/// @param b matrix to add from
/// @return exit status
int Matrix_Add(matrix* a, matrix* b);

/// @brief Subtracts the second matrix from the first. Mutates a. Doesn't mutate b
/// @param a matrix to subtract from
/// @param b matrix to subtract
/// @return exit status
int Matrix_Sub(matrix* a, matrix* b);


/* ===Transformation=== */

/// @brief switches rows and columns
/// @param a matrix to transpose. Mutates its values
/// @return exit status
int Matrix_Transpose(matrix* a);


/* ===Misc=== */

/// @brief sets values from one to the other.
/// @param from matrix to copy from (stays unmodified)
/// @param to matrix to copy to (is modified)
/// @return Exit Status
int Matrix_Copy(matrix* from, matrix* to);

/// @brief get sepcified column from passed matrix
/// @param m matrix to get column from. Isn't modified
/// @param col column number to get 
/// @return new matrix. Returns null if error encountered.
matrix* Matrix_VectorCol(matrix* m, int col);

/// @brief get sepcified row from passed matrix
/// @param m matrix to get row from. Isn't modified
/// @param row row number to get 
/// @return new matrix. Returns null if error encountered.
matrix* Matrix_VectorRow(matrix* m, int row);

/* ===Printing=== */


/// @brief prints matrix to stdout, with decimal precision of 2
/// @param m matrix to print
/// @return exit status
int Matrix_Print(matrix* m);


/// @brief prints matrix to stdout
/// @param m matrix to print
/// @param decimalPlaces decimal precision
/// @return exit status
int Matrix_Printf(matrix *m, int decimalPlaces);

/* ===Get/Set=== */

/// @brief gets value at passed col row.
/// @param m matrix to read from
/// @param col collumn
/// @param row row
/// @return value at position
double Matrix_Get(matrix* m, int col, int row);

/// @brief sets value at given col, row. Mutates matrix.
/// @param matrix to update
/// @param collum
/// @param row
/// @return exit status
int Matrix_Set(matrix* m, int col, int row, double value);



#endif
