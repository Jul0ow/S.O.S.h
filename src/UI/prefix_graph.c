#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "prefix_graph.h"

void print_words(Pgraph *G);
void LW_append(LW *lw, char *s);

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

LW *init_LW(char *w)
{
    LW *lw = malloc(sizeof(LW));
    lw->next = NULL;
    lw->prev = NULL;
    lw->tail = lw;
    lw->word = w;
    lw->is_dir = 0;
    return lw;
}

Pgraphs *init_Pgraphs(Pgraph *G)
{
    Pgraphs *Gs = malloc(sizeof(Pgraphs));
    Gs->G = G;
    Gs->v = vector_new();
    Gs->lw = init_LW(NULL);
    char *s = malloc(3 * sizeof(char));
    strcpy(s, "./");
    LW_append(Gs->lw, s);
    return Gs;
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

void free_LW(LW* lw)
{
    LW *p;
    while (lw != NULL)
    {
        p = lw->next;
        free(lw->word);
	//free(lw->tail);
        free(lw);
        lw = p;
    }
}

void free_Pgraphs(Pgraphs *Gs)
{
    free_Pgraph(Gs->G);
    vector_free(Gs->v);
    free_LW(Gs->lw);
    free(Gs);
}

/*==================== Utils ===============================*/

void print_LW(LW *lw)
{
    for (LW *p = lw->next; p; p = p->next)
    {
	printf("%s\n", p->word);
    }
}

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

void reinit_word(Pgraph *G)
{
    char *s = malloc(G->longest * sizeof(char));
    Node *node = get_node(G, G->cur_pos);
    int i = 0;
    while (node->father != -1)
    {
	s[i] = node->value;
	i++;
	node = get_node(G, node->father);
    }
    
    //reverse
    i--;
    int j = 0;
    while (i > 0)
    {
	G->word->w[j] = s[i];
	j++;
	i--;
    }

    G->word->w[j] = 0;
    G->word->index = j;
    free(s);
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

void LW_append(LW *lw, char *w)
{
    LW *elm = init_LW(w);
    for (; lw->next; lw = lw->next)
	lw->tail = elm;
    elm->prev = lw;
    elm->next = lw->next;
    lw->next = elm;
    lw->tail = elm;
}

void LW_pop(LW *lw)
{
    if (lw->tail->prev)
    {
	LW *p = lw->tail;
        lw->tail->prev->next = NULL;
	for (;lw; lw = lw->next)
	    lw->tail = p->prev;
	free_LW(p);
    }
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
    struct stat buf;

    int len;
    DIR *d = opendir(cur_dir);

    if (d)
    {
	int dir_len = strlen(cur_dir);
	while ((dir = readdir(d)) != NULL)
	{
	    len = strlen(dir->d_name) + 2;
            char *path = malloc((dir_len + len - 1) * sizeof(char));
	    char *s = malloc(len * sizeof(char));
	    strcpy(path, cur_dir);
	    strcat(path, dir->d_name);
	    strcpy(s, dir->d_name);
	  
	    if (stat(path, &buf) == -1)
	    {
		perror("Error type");
                errx(1, "%s: Not a file nor a directory", dir->d_name);
	    }
	    if (S_ISDIR(buf.st_mode))
		strcat(s, "/");
	    else
	        strcat(s, " ");
	    
            add_word(G, s, len);
	    G->order += len;
	    if (G->longest < len)
		G->longest = len;
	    free(path);
	    free(s);
	}
	G->word->w = malloc((G->longest + 2) * sizeof(char));
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
