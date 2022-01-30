#ifndef PREFIX_GRAPH_H
#define PREFIX_GRAPH_H

#include "vector.h"

typedef struct Adjlists
{
    struct Adjlists *next;
    int index;
}Adjlists;

typedef struct Node
{
    char value;
    int is_end;
    int self;
    int father;
    Adjlists *adjlists;
}Node;

typedef struct List_Graph
{
    struct List_Graph *next;
    Node *node;
}List;

typedef struct Prefix_Graph
{
    Vector *father;
    List *nodes;
    int order;
    int cur_pos;
    int longest;
    int over_write;
}Pgraph;

void print_words(Pgraph *G);

Pgraph *init_Pgraph();

void free_Pgraph(Pgraph *G);

Node* get_node(Pgraph *G, int i);

Pgraph *create_Pgraph_with_dir(char *cur_dir);

void add_word(Pgraph *G, char *word, size_t size);

char* get_word(Pgraph *G, char c);

#endif
