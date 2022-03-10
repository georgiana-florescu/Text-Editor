#ifndef STACK_H
#define STACK_H

typedef struct Node {
    char data[20];
    struct Node *next;
} Node;

typedef struct {
    int size;
    Node *top;
} Stack;

//initializeaza o stiva
Stack* init_stack();

//returneaza 1 daca stiva e goala
int is_empty_stack(Stack *s);

//insereaza un element la inceputul stivei
void push(Stack *s, char *x);

//sterge primul element din stiva
void pop(Stack *s);

//goleste stiva
void free_stack(Stack *s);

#endif