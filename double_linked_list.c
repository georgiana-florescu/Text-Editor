#include "double_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

List *init_List() {
    List * list;
    list = (List * )malloc(sizeof(List));
    list->head=NULL;
    list->tail=NULL;
    return list;
}

int is_empty(List *list) {
    if(list->head==NULL) return 1;
    else return 0;
}

void Insert(List *list, char val) {
    DLNode *new=(DLNode*)malloc(sizeof(DLNode));
    new->data=val;
    new->next=NULL;
    if(is_empty(list)) {
        list->head=new;
        list->tail=new;
        new->prev=NULL;
    }
    else {
        list->tail->next=new;
        new->prev=list->tail;
        list->tail=new;
    }
}

DLNode* insert(List *text, DLNode *pos, char c)
{
    DLNode *node = (DLNode *)malloc(sizeof(DLNode));
    node->data = c;
    if(is_empty(text)) {
        node->next=node->prev=NULL;
        text->head=text->tail=node;
        return node;
    }
    if(pos==NULL) {
        text->tail->next = node;
        node->next = NULL;
        node->prev = text->tail;
        text->tail = node;
        return node;
    }
    else {
        node->next = pos->next;
        node->prev = pos;
        if(pos->next==NULL) {
            pos->next=node;
            text->tail=node;
            return node;
        }
        pos->next->prev = node;
        pos->next = node;
        return node;
    }
}

void free_list(List *list)
{
    while(!is_empty(list)) {
        delnode(list,list->head);
    }
}

void delnode(List *text, DLNode *del)
{
    if (text->head == NULL || del == NULL)
        return;
    if (text->head == del)
        text->head = del->next;
    if (del->next != NULL)
        del->next->prev = del->prev;
    else {
        text->tail=del->prev;
    }
    if (del->prev != NULL)
        del->prev->next = del->next;
    free(del);
    return;
}

List* copy_list(List *list1,List *list2)
{
    DLNode *first = list1->head;
    while(first!=NULL) {
        Insert(list2,first->data);
        first = first->next;
    }
    return list2;
}