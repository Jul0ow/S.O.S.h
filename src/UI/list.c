#include <stdlib.h>
#include <err.h>
#include "list.h"

List* list_init()
{
    List *l = malloc(sizeof(struct list));
    if (l == NULL)
	errx(1, "Not enough memory");
    l->next = NULL;
    l->data = 0;
    return l;
}

void list_free(List* l)
{
    free(l);
}

int list_is_empty(struct list *list)
{
    return list->next == NULL;
}

size_t list_len(struct list *list)
{
    size_t len;
    for (len = 0; list->next; list = list->next)
        len++;
    return len;
}

void list_push_front(struct list *list, struct list *elm)
{
    elm->next = list->next;
    list->next = elm;
}

void list_append(struct list *list, struct list *elm)
{
    for (; list->next; list = list->next)
        continue;
    elm->next = list->next;
    list->next = elm;
}

struct list *list_pop_front(struct list *list)
{
    if (list->next)
    {
        struct list* res = list->next;
        list->next = res->next;
        return res;
    }
    return NULL;
}

/*
struct list *list_find(struct list *list, void* value)
{
    for (;list->next; list = list->next)
    {
        if (list->next->data == value)
            return list->next;
    }
    return NULL;
}


struct list *list_lower_bound(struct list *list, int value)
{
    for (;list->next; list = list->next)
    {
        if (list->next->data > value)
            return list;
    }
    return list;
}

int list_is_sorted(struct list *list)
{
    list = list->next;
    if (!list)
        return 1;
    for (;list->next; list = list->next)
    {
        if (list->data > list->next->data)
            return 0;
    }
    return 1;
}


void list_insert(struct list *list, struct list *elm)
{
    for (; list->next && list->next->data < elm->data; list = list->next)
        continue;
    elm->next = list->next;
    list->next = elm;
}


void list_rev(struct list *list)
{
    if (list->next)
    {
        struct list *sentinel = list;
        struct list *tmp;
        struct list *prev;
        prev = list->next;
        list = prev->next;
        prev->next = NULL;
        for (;list;)
        {
             tmp = list->next;
             list->next = prev;
             prev = list;
             list = tmp;
        }
        sentinel->next = prev;
    }
}

void list_half_split(struct list *list, struct list *second)
{
    size_t len = list_len(list);
    len = len / 2 + len % 2;
    for (size_t i = 0; i < len; i++)
        list = list->next;
    second->next = list->next;
    list->next = NULL;
}
*/
