#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

typedef struct DLNode
{
    struct DLNode * next;
    struct DLNode * prev;
    char data;
} DLNode;

typedef struct List
{
    struct DLNode * head;
    struct DLNode * tail;
} List;

//initializeaza lista
List* init_List();

//returneaza 1 daca lista este goala
int is_empty(List *list);

//insereaza un caracter la finalul listei
void Insert(List *list, char val);

//insereaza un caracter la pozitia specificata prin pos
//returneaza nodul inserat
DLNode* insert(List *text, DLNode *pos, char c);

//elibereaza lista
void free_list(List *list);

//sterge nodul del din lista
void delnode(List *text, DLNode *del);

//copiaza list1 in list2
List* copy_list(List *list1,List *list2);

#endif
