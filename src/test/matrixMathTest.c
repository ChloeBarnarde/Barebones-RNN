#include "../matrixMath.h"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
    

int c = 0;
double Count() {
    return c++;
}


void TestDotProd() {
    int testCount = 0;
    int testsPassed = 0;
    printf("=== Dot Product Test ===\n");
    c = 0;
    matrix* m1 = Matrix_Initialize(2, 3, &Count);
    c = 0;
    matrix* m2 = Matrix_Initialize(3, 2, &Count);    

    // test dot product with correct arguements
    testCount++;
    int status = Matrix_DotProd(m1, m2); // should be 2x2 matrix 
    matrix* m3 = Matrix_InitializeVarArg(2, 2, 10, 13, 28, 40);
    if (status == 0 && Matrix_Equals(m1, m3) == 1) {
        testsPassed++;
        printf("Correct Args Test 00:"ANSI_COLOR_GREEN" pass\n"ANSI_COLOR_RESET);
    } else {
        printf("Correct Args Test 00:"ANSI_COLOR_RED" fail\n"ANSI_COLOR_RESET);
    }

    Matrix_Free(m1);
    Matrix_Free(m2);
    Matrix_Free(m3);

    testCount++;
    c = 0;
    m1 = Matrix_Initialize(4, 2, &Count);
    c = 0;
    m2 = Matrix_Initialize(2, 4, &Count);    
    m3 = Matrix_InitializeVarArg(4, 4, 4, 5, 6, 7, 12, 17, 22, 27, 20, 29, 38, 47, 28, 41, 54, 67);
    status = Matrix_DotProd(m1, m2);
    if (status == 0 && Matrix_Equals(m1, m3) == 1) {
        testsPassed++;
        printf("Correct Args Test 01:"ANSI_COLOR_GREEN" pass\n"ANSI_COLOR_RESET);
    } else {
        printf("Correct Args Test 01:"ANSI_COLOR_RED" fail\n"ANSI_COLOR_RESET);
    }
   
    Matrix_Free(m1);
    Matrix_Free(m2);
    Matrix_Free(m3);


    // test dot product with incorrect arguements


    // total tests passed
    printf("Tests Passed: %d/%d\n", testsPassed, testCount);
}

void TestAdd() {

}

void  TestScaler() {

}

void TestSub() {

}

void TestVector() {

}


int main() {
    // matrix* m = Matrix_Initialize(2, 3, &Count);
    // printf("print return: %d\n", Matrix_Print(m));
    // matrix* m2 = Matrix_InitializeVarArg(2, 2, 0, 1, 2, 3);
    // printf("print return: %d\n", Matrix_Print(m2));
    // Matrix_Free(m);
    // Matrix_Free(m2);
    TestDotProd();

    TestAdd();
    TestScaler();
    TestSub();
    TestVector();
}

