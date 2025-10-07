#include "../matrixMath.h"
#include <stdlib.h>
#include <stdio.h>
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
    

int c = 0;
double Count() {
    return c++;
}

int Output(int pass, char* test, int num) {
    if (pass == 1) {
       printf("%s Test %d: " ANSI_COLOR_GREEN" pass\n"ANSI_COLOR_RESET, test, num);
       return 1;
    } else {
       printf("%s Test %d: " ANSI_COLOR_RED" fail\n"ANSI_COLOR_RESET, test, num);
       return 0;
    }
}
int Equal(matrix* m1, matrix* m2, char* test, int num) { 
    int out = Matrix_Equals(m1, m2);
    Output(out, test, num);
    return out;
}
int DPH(matrix* m1,matrix* m2 ,matrix* m3, int i) {
   
    Matrix_DotProd(m1, m2);
    int status = Equal(m1, m3, "Correct Args",i); 
    Matrix_Free(m1);
    Matrix_Free(m2);
    Matrix_Free(m3);
    return status;
}
int AH(matrix* m1, matrix* m2, matrix* m3, int i) {
    Matrix_Add(m1, m2);
    int status = Equal(m1, m3, "Correct Args", i);
    Matrix_Free(m1);
    Matrix_Free(m2);
    Matrix_Free(m3);
    return status;
}
void TestDotProd() {
    int testCount = 0;
    int testsPassed = 0;
    printf("\n=== Dot Product Test ===\n");

    // test dot product with correct arguements
    testCount++;
    c = 0;
    matrix* m1 = Matrix_Initialize(2, 3, &Count);
    c = 0;
    matrix* m2 = Matrix_Initialize(3, 2, &Count);    
    matrix* m3 = Matrix_InitializeVarArg(2, 2, 10, 13, 28, 40);
    testsPassed += DPH(m1, m2, m3, testCount);  
    
    testCount++;
    c = 0;
    m1 = Matrix_Initialize(4, 2, &Count);
    c = 0;
    m2 = Matrix_Initialize(2, 4, &Count);    
    m3 = Matrix_InitializeVarArg(4, 4, 4, 5, 6, 7, 12, 17, 22, 27, 20, 29, 38, 47, 28, 41, 54, 67);
    testsPassed += DPH(m1, m2, m3, testCount); 

    testCount++;
    c = 0;
    m1 = Matrix_Initialize(3, 3, &Count);
    c = 0;
    m2 = Matrix_Initialize(3, 4, &Count);    
    m3 = Matrix_InitializeVarArg(3, 4, 20, 23, 26, 29, 56, 68, 80, 92, 92, 113, 134, 155);
    testsPassed += DPH(m1, m2, m3, testCount);

    // test dot product with incorrect arguements
    testCount++;
    int out = Matrix_DotProd(NULL, NULL);
    testsPassed += Output(out == EXIT_FAILURE, "Null Test", 1);
    
    testCount++;
    m1 = malloc(sizeof(matrix));
    out = Matrix_DotProd(m1, NULL);
    testsPassed += Output(out == EXIT_FAILURE, "Null Test", 2);
     
    testCount++;
    out = Matrix_DotProd(NULL, m1);
    testsPassed += Output(out == EXIT_FAILURE, "Null Test", 3);

    testCount++;
    m2 = malloc(sizeof(matrix));
    out = Matrix_DotProd(m1, m2);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 4);

    testCount++;
    free(m1);
    m1 = Matrix_Create(2, 3);
    free(m2);
    m2 = Matrix_Create(4, 5);
    out = Matrix_DotProd(m1, m2); 
    testsPassed += Output(out == EXIT_FAILURE, "Incorrect Arg", 1);

    // total tests passed
    printf("Tests Passed: %d/%d\n", testsPassed, testCount);
}

void TestAdd() {
    printf("\n=== Addition Test ===\n");
    int testCount = 0;
    int testsPassed = 0;

    testCount++;
    c = 0;
    matrix* m1 = Matrix_Initialize(2, 2, &Count);
    c = 0;
    matrix* m2 = Matrix_Initialize(2, 2, &Count);
    matrix* m3 = Matrix_InitializeVarArg(2, 2, 0, 2, 4, 6);
    testsPassed += AH(m1, m2, m3, 1);


    printf("Tests Passed: %d/%d\n", testsPassed, testCount);
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

