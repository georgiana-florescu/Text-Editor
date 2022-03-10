#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Stack* init_stack()
{
    Stack * s;
    s = (Stack*)malloc(sizeof(Stack));
    s->size=0;
    s->top=NULL;
    return s;
}

int is_empty_stack(Stack *s)
{
    if(s->top==NULL) 
        return 1;
    return 0;
}

void push(Stack *s, char* x)
{
    Node *new = (Node *)malloc(sizeof(Node));
    strcpy(new->data, x);
    if(s->top==NULL) {
        new->next=NULL;
        s->top=new;
        s->size++;
    }
    else {
        new->next=s->top;
        s->top=new;
        s->size++;
    }
}

void pop(Stack *s)
{
    Node * del;
    if(s->top==NULL) 
        return;
    else {
        del = s->top;
        s->top =s->top->next;
        s->size--;
        free(del);
    }
}

void free_stack(Stack *s) 
{
    while(!is_empty_stack(s)) {
        pop(s);
    }
}
