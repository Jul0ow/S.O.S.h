#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

//#include "prefix_graph.h"

#define SIZE 256

/*
void test_prefix_tree()
{
    //Pgraph *G = create_Pgraph_with_dir(".");
    print_words(G);
    
    char *w = get_word(G, 'l');
    printf("\n");
    printf("%s\n", w);
    free(w);
    
    G->cur_pos = 0;
    w = get_word(G, 'M');
    printf("%s\n", w);
    free(w);
    
    G->cur_pos = 3;
    w = get_word(G, 'e');
    printf("%s\n", w);
    free(w);

    G->cur_pos = 3;
    w = get_word(G, '\0');
    printf("%s\n", w);
    free(w);

    w = get_word(G, '\0');
    printf("%s\n", w);
    free(w);

    
    char *w = get_word(G, 'M');
    free(w);
    w = get_word(G, 'a');
    free(w);
    w = get_word(G, 'k');
    free(w);
    w = get_word(G, 'e');
    free(w);
    w = get_word(G, 'f');
    free(w);
    w = get_word(G, 'i');
    free(w);
    w = get_word(G, 'l');
    free(w);
    w = get_word(G, 'e');
    free(w);

    printf("%s\n", G->word->w);

    //LPgraph *LG = init_LPgraph(G);
    //Pgraph *G1 = create_Pgraph_with_dir("../../../S.O.S.h/src/UI/");
    Pgraph *G2 = create_Pgraph_with_dir("command/./");
    //Pgraph *G3 = create_Pgraph_with_dir("data");
    print_words(G2);
    free_Pgraph(G2);
    LPgraph_append(LG, G1);
    LPgraph_append(LG, G2);
    LPgraph_append(LG, G3);

    for (LPgraph *l = LG; l; l = l->next)
    {
	printf("=======================\n");
	print_words(l->G);
    }

    LPgraph_pop(LG);

    for (LPgraph *l = LG; l; l = l->next)
    {
        printf("=======================\n");
        print_words(l->G);
    }

    //free_LPgraph(LG);
}
*/

typedef struct Arguments
{
    char* command;
    char** args;
    size_t len;
}Args;

Args* init_args(char *buf)
{
    Args* args = malloc(sizeof(Args));
    args->command = NULL;

    size_t nb_args = 0;
    for (size_t i = 0; buf[i] != 0; i++)
    {
	if (buf[i] == ' ')
	{
	    if (buf[i + 1] != ' ' && buf[i + 1] != '\n')
	        nb_args++;
	}
    }

    nb_args += 2;
    args->args = malloc(nb_args * sizeof(char *));
    args->len = nb_args;
    return args;
}

void free_args(Args* args)
{
    free(args->command);
    for (size_t i = 0; i < args->len; i++)
    {
	free(args->args[i]);
    }
    free(args->args);
    free(args);
}

char* my_str_split(char* buf, char delim, size_t* i)
{
    size_t len = strlen(buf) - 1;
    while (*i < len && buf[*i] == delim)
	*i += 1;

    if (*i == len)
        return NULL;
    
    char* res = malloc((len + 1) * sizeof(char));

    size_t pos = *i;
    for (; *i < len && buf[*i] != delim; *i += 1)
    {
	res[*i - pos] = buf[*i];
    }

    res[*i - pos] = 0;
    if (buf[*i] == delim) 
	*i += 1;
    return res;
}

void my_parse(char *buf)
{
    Args *args = init_args(buf);
    size_t i = 0;
    size_t len = 0;

    char *ptr;
    while ((ptr = my_str_split(buf, ' ', &i)) != NULL && len < args->len)
    {
	args->args[len] = ptr;
	len++;
    }

    args->args[args->len - 1] = (char *)NULL;

    /*
    for (size_t j = 0; j < args->len - 1; j++)
	printf("%s\n", args->args[j]);
    if (args->args[args->len - 1] == NULL)
	printf("NULL\n");
    */

    char* s = malloc((strlen(args->args[0]) + 9) * sizeof(char));
    strcpy(s, "command/");
    strcat(s, args->args[0]);
    execv(s, args->args);

    free_args(args);
}

int main()
{
    char buf[SIZE];
    read(STDIN_FILENO, buf, SIZE);
    my_parse(buf);
    return 0;
}
