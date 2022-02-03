#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

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
    if (adjl == NULL) errx(1, "fail malloc adjlists");
    adjl->next = NULL;
    adjl->index = i;
    return adjl;
}

Node *init_node(char v, int end, int self, int father, Adjlists *adjl)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL) errx(1, "fail malloc node");
    node->value = v;
    node->is_end = end;
    node->self = self;
    node->father = father;
    node->adjlists = adjl;
    return node;
}

List *init_list(Node *node)
{
    //printf("OKOKOK\n");
    List *l = malloc(sizeof(List));
    //printf("OKOKOK\n");
    if (l == NULL) errx(1, "fail malloc list");
    //printf("OKOKOK\n");
    l->next = NULL;
    l->node = node;
    return l;
}

Word *init_word()
{
    Word *w = malloc(sizeof(Word));
    if (w == NULL) errx(1, "fail malloc word");
    w->w = NULL;
    w->index = 0;
    return w;
}

Pgraph *init_Pgraph()
{
    Pgraph *G = malloc(sizeof(Pgraph));
    if (G == NULL) errx(1, "fail malloc graph");
    //printf("OKOK1\n");
    Node *node = init_node('$', 0, 0, -1, NULL);
    //printf("OKOK2\n");
    G->nodes = init_list(node);
    //printf("OKOK3\n");
    G->order = 1;
    //printf("OKOK4\n");
    G->cur_pos = 0;
    //printf("OKOK5\n");
    G->longest = 0;
    //printf("OKOK6\n");
    G->over_write = 0;
    //printf("OKOK7\n");
    G->last_pos = 0;
    //printf("OKOK8\n");
    G->word = init_word();
    //printf("OKOK9\n");
    return G;
}

LPgraph *init_LPgraph(Pgraph *G)
{
    LPgraph *LG = malloc(sizeof(LPgraph));
    LG->next = NULL;
    LG->prev = NULL;
    LG->G = G;
    return LG;
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

void free_word(Word *wrd)
{
    free(wrd->w);
    free(wrd);
}

void free_Pgraph(Pgraph *G)
{
    free_list(G->nodes);
    free_word(G->word);
    free(G);
}

void free_LPgraph(LPgraph *LG)
{
    LPgraph *p;
    while (LG != NULL)
    {
	p = LG->next;
	free_Pgraph(LG->G);
	free(LG);
	LG = p;
    }
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

void LPgraph_append(LPgraph *LG, Pgraph *G)
{
    LPgraph *elm = init_LPgraph(G);
    for (; LG->next; LG = LG->next)
	continue;
    elm->next = LG->next;
    LG->next = elm;
    elm->prev = LG;
}

void LPgraph_pop(LPgraph *LG)
{
    for (; LG->next; LG = LG->next)
	continue;
    LG->prev->next = LG->next;
    LG->prev = NULL;
    LG->next = NULL;
    free_LPgraph(LG);
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
    printf("OKOK\n");
    Pgraph *G = init_Pgraph();
    printf("OKOK\n");
    struct dirent *dir;
    // struct stat buf;

    int len;
    DIR *d = opendir(cur_dir);

    if (d)
    {
	while ((dir = readdir(d)) != NULL)
	{
	    len = strlen(dir->d_name) + 2;
	    char *s = malloc(len * sizeof(char));
	    //if (s == NULL) errx(1, "fail malloc");
	    strcpy(s, dir->d_name);
	    
	    /*if (stat(dir->d_name, &buf) == -1)
	    {
		perror("Error type");
                errx(1, "%s: Not a file nor a directory", dir->d_name);
	    }
	    if (S_ISDIR(buf.st_mode))
		strcat(s, "/");
	    else
	        strcat(s, " ");*/
	    DIR *tmp;
	    if ((tmp = opendir(dir->d_name)) == NULL)
	    {
		//printf("%s\n", dir->d_name);
		strcat(s, " ");
	    }
	    else
	    {
		strcat(s, "/");
		closedir(tmp);
	    }
	    
            add_word(G, s, len);
	    G->order += len;
	    if (G->longest < len)
		G->longest = len;
	    free(s);
	}
	G->word->w = malloc((G->longest + 2) * sizeof(char));
	//if (G->word->w == NULL) errx(1, "fail malloc");
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
    if (G->cur_pos != 0)
    	G->cur_pos = node->father;
}

char *min_word(Pgraph *G, Node *node, char *s)
{
    int i = 0;
    while (!node->is_end && !node->adjlists->next->next)
    {
        s[i] = node->value;
        node = get_node(G, node->adjlists->next->index);
        i++;
    }

    if (node->adjlists->next && node->adjlists->next->next)
    {
	s[i] = node->value;
	G->last_pos = node->self;
	s[i + 1] = 0;
    }
    else
    {
        s[i] = 0;
        G->last_pos = node->father;
    }
    return s;
}

char* __get_word(Pgraph *G, char* s)
{
    int set_fat = 1;
    if (G->over_write > 0)
    {
	G->over_write -= 1;
	set_fat = 0;
    }
    else if (G->word->index > 0)
    {
	G->word->index--;
	G->word->w[G->word->index] = 0;
    }
    
    Node *node = get_node(G, G->cur_pos);
    if (set_fat == 1)
    {
        set_pos_to_father(G, node);
    }
    else
        node = get_node(G, node->adjlists->next->index);
    
    if (G->over_write == 0 && G->cur_pos != 0)
	if (node->adjlists)
	    return min_word(G, node, s);

    s[0] = 0;
    return s;
}

//if 'c' = '\0' means back (a letter was erased)
//else a new letter is entered by the user
char* get_word(Pgraph *G, char c)
{
    char *s = malloc((G->longest + 2) * sizeof(char));
    if (c != '\0')
    {
	if (G->over_write == 0)
	{
    	    Node *node = get_node(G, G->cur_pos);
    	    node = update_pos(G, c, node);
    	    if (node && node->adjlists)
    	    {
	        node = get_node(G, node->adjlists->next->index);
		G->word->w[G->word->index] = c;
		//printf("%i, %c\n", node->self, node->value);
		//printf("%i\n", G->cur_pos);
		G->word->index++;
		G->word->w[G->word->index] = 0;
		return min_word(G, node, s);
            }
	}
	G->over_write += 1;
	s[0] = 0;
        return s;
    }
    else
	return __get_word(G, s);
}
