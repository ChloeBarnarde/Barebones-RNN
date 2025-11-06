#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "matrixMath.h"

struct list {
    char token;
    int index;
    struct list* next;
    struct list* prev;
} typedef list;

list* add(list* l, char token) {
    list* new = malloc(sizeof(list));
    l->next = new;
    new->token = token;
    new->index = l->index + 1;
    new->prev = l;

    return new;
}

bool contains_next(list* l, char token) {
    if (l->token == token)
        return true;
    if (l->next != NULL)
        return contains_next(l->next, token);

}
bool contains(list* l, char token) {
    if (l->token == token)
        return true;
    
    bool prev = false;
    bool next = false;
    //printf("prev check\n");
    if (l->prev != NULL)
    prev = contains(l->prev, token);
    if (prev)
    return prev;
    
    //printf("next check\n");
    if (l->next != NULL)
        next = contains_next(l->next, token);
    
    return next;
}



int free_list_prev(list* l) {
    if (l->prev != NULL)
        free_list_prev(l->prev);
    free(l);
}

int free_list_next(list* l) {
    if (l->next != NULL)
        free_list_next(l->next);
    free(l);
}

int free_list(list* l) {
    if (l->prev != NULL)
        free_list_prev(l->prev);
    if (l->next != NULL)
        free_list_next(l->next);
    free(l);
    
}

int setHotCoding(matrix* m, int i, char a, list* l) {

}

int main() {
    FILE* fp;
    char row[1000];
    char* token;

    fp = fopen("data/TheExaminer_SpamClickbaitCatalog/examiner_date_tokens_short.csv", "r");
    
    list* char_types = malloc(sizeof(list));
    char_types->prev = NULL;
    char_types->next = NULL;
    char_types->index = 0;
    char_types->token = 'a';
    
    int size = 0;
    
    while(fgets(row, 1000, fp)) {
    // while(size < 5) {
        size++;
        //printf("Beans3\n");
        token = strtok(row, "|"); // we know that I don't want one
        token = strtok(NULL, "|");
        //printf("%s\n", token);

        for (size_t i = 0; i < strlen(token); i++)
        {
            //printf("Beans4\n");
            if (contains(char_types, token[i]))
                continue;
            //printf("Beans5\n");
            char_types = add(char_types, token[i]);
            //printf("character index: %d, %c\n", char_types->index, char_types->token);    
        }
    }
    fclose(fp);
    free_list(char_types);


    // x size of n-1 x char_types size
    // y size of n-1 x char_types size
    
    //loop through again




    printf("done\n");

}