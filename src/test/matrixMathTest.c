#include "../matrixMath.h"


int TestDotProd() {
    // matrix* m1 = Matrix_Initialize(2, 3, &Count);
    // matrix* m2 = Matrix_Initialize(3, 2, &Count);
    
    // int status = Matrix_DotProd(m1, m2); // should be 2x2 matrix

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


int c = 0;
double Count() {
    return c++;
}
int main() {
    matrix* m = Matrix_Initialize(2, 3, &Count);
    printf("print return: %d\n", Matrix_Print(m));
    matrix* m2 = Matrix_InitializeVarArg(2, 2, 0, 1, 2, 3);
    printf("print return: %d\n", Matrix_Print(m2));
    Matrix_Free(m);
    Matrix_Free(m2);
}