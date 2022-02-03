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

typedef struct Word
{
    char *w;
    int index;
}Word;

typedef struct Prefix_Graph
{
    List *nodes;
    int order;
    int cur_pos;
    int longest;
    int over_write;
    int last_pos;
    Word *word;
}Pgraph;

typedef struct List_Pgraph
{
    struct List_Pgraph *next;
    struct List_Pgraph *prev;
    Pgraph *G;
}LPgraph;

void print_words(Pgraph *G);

Pgraph *init_Pgraph();

LPgraph *init_LPgraph(Pgraph *G);

void free_Pgraph(Pgraph *G);

void free_LPgraph(LPgraph *LG);

void LPgraph_append(LPgraph *LG, Pgraph *G);

void LPgraph_pop(LPgraph *LG);

Node* get_node(Pgraph *G, int i);

void set_pos_to_father(Pgraph *G, Node *node);

Pgraph *create_Pgraph_with_dir(char *cur_dir);

void add_word(Pgraph *G, char *word, size_t size);

char* get_word(Pgraph *G, char c);

#endif
