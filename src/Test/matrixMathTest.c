#include "matrixMath.h"
#include <stdlib.h>
#include <stdio.h>
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"


/* Helper Methods */

int c = 0;
double Count() {
    return c++;
}
double Eight() {
    return 8.0;
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
    int out = matrix_is_equal(m1, m2);
    Output(out, test, num);
    return out;
}
int DPH(matrix* m1,matrix* m2 ,matrix* m3, int i) {
   
    matrix_dot_prod(m1, m2);
    int status = Equal(m1, m3, "Correct Args",i); 
    matrix_free(m1);
    matrix_free(m2);
    matrix_free(m3);
    return status;
}
int AH(matrix* m1, matrix* m2, matrix* m3, int i) {
    matrix_add(m1, m2);
    int status = Equal(m1, m3, "Correct Args", i);
    matrix_free(m1);
    matrix_free(m2);
    matrix_free(m3);
    return status;
}
int SubH(matrix* m1, matrix* m2, matrix* m3, int i) {
    matrix_sub(m1, m2);
    int status = Equal(m1, m3, "Correct Args", i);
    matrix_free(m1);
    matrix_free(m2);
    matrix_free(m3);
    return status;
}
int SH(matrix* m1, double a, matrix* m3, int i) {
    matrix_scaler(m1, a);
    int status = Equal(m1, m3, "Correct Args", i);
    matrix_free(m1);
    matrix_free(m3);
    return status;
}
/* Tests for respective functions/funtionality*/

void TestDotProd() {
    int testCount = 0;
    int testsPassed = 0;
    printf("\n=== Dot Product Test ===\n");

    // test dot product with correct arguements
    testCount++;
    c = 0;
    matrix* m1 = matrix_initialize(2, 3, &Count);
    c = 0;
    matrix* m2 = matrix_initialize(3, 2, &Count);    
    matrix* m3 = matrix_initialize_vararg(2, 2, 10., 13., 28., 40.);
    testsPassed += DPH(m1, m2, m3, testCount);  
    
    testCount++;
    c = 0;
    m1 = matrix_initialize(4, 2, &Count);
    c = 0;
    m2 = matrix_initialize(2, 4, &Count);    
    m3 = matrix_initialize_vararg(4, 4, 4., 5., 6., 7., 12., 17., 
        22., 27., 20., 29., 38., 47., 28., 41., 54., 67.);
    testsPassed += DPH(m1, m2, m3, testCount); 

    testCount++;
    c = 0;
    m1 = matrix_initialize(3, 3, &Count);
    c = 0;
    m2 = matrix_initialize(3, 4, &Count);    
    m3 = matrix_initialize_vararg(3, 4, 20., 23., 26., 29., 
        56., 68., 80., 92., 92., 113., 134., 155.);
    testsPassed += DPH(m1, m2, m3, testCount);

    // test dot product with incorrect arguements
    testCount++;
    int out = matrix_dot_prod(NULL, NULL);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 1);
    
    testCount++;
    m1 = malloc(sizeof(matrix));
    out = matrix_dot_prod(m1, NULL);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 2);
     
    testCount++;
    out = matrix_dot_prod(NULL, m1);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 3);

    testCount++;
    m2 = malloc(sizeof(matrix));
    out = matrix_dot_prod(m1, m2);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 4);

    testCount++;
    free(m1);
    m1 = matrix_create(2, 3);
    free(m2);
    m2 = matrix_create(4, 5);
    out = matrix_dot_prod(m1, m2); 
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
    matrix* m1 = matrix_initialize(2, 2, &Count);
    c = 0;
    matrix* m2 = matrix_initialize(2, 2, &Count);
    matrix* m3 = matrix_initialize_vararg(2, 2, 0., 2., 4., 6.);
    testsPassed += AH(m1, m2, m3, 1);

    testCount++;
    c = 0;
    m1 = matrix_initialize(5, 3, &Count);
    c = 0;
    m2 = matrix_initialize(5, 3, &Count);
    m3 = matrix_initialize_vararg(5, 3, 0., 2., 4., 6., 8., 10., 
        12., 14., 16., 18., 20., 22., 24., 26., 28.);
    testsPassed += AH(m1, m2, m3, 2);

    testCount++;
    c = 0;
    m1 = matrix_initialize(2, 6, &Count);
    c = 0;
    m2 = matrix_initialize(2, 6, &Count);
    m3 = matrix_initialize_vararg(2, 6, 0., 2., 4., 6., 8., 10., 12., 14., 16., 18., 20., 22.);
    testsPassed += AH(m1, m2, m3, 3);
    
    // test add with null arguements
    testCount++;
    int out = matrix_add(NULL, NULL);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 1);
    
    testCount++;
    m1 = malloc(sizeof(matrix));
    out = matrix_add(m1, NULL);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 2);
     
    testCount++;
    out = matrix_add(NULL, m1);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 3);

    testCount++;
    m2 = malloc(sizeof(matrix));
    out = matrix_add(m1, m2);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 4);

    testCount++;
    free(m1);
    m1 = matrix_create(2, 3);
    free(m2);
    m2 = matrix_create(4, 5);
    out = matrix_add(m1, m2); 
    testsPassed += Output(out == EXIT_FAILURE, "Incorrect Args", 1);
    

    testCount++;
    free(m1);
    m1 = matrix_create(4, 3);
    free(m2);
    m2 = matrix_create(4, 5);
    out = matrix_add(m1, m2); 
    testsPassed += Output(out == EXIT_FAILURE, "Incorrect Args", 2);

    testCount++;
    free(m1);
    m1 = matrix_create(3, 5);
    free(m2);
    m2 = matrix_create(4, 5);
    out = matrix_add(m1, m2); 
    testsPassed += Output(out == EXIT_FAILURE, "Incorrect Args", 3);

    printf("Tests Passed: %d/%d\n", testsPassed, testCount);
}

void  TestScaler() {
    printf("\n=== Scaler Test ===\n");
    int testCount = 0;
    int testsPassed = 0;

    testCount++;
    c = 0;
    matrix* m1 = matrix_initialize(2, 2, &Count);
    matrix* m3 = matrix_initialize_vararg(2, 2, 0., 2., 4., 6.);
    testsPassed += SH(m1, 2, m3, testCount); 

    testCount++;
    c = 0;
    m1 = matrix_initialize(2, 2, &Count);
    m3 = matrix_initialize_vararg(2, 2, 0., 0.5, 1., 1.5);
    testsPassed += SH(m1, 0.5, m3, testCount); 
    
    //null test
    testCount++;
    int out = matrix_scaler(NULL, 1);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 1);
    
    testCount++;
    m1 = malloc(sizeof(matrix));
    out = matrix_scaler(NULL, 1);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 2);

    printf("Tests Passed: %d/%d\n", testsPassed, testCount);
}

void TestSub() {
    printf("\n=== Subtraction Test ===\n");
    int testCount = 0;
    int testsPassed = 0;


        testCount++;
    c = 0;
    matrix* m1 = matrix_initialize(2, 2, &Count);
    c = 0;
    matrix* m2 = matrix_initialize(2, 2, &Count);
    matrix* m3 = matrix_create(2, 2);
    testsPassed += SubH(m1, m2, m3, 1);

    testCount++;
    c = 0;
    m2 = matrix_initialize(4, 2, &Count);
    m1 = matrix_initialize(4, 2, &Count);
    m3 = matrix_initialize(4, 2, &Eight);
    testsPassed += SubH(m1, m2, m3, 2);

    testCount++;
    m1 = matrix_initialize_vararg(2, 3, 4., 5., 6., 7., 8., 9.);
    m2 = matrix_initialize_vararg(2, 3, 2., 10., 8., 3., 2., 3.);
    m3 = matrix_initialize_vararg(2, 3, 2., -5., -2., 4., 6., 6.);
    testsPassed += SubH(m1, m2, m3, 3);
    
    // test sub with null arguements
    testCount++;
    int out = matrix_sub(NULL, NULL);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 1);
    
    testCount++;
    m1 = malloc(sizeof(matrix));
    out = matrix_sub(m1, NULL);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 2);
     
    testCount++;
    out = matrix_sub(NULL, m1);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 3);

    testCount++;
    m2 = malloc(sizeof(matrix));
    out = matrix_sub(m1, m2);
    testsPassed += Output(out == EXIT_FAILURE, "Null", 4);

    testCount++;
    free(m1);
    m1 = matrix_create(2, 3);
    free(m2);
    m2 = matrix_create(4, 5);
    out = matrix_sub(m1, m2); 
    testsPassed += Output(out == EXIT_FAILURE, "Incorrect Args", 1);
    
    // incorect arguements
    testCount++;
    free(m1);
    m1 = matrix_create(4, 3);
    free(m2);
    m2 = matrix_create(4, 5);
    out = matrix_sub(m1, m2); 
    testsPassed += Output(out == EXIT_FAILURE, "Incorrect Args", 2);

    testCount++;
    free(m1);
    m1 = matrix_create(3, 5);
    free(m2);
    m2 = matrix_create(4, 5);
    out = matrix_sub(m1, m2); 
    testsPassed += Output(out == EXIT_FAILURE, "Incorrect Args", 3);


    printf("Tests Passed: %d/%d\n", testsPassed, testCount);
}

void TestVector() {
    printf("\n=== Vector Test ===\n");
    int testCount = 0;
    int testsPassed = 0;

    //coorect args row
    testCount++;
    c = 0;
    matrix* m1 = matrix_initialize(2,2, &Count);
    matrix* m2 = matrix_get_vector_row(m1, 1);
    matrix* m3 = matrix_initialize_vararg(1,2, 2., 3.);
    testsPassed += Equal(m2, m3, "Correct Args", 1);
    matrix_free(m1);
    matrix_free(m2);
    matrix_free(m3);

    testCount++;
    c = 0;
    m1 = matrix_initialize(2,2, &Count);
    m2 = matrix_get_vector_col(m1, 1);
    m3 = matrix_initialize_vararg(2,1, 1., 3.);
    testsPassed += Equal(m2, m3, "Correct Args", 2);
    matrix_free(m1);
    matrix_free(m2);
    matrix_free(m3);

    // Null test
    testCount++;
    matrix* out = matrix_get_vector_col(NULL, 1);
    testsPassed += Output(out == NULL, "Null", 1);

    testCount++;
    out = matrix_get_vector_row(NULL, 1);
    testsPassed += Output(out == NULL, "Null", 2);

    // incorrect args
    testCount++;
    m1 = matrix_create(2, 2);
    out = matrix_get_vector_col(m1, 3);
    testsPassed += Output(out == NULL, "Incorrect Args", 1);

    testCount++;
    out = matrix_get_vector_row(m1, 3);
    testsPassed += Output(out == NULL, "Incorrect Args", 2);
    matrix_free(m1);


    printf("Tests Passed: %d/%d\n", testsPassed, testCount);
}


int main() {
    TestDotProd();
    TestAdd();
    TestSub();
    TestScaler();
    TestVector();

    printf("\nTesting done!\n");
}

