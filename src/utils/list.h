#ifndef _LIST_H_
#define _LIST_H_
#include "lexer.h"

typedef struct
{
    struct list *head;
    struct list *tail;
}list;

typedef struct 
{
  struct list_elm *next;
  token *token;
}list_elm;

// Initialize the sentinel of an empty list.
void list_init(list *list);

// Return true if the list is empty.
// Otherwise, return false.
// Do not forget that there is always a sentinel.
// So the list is empty if the sentinel does not point to a next element.
int list_is_empty(list *list);

// Return the length of the list (sentinel excluded).
size_t list_len(list *list);

// Insert 'elm' in front of the list, that is just after the sentinel.
// Note that 'elm' is already an existing element.
// You just have to insert it.
void list_push_front(list *list, token *t);

void list_push_end(list *list, token *t);

// Extract the first element (not the sentinel) of the list.
// This operation removes the element from the list and returns it
// (the caller is responsible for freeing it).
// If the list is empty, the function returns NULL.
token *list_pop_front(list *list);

// Search for the first element that contains 'value' and return it
// (without removing it from the list).
// The function returns NULL if the value is not in the list.
token *list_find(list *list, token *token);

// Reverse the elements of the list (except for the sentinel).
void list_rev(list *list);

#endif
