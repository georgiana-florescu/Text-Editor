#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "double_linked_list.h"

Stack *undo, *redo;
List *del_nodes;

typedef struct Cursor
{
    int line, ch;
} Cursor;

Cursor cursor;

//returneaza nodul pe care se afla cursorul
DLNode * search_pos(List *text)
{
    DLNode *pos = text->head;
    int k = 1;
    while(k < cursor.line)
    {
        if(pos->data=='\n')
            k++;
        pos = pos->next;
    }
    k=1;
    while(k < cursor.ch)
    {
        pos = pos->next;
        k++;
    }
    return pos;
}

//returneaza nodul corespunzator primei aparitii a cuvantului in text
DLNode * find_word(List *text, char *str)
{
    DLNode * pos, *res;
    pos = search_pos(text);
    int k;
    while(pos!=NULL)
    {
        k=0;
        while(pos->data!=str[0]){
            pos=pos->next;
            if(pos==NULL)
                return NULL;
        }
        res = pos;
        while(res!=NULL && str[k]!='\n' && str[k]!=' ')
        {
            if(res->data==str[k]) {
                k++;
                res=res->next;
            }
            else 
             break;
        }
        if(str[k]!='\n' && str[k]!=' ')
            pos=pos->next;
        else
            return pos;
    }
}

//initializeaza cursorul
void init_cursor()
{
    cursor.line = 1;
    cursor.ch = 1;
}

//citeste in lista textul pentru inserare
int read_text(List *text, FILE *fi)
{
    int p=0, numchar=0, rand=0;
    char *str = (char*)malloc(100);
    DLNode *pos = search_pos(text);
    if(pos!=text->head && pos!=NULL)
        if(pos->prev->data=='\n') {
            pos=pos->prev;
            p=1;
        }
    while(1)
    {
        int k = 0;
        fgets(str,100,fi);
        rand++;
        if(!strncmp(str,"::",2)) {
            //pentru a nu adauga \n la inserarea in mijloc
            if(pos->next!=NULL && p==0 && rand<3) {
                delnode(text,pos);
                numchar--;
            }
            free(str);
            return numchar;
        }
        while(str[k]!='\0')
        {
            pos = insert(text, pos, str[k]);
            k++;
            numchar++;
        }
        cursor.line++;
    }
}

//sterge liniile libere din text
void free_newlines(List *text)
{
    DLNode *pos = text->head,*q;
    while(pos->next!=NULL)
    {
        q = pos;
        pos = pos->next;
        if(q->data=='\n' && q->next->data=='\n')
            delnode(text,q);
    }
}

//modifica pozitia cursorului
void Goto(char *str)
{
    if(!strncmp(str,"gl",2)) {
        cursor.line = str[3]-'0';
        cursor.ch = 1;
    }
    else {
        cursor.ch = str[3]-'0';
        if(str[4]!='\n') {
            cursor.line = str[5]-'0';
        }
    }
}

//sterge o linie din text
void delete_line(List *text, char *str) {
    DLNode *first = text->head, *q;
    int nr;
    if(undo->top->data[2]=='\n')
        nr = cursor.line;
    else
        nr = atoi(undo->top->data+3);
    while(nr-1) {
        if(first->data=='\n') {
            nr--;
        }
        first=first->next;
    }
    while(first->data!='\n') {
        q=first;
        first=first->next;
        Insert(del_nodes,q->data);
        delnode(text,q);
    }
    Insert(del_nodes,first->data);
    Insert(del_nodes,'\0');
    delnode(text,first);
}

//sterge caracterul din spatele cursorului
void backspace(List *text)
{
    DLNode * pos = search_pos(text);
    if(pos==NULL) delnode(text,text->tail->prev);
    else delnode(text,pos);
    cursor.ch--;
}

//inlocuieste primul cuvant din string cu urmatorul
void Replace(List *text, char *str)
{
    DLNode *pos = find_word(text,str), *q;
    int k=0;
    //sterge vechiul cuvant
    while(str[k]!=' ') {
        q = pos;
        pos = pos->next;
        delnode(text,q);
        k++;
    }
    k++;
    pos = pos->prev;
    //insereaza noul cuvant
    while(str[k]!='\n') {
        DLNode *node = (DLNode*)malloc(sizeof(DLNode));
        node->data=str[k++];
        if(pos==NULL) {
            node->prev=NULL;
            node->next=text->head;
            text->head->prev=node;
            text->head=node;
        }
        else {
            node->next=pos->next;
            node->prev=pos;
            pos->next->prev=node;
            pos->next=node;
        }
        pos = node;
    }
}

//sterge caracterul urmator dupa cursor
void just_delete(List *text, char *str)
{
    DLNode *pos = search_pos(text);
    int n = atoi(str+2);
    if(!n) n = 1;
    if(pos==text->head) n--;
    while(n>0)
    {
        Insert(del_nodes,pos->next->data);
        delnode(text,pos->next);
        n--;
    }
    if(pos==text->head) delnode(text,text->head);
    Insert(del_nodes,'\0');
}

//sterge str din lista si intoarce 0 daca nu a sters nimic
int delete_word(List *text, char *str)
{
    DLNode *pos = find_word(text,str),*q;
    int k=0;
    if(pos==NULL) return 0;
    while(str[k]!='\n')
    {
        q = pos;
        pos = pos->next;
        delnode(text,q);
        k++;
    }
    return 1;
}

//sterge toate aparitiile cuvantului in lista
void delete_all(List *text, char *str)
{
    Cursor c = cursor;
    int i;
    init_cursor();
    DLNode *pos = find_word(text,str), *q;
    while(pos!=NULL)
    {
        i=0;
        while(str[i++]!='\n') {
            q=pos;
            pos=pos->next;
            delnode(text,q);;
        }
        pos = find_word(text,str);
    }
    cursor = c;
    free_newlines(text);
}

//inlocuieste toate aparitiile cuvantului cu urmatorul din str
void replace_all(List *text, char *str)
{
    Cursor c = cursor;
    init_cursor();
    DLNode *pos = find_word(text,str);
    while(pos!=NULL)
    {
        Replace(text,str);
        pos = find_word(text,str);
    }
    cursor = c;
}

void Undo(List *text, Cursor c, int *vec, int n)
{
    cursor = c;
    DLNode *pos = search_pos(text), *q;
    char *str, *pnt;
    int k=3;
    switch (undo->top->data[0])
    {
        case ':':
            while(vec[n]>1){
                q=pos;
                pos=pos->next;
                Insert(del_nodes,pos->prev->data);
                delnode(text,q);
                vec[n]--;
            }
            delnode(text,pos);
            Insert(del_nodes,'\n');
            Insert(del_nodes,'\0');
            break;
        case 'd':
            if(undo->top->data[1]=='w') {
                if(pos->prev->data=='\n') 
                    pos=pos->prev;
                while(undo->top->data[k]!='\n')
                pos = insert(text,pos,undo->top->data[k++]);
                k=3;
            }
            else {
                delnode(del_nodes,del_nodes->tail);
                if(pos->prev->data=='\n')
                    pos = pos->prev;
                while(del_nodes->tail->data!='\0') {
                    pos = insert(text,pos,del_nodes->tail->data);
                    delnode(del_nodes,del_nodes->tail);
                }
            }
            break;
        case 'r':
            str=malloc(100);
            pnt = undo->top->data+3;
            int i=0,j=0;
            while(pnt[i]!=' ')
                i++;
            i++;
            while(pnt[i]!='\n')
                str[j++]=pnt[i++];
            str[j++]=' ';
            i=0;
            while(pnt[i]!=' ')
                str[j++]=pnt[i++];
            str[j]='\n';
            Replace(text,str);
            free(str);
            break;
        default:
            break;
    }
}

void Redo(List *text)
{
    DLNode *pos = search_pos(text);
    switch (redo->top->data[0])
    {
        case ':':
            while(del_nodes->head->data!='\0')
            {
                pos = insert(text,pos,del_nodes->head->data);
                delnode(del_nodes,del_nodes->head);
            }
            delnode(del_nodes,del_nodes->head);
            break;
        case 'd':
            if(redo->top->data[1]=='w')
                delete_word(text,redo->top->data+3);
            else if(redo->top->data[1]=='l')
                delete_line(text,redo->top->data);
            else 
                just_delete(text,redo->top->data);
            break;
        case 'g':
            Goto(redo->top->data);
            break;
        case 'r':
            Replace(text,redo->top->data+3);
            break;
        default:
            break;
    }
}

//functie pentru citirea si executarea imediata a operatiilor
List* execute_commands(List *text, FILE *fi)
{
    List *saved_list = init_List();
    char *str = (char *)malloc(20);
    int k=1, n=0, isdel, numchar[100];
    Cursor c[100];
    c[0]=cursor;
    while(1)
    {
        fgets(str,20,fi);
        switch (str[0])
        {
            case 's':
                free_list(saved_list);
                copy_list(text,saved_list);
                break;
            case 'g':
                c[k++]=cursor;
                push(undo,str);
                Goto(str);
                break;
            case 'd':
                c[k++]=cursor;
                push(undo,str);
                if(str[1]=='l')
                    delete_line(text,str);
                else if(str[1]=='w') {
                    isdel = delete_word(text,str+3);
                    if(!isdel) {
                        pop(undo);
                        //daca nu am sters nimic, la undo nu inseram nimic
                        push(undo,"nothing");
                    }
                }
                else if(str[1]=='a') 
                    delete_all(text,str+3);
                else
                    just_delete(text,str);
                break;
            case 'q':
                free(str);
                return saved_list;
            case 'b':
                c[k++]=cursor;
                push(undo,str);
                backspace(text);
                break;
            case 'u':
                push(redo,undo->top->data);
                if(undo->top->data[0]=='g') {
                    cursor=c[k-1];
                }
                else {
                    if(undo->top->data[0]==':') n--;
                    Undo(text,c[k-1],numchar,n);
                }
                k--;
                pop(undo);
                break;
            case 'r':
                c[k++]=cursor;
                if(str[1]=='e') {
                    push(undo,str);
                    Replace(text,str+3);
                }
                else if(str[1]=='a'){
                    push(undo,str);
                    replace_all(text,str+3);
                }
                else {
                    push(undo,redo->top->data);
                    Redo(text);
                    free_newlines(text);
                    pop(redo);
                }
                break;
            case ':':
                push(undo,str);
                c[k++]=cursor;
                numchar[n++]=read_text(text,fi);
                break;
        }
    }
}

//insereaza textul din lista in fisierul de iesire
void insert_text(List *text)
{
    DLNode* pos = text->head;
    FILE *fo = fopen("editor.out", "w");
    while(pos!=NULL)
    {
        fputc(pos->data,fo);
        pos=pos->next;
    }
    fclose(fo);
}

int main()
{
    FILE *fi = fopen("editor.in", "r");
    List *text = init_List(text), *saved_list;
    init_cursor();
    undo = init_stack();
    redo = init_stack();
    del_nodes=init_List();
    read_text(text,fi);
    saved_list = execute_commands(text,fi);
    insert_text(saved_list);

    //eliberarea memoriei
    fclose(fi);
    free_list(text);
    free(text);
    free_list(saved_list);
    free(saved_list);
    free_stack(undo);
    free(undo);
    free_stack(redo);
    free(redo);
    free_list(del_nodes);
    free(del_nodes);
    return 0;
}