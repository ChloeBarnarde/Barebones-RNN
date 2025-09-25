#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H
#include <stdio.h>
#include <stdlib.h>

// currently just a 2d matrix :p
struct {
    //int* dimensions; // can be added later ig

    int* size;
    float* values;
} typedef matrix;


int CrToIdx(int col, int row, matrix* m) {
    return col * m->size[1] + row; 
}


/// @brief Dot Product of a and b. Doesn't mutate a or b
/// @param a matrix* a
/// @param b matrix* b
/// @return result of dot product
matrix* MDotProd(matrix* a, matrix* b, matrix* output) {
    // need to make sure a->size[1] == b->size[0]
    if (a->size[1] != b->size[0]) {
        printf("Passed Matrices don't have sizes that can be multiplied together a: %d, %d\n", a->size[0], a->size[1]);
        return output;
    }
    
    int size[2] = {a->size[0], b->size[1]}; // might need to rewrite this

    InitializeValues(output, size);
    
    // just gonna use naive implementation for now
    for (int i = 0; i < a->size[0]; i++) {
        for (int j = 0; j < b->size[1]; j++) {
            for (int p = 0; p < a->size[1]; p++) {
                output->values[CrToIdx(i, j, output)] += a->values[CrToIdx(i, p, a)] * b->values[CrToIdx(p, j, b)]; // need to double check this ig
            }
        }
    }
    return output;
}


// this needs to be heavily modified
int InitializeValues(matrix* m, float sizeGoal[2]) {
    if (m->values == NULL) {
        m->values = (float *)malloc(sizeGoal[0] * sizeof(float) * sizeGoal[1] * sizeof(float));
        return EXIT_SUCCESS;
    }

    if (m->size == NULL) {
        // don't know size sooo??? do above
        m->size[0] = sizeGoal[0];
        m->size[1] = sizeGoal[1];
        free(m->values);
        m->values = (float *)malloc(sizeGoal[0] * sizeof(float) * sizeGoal[1] * sizeof(float));
        return EXIT_SUCCESS;
    }


    if (m->size[0] == sizeGoal[0] && m->size[1] == sizeGoal[2]) {
        return EXIT_SUCCESS;
    }

    m->size[0] = sizeGoal[0];
    m->size[1] = sizeGoal[1];
    free(m->values);
    m->values = (float *)malloc(sizeGoal[0] * sizeof(float) * sizeGoal[1] * sizeof(float));

}

// /// @brief Multiplies all elements in a with b. Doesn't mutate a
// /// @param a Matrix pointer
// /// @param b Scaler to multiply with
// /// @return new matrix from the resault of a * b
// matrix* MScaler(matrix* a, float b) {
//     matrix* output = (matrix*)malloc(sizeof(matrix));
//     int size[2] = {a->size[0], a->size[1]};
//     output->size = &size[0];

//     for (int i = 0; i < a->size[0]; i++) {
//         for (int j = 0; j < a->size[1]; j++)
//         {
//             output->values[CrToIdx(i, j, output)] = output->values[CrToIdx(i, j, output)] * b;
//         }
//     }
//     return output;
// }

// matrix* MElementWiseFuncF(matrix* a, matrix* output, float (*fptr)(float)) {

//     int size[2] = {a->size[0], a->size[1]};
//     output->size = &size[0];
    
//     if (output->values == NULL)
//     for (int i = 0; i < a->size[0]; i++) {
//         for (int j = 0; j < a->size[1]; j++)
//         {
//             output->values[CrToIdx(i, j, output)] = fptr(a->values[CrToIdx(i, j, a)]);
//         }
//     }

//     return output;
// }


// matrix* MElementWiseFuncFF(matrix* a, matrix* b, matrix* output, float (*fptrs)(float, float)) {
    
//     if (a->size[0] != b->size[0] && a->size[1] != b->size[1]) {
//         printf("Passed matrices didn't have matching sizes! \nA size: %d, %d\nB size: %d, %d", 
//             a->size[0], a->size[1], b->size[0], b->size[1]);
//     }

//     int size[2] = {a->size[0], a->size[1]};
//     output->size = &size[0];


//     for (int i = 0; i < a->size[0]; i++) {
//         for (int j = 0; j < a->size[1]; j++)
//         {
//             output->values[CrToIdx(i, j, output)] = fptr(a->values[CrToIdx(i, j, a)], b->values(CrToIdx(i, j, b)));
//         }
//     }

//     return output;    
// }


// float b = 0;
// float add(float a) {
//     return  a+ b;
// }
// matrix* MAdd(matrix* a, matrix* b) {
    
//     matrix* output = (matrix*)malloc(sizeof(matrix));

//     if (a->size[0] != b->size[0] && a->size[1] != b->size[1]) {
//         printf("Passed matrices didn't have matching sizes! \nA size: %d, %d\nB size: %d, %d", 
//             a->size[0], a->size[1], b->size[0], b->size[1]);
//         return EXIT_FAILURE;
//     }


// }


// // wanna rewrite this so that its one function, cause atm its going through the matrix twice ;-;
// int MSub(matrix* a, matrix* b, matrix* output) {
//     matrix* c = malloc(sizeof(matrix));
//     MScaler(b, -1, c);
//     MAdd(a, c, output);

//     return EXIT_SUCCESS;
// }


// int MVec(matrix* a, int col, matrix* output) {
//     if (col >= a->size[0]) {
//         // error
//         printf("Passed collumn (%d) is greater than matrix columns (%d)", col, a->size[0]);
//         return EXIT_FAILURE;
//     }

//     output->size[0] = 1;
//     output->size[1] = a->size[1]; 

//     return EXIT_SUCCESS;
// }


#endif