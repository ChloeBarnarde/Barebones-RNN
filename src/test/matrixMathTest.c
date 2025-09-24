#include "../matrixMath.h"

int main() {
    matrix* m1 = malloc(sizeof(matrix));
    matrix* m2 = malloc(sizeof(matrix));
    matrix* m3 = malloc(sizeof(matrix));
    
    int size[2] = {1, 2};
    m1->size = &size[0];
    float values1[2] = {1, 2};
    m1->values = &values1[0];
    
    
    
    int size2[2] = {2, 1};
    m2->size = &size2[0];
    float values2[2] = {3, 4};
    m2->values = &values2[0];
    
    for (int i = 0; i < 2; i++)
    {
        printf("%f \n", m2->values[CrToIdx(0, i, m2)]);
    }
    
    MDotProd(m1, m2, m3);

    
    printf("%f\n", m3->values[CrToIdx(0, 0, m3)]);
}