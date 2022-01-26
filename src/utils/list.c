#include <stdlib.h>
#include "list.h"
#include "lexer.h"

void list_init(struct list *list)
{
    list->next = NULL;
    list->token = NULL;
}

int list_is_empty(struct list *list)
{
    return list->next == NULL;
}

size_t list_len(struct list *list)
{
    struct list *p = list;
    size_t acc = 0;
    while(p->next != NULL)
    {
        acc++;
        p = p->next;
    }
    return acc;
}

void list_push_front(struct list *list, struct list *elm)
{
    elm->next = list->next;
    list->next = elm;
}

struct list *list_pop_front(struct list *list)
{
    struct list *toPop = list->next;
    if(toPop != NULL)
    {
        list->next = toPop->next;
    }
    toPop->next = NULL;
    return toPop;
}

struct list *list_find(struct list *list, token *token)
{
    struct list *p = list->next;
    while(p != NULL)
    {
        if(p->token == token)
            return p;
        p = p->next;
    }
    return NULL;
}


void list_insert(struct list *list, struct list *elm)
{
    struct list * p = list;
    while(p->next != NULL)
    {
        if((p->next)->data > elm->data)
        {
            elm->next = p->next;
            p->next = elm;
            return;
        }
        p = p->next;
    }
    p->next = elm;
}

void list_rev(struct list *list)
{
    struct list *current = list->next, *prev = NULL, *next = NULL;

    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list->next = prev;
}

void list_half_split(struct list *list, struct list *second)
{
    size_t len = list_len(list);
    struct list *p = list;
    for(size_t i = 0; i<len/2; i++)
    {
       p = p->next;
    }
    second->next = p->next;
    p->next = NULL;
}
