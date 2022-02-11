#include <stdlib.h>
#include "listT.h"
#include "lexer.h"
#include "xmalloc.h"


void freeL(listT *l)
{
    list_elm *elm = l->head;
    while(elm != NULL)
    {
        list_elm *next = elm->next;
        if(elm->token->type == COMMAND || elm->token->type == ARGUMENT)
            free(elm->token->string);
        free(elm->token);
        
        free(elm);
        elm = next;
    }
    free(l);
}
void list_init(listT *list)
{
    list->head = NULL;
    list->tail = NULL;
}

int list_is_emptyT(listT *list)
{
    return list->head == NULL;
}

size_t list_lenT(listT *list)
{
    list_elm *p = list->head;
    size_t acc = 0;
    while(p->next != list->tail)
    {
        acc++;
        p = p->next;
    }
    return acc+1;
}

void list_push_endT(listT *list, token *t)
{
    list_elm *elm = Xmalloc(sizeof(list_elm));
    elm->token = t;
    elm->next = NULL;
    if (list->head == NULL)
    {
        list->head = elm;
        list->tail = elm;
    } 
    else
    {
        list->tail->next = elm;
        list->tail = elm;
    }
}

void list_push_frontT(listT *list, token *t)
{
    list_elm *elm = Xmalloc(sizeof(list_elm));
    elm->token = t;
    if (list->head == NULL)
        list->tail = elm;
    elm->next = list->head;
    list->head = elm;
}

token *list_pop_frontT(listT *list)
{
    list_elm *toPop = list->head;
    if(toPop != NULL)
    {
        list->head = toPop->next;
        if (list->head == NULL)
            list->tail = NULL;
    }
    toPop->next = NULL;
    token *t = toPop->token;
    free(toPop);
    return t;
}

list_elm *list_findT(listT *list, token *token)
{
    list_elm *p = list->head;
    while(p != NULL)
    {
        if(p->token == token)
            return p;
        p = p->next;
    }
    return NULL;
}


void list_revT(listT *list)
{
    list_elm *current = list->head, *prev = NULL, *next = NULL;
    list->tail = list->head;
    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list->head = prev;
}
