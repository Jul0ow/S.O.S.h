#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "prefix_graph.h"

/*==================== Print ===============================*/
//Simple DFS

void __aux(Pgraph *G, Node *node, char *s, int i)
{
    Node *n;
    if (node->is_end == 1)
    {
	s[i] = 0;
        printf("%s\n", s);
    }
    for (Adjlists *adj = node->adjlists->next; adj; adj = adj->next)
    {
	n = get_node(G, adj->index);
	s[i] = n->value;
	__aux(G, n, s, i + 1);
    }
    
    s[i] = 0;
}

void print_words(Pgraph *G)
{
    char *s = malloc((G->longest + 1) * sizeof(char));
    __aux(G, G->nodes->node, s, 0);
    free(s);
}

/*==================== Init struct ===============================*/

Adjlists *init_adjlists(int i)
{
    Adjlists *adjl = malloc(sizeof(Adjlists));
    adjl->next = NULL;
    adjl->index = i;
    return adjl;
}

Node *init_node(char v, int end, int self, int father, Adjlists *adjl)
{
    Node *node = malloc(sizeof(Node));
    node->value = v;
    node->is_end = end;
    node->self = self;
    node->father = father;
    node->adjlists = adjl;
    return node;
}

List *init_list(Node *node)
{
    List *l = malloc(sizeof(List));
    l->next = NULL;
    l->node = node;
    return l;
}

Pgraph *init_Pgraph()
{
    Pgraph *G = malloc(sizeof(Pgraph));
    G->father = vector_new();
    Node *node = init_node('$', 0, 0, -1, NULL);
    G->nodes = init_list(node);
    G->order = 1;
    G->cur_pos = 0;
    G->longest = 0;
    G->over_write = 0;
    return G;
}

/*==================== Free struct ===============================*/

void free_adjlists(Adjlists *adj)
{
    Adjlists *p;
    while (adj != NULL)
    {
	p = adj->next;
	free(adj);
	adj = p;
    }
}

void free_node(Node *node)
{
    free_adjlists(node->adjlists);
    free(node);
}

void free_list(List *l)
{
    List *p;
    while (l != NULL)
    {
	p = l->next;
	free_node(l->node);
        free(l);
	l = p;
    }
}

void free_Pgraph(Pgraph *G)
{
    vector_free(G->father);
    free_list(G->nodes);
    free(G);
}

/*==================== Utils ===============================*/

Node* get_node(Pgraph *G, int ind)
{
    List *l;
    int i = 0;
    for (l = G->nodes; l->next; l = l->next)
    {
	if (i == ind)
	    return l->node;
	i++;
    }
    return l->node;
}

int list_append(List *list, List *elm)
{
    int i = 0;
    for (; list->next; list = list->next)
        i++;
    elm->next = list->next;
    list->next = elm;
    return i + 1;
}

void adjlists_insert(Pgraph *G, Adjlists *list, Adjlists *elm, char c)
{
    for (; list->next && c > get_node(G, list->next->index)->value; list = list->next)
        continue;
    elm->next = list->next;
    list->next = elm;
}

/*==================== Add a word in the Pgraph ===========================*/

Node* add_letter(Pgraph *G, char c, Node *node)
{
    Node *n = init_node(c, 0, -1, node->self, NULL);
    n->adjlists = init_adjlists(-1);
    List *l = init_list(n);
    int index = list_append(G->nodes, l);
    n->self = index;
    Adjlists *adjl = init_adjlists(index);
    if (node->adjlists == NULL)
    {
	node->adjlists = init_adjlists(-1);
	node->adjlists->next = adjl;
    }
    else
    {
	adjlists_insert(G, node->adjlists, adjl, c);
    }

    return n;
}

void add_word(Pgraph *G, char *word, size_t size)
{
    size_t pos = 0;
    int exist = 1;
    Node *node = G->nodes->node;
    Adjlists *p;
    while (node->adjlists && exist != 0 && pos < size)
    {
	for (p = node->adjlists->next; p; p = p->next)
	{
	    Node *n = get_node(G, p->index);
	    if (word[pos] < n->value)
	    {
		exist = 0;
	        break;
	    }
	    else if (word[pos] == n->value)
	    {
                pos++;
		node = n;
		break;
	    }
	}
	if (p == NULL)
            exist = 0;
    }
    
    if (pos == size)
        node->is_end = 1;
    else
    {
	while (pos < size)
	{
	    node = add_letter(G, word[pos], node);
	    pos++;
	}
	node->is_end = 1;
    } 
}

Pgraph *create_Pgraph_with_dir(char *cur_dir)
{
    Pgraph *G = init_Pgraph();
    struct dirent *dir;

    int len;
    DIR *d = opendir(cur_dir);

    if (d)
    {
	while ((dir = readdir(d)) != NULL)
	{
	    len = strlen(dir->d_name);
	    add_word(G, dir->d_name, len);
	    G->order += len;
	    if (G->longest < len)
		G->longest = len;
	}
	closedir(d);
    }

    free(dir);
    return G;
}

/*==================== Get a word ===============================*/
//Get a word from a char and the position of the
//cursor of the graph

Node* update_pos(Pgraph *G, char c, Node *node)
{
    if (node->adjlists != NULL)
    {
        for (Adjlists *p = node->adjlists->next; p; p = p->next)
        {
	    Node *n = get_node(G, p->index);
	    if (c < n->value)
	        return NULL;
	    else if (c == n->value)
	    {
	        G->cur_pos = p->index;
	        return n;
	    }
        }
    }

    return NULL;
}

void set_pos_to_father(Pgraph *G, Node *node)
{
    G->cur_pos = node->father;
}

char* __get_word(Pgraph *G, char* s)
{
    int set_fat = 1;
    if (G->over_write > 0)
    {
	G->over_write -= 1;
	if (G->over_write == 0)
	    set_fat = 0;
    }
    if (G->over_write == 0 && G->cur_pos != 0)
    {
	int i = 0;
        Node *node = get_node(G, G->cur_pos);
	if (set_fat == 1)
	    set_pos_to_father(G, node);
	if (node->adjlists)
        {
            while (!node->is_end && !node->adjlists->next->next)
            {
                s[i] = node->value;
                node = get_node(G, node->adjlists->next->index);
                i++;
            }

            s[i] = node->value;
	    if (node->is_end)
	    {
		s[i + 1] = ' ';
		i++;
	    }
            s[i + 1] = 0;
            return s;
        }
    }
    s[0] = 0;
    return s;
}

//if 'c' = '\0' means back (a letter was erased)
//else a new letter is entered by the user
char* get_word(Pgraph *G, char c)
{
    char *s = malloc((G->longest + 2) * sizeof(char));
    if (c != '\0' && G->over_write == 0)
    {
    	Node *node = get_node(G, G->cur_pos);
    	node = update_pos(G, c, node);
    	int i = 0;
    	if (node && node->adjlists)
    	{
	    node = get_node(G, node->adjlists->next->index);
            while (!node->is_end && !node->adjlists->next->next)
    	    {
	        s[i] = node->value;
	        node = get_node(G, node->adjlists->next->index);
	        i++;
            }

	    s[i] = node->value;
	    if (node->is_end)
            {
                s[i + 1] = ' ';
                i++;
            }
	    s[i + 1] = 0;
	    return s;
        }
	G->over_write += 1;
	s[0] = 0;
        return s;
    }
    else
	return __get_word(G, s);

}
