#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// currently just a 2d matrix :p

/// @brief represents matrix. It holds pointers to values pretaining to it
typedef struct {
    //int* dimensions; // can be added later ig

    //pointer of size 2, holding amount of rows and cols respectivaley
    int* size;
    //pointer of to all values
    double* values;
} matrix;


/* ===Creation===*/

/// @brief Allocates memory for matrix of given size and initalizes values to 0. 
/// @param rows amount of rows
/// @param cols amount of collums
/// @return pointer to matrix location in memory
matrix* Matrix_Create(int rows, int cols);

/// @brief Allocates memory for matrix of given size and initalizes values with passed function
/// @param rows amount of rows
/// @param cols amount of columns
/// @param fprt initalization value function
/// @return pointer to matrix location in memory
matrix* Matrix_Initialize(int rows, int cols, double (*fprt)());


/// @brief Create and Initalize a matrix with passed values
/// @param cols amount of columns
/// @param rows amount of columns
/// @param ... values to initalize matrix with
/// @return new matrix pointer
matrix* Matrix_InitializeVarArg(int rows, int cols, ...);

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

////// Might make a seperate thing for vectors??? //////

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

/// @brief get a sub matrix. The matrix values will have their own space in memory
/// @param m matrix to referece and get the sub matrix FROM
/// @param rowStart the row to start, inclusive
/// @param rowEnd the row to end, exclusive
/// @param colStart the col to start, inclusive
/// @param colEnd the col to end, exclusive
/// @return returns a new matrix with the values from the specified chunk of the orignal, if error, it will return a null pointer
matrix* Matrix_SubMatrix(matrix* m, int rowStart, int rowEnd, int colStart, int colEnd);

/// @brief compare matrix values to see if they're equal
/// @param a first matrix
/// @param b second matrix
/// @return 0 if not equal, 1 if equal
int Matrix_Equals(matrix* a, matrix* b);



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
/// @param row row index
/// @param col collumn index
/// @return value at position, returns NAN if an issue
double Matrix_Get(matrix* m, int row, int col);

/// @brief sets value at given col, row. Mutates matrix.
/// @param matrix to update
/// @param row row index
/// @param col collumn index
/// @return exit status
int Matrix_Set(matrix* m, int row, int col, double value);



#endif
