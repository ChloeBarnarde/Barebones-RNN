#include <stdio.h>
#include <stdlib.h>

struct test {
    int i;
};

int main() {
    struct test* t = malloc(sizeof(struct test));

    t->i = 1;
    int i = t->i;
    printf("%d\n", i);
}