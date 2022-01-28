#include <stdlib.h>
#include "list.h"
#include "lexer.h"

void list_init(list *list)
{
    list->head = NULL;
    list->tail = NULL;
}

int list_is_empty(list *list)
{
    return list->head == NULL;
}

size_t list_len(list *list)
{
    list_elm *p = list->head;
    size_t acc = 0;
    while(p->next != list_tail)
    {
        acc++;
        p = p->next;
    }
    return acc+1;
}

void list_push_end(list *list, token *t)
{
    list_elm *elm = xmalloc(sizeof(list_elm));
    elm->token = t;
    if (list->head == NULL)
        list->head = elm;
    list->tail->next = elm;
    list->tail = elm;
}

void list_push_front(list *list, token *t)
{
    list_elm *elm = xmalloc(sizeof(list_elm));
    elm->token = t;
    if (list->head == NULL)
        list->tail = elm;
    elm->next = list->head;
    list->head = elm;
}

token *list_pop_front(list *list)
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

token *list_find(list *list, token *token)
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


void list_rev(list *list)
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
