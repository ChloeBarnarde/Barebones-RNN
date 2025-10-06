#include "../matrixMath.h"

int c = 0;
double Count() {
    return c++;
}


int TestDotProd() {
    printf("=== Dot Product Test ===\n");
    c = 0;
    matrix* m1 = Matrix_Initialize(2, 3, &Count);
    c = 0;
    matrix* m2 = Matrix_Initialize(3, 2, &Count);    
    int status = Matrix_DotProd(m1, m2); // should be 2x2 matrix
    matrix* m3 = Matrix_InitializeVarArg(2, 2, 10, 13, 28, 40);
    printf("Correct Args result: %s\n", (status == 0 && Matrix_Equals(m1, m3) == 1) ? "pass" : "fail");

    Matrix_Free(m1);
    Matrix_Free(m2);
    Matrix_Free(m3);


    // return 1;
}

int TestAdd() {

}

int TestScaler() {

}

int TestSub() {

}

int TestVector() {

}


int main() {
    // matrix* m = Matrix_Initialize(2, 3, &Count);
    // printf("print return: %d\n", Matrix_Print(m));
    // matrix* m2 = Matrix_InitializeVarArg(2, 2, 0, 1, 2, 3);
    // printf("print return: %d\n", Matrix_Print(m2));
    // Matrix_Free(m);
    // Matrix_Free(m2);
    TestDotProd();
}