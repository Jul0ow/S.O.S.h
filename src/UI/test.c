#include <stdio.h>
#include <unistd.h>
#include <err.h>

#include "prefix_graph.h"

#define SIZE 256

int main()
{
    //Pgraph *G = create_Pgraph_with_dir(".");
    /*print_words(G);
    
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
    free(w);*/

    /*
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

    printf("%s\n", G->word->w);*/

    //LPgraph *LG = init_LPgraph(G);
    //Pgraph *G1 = create_Pgraph_with_dir("../../../S.O.S.h/src/UI/");
    Pgraph *G2 = create_Pgraph_with_dir("command/./");
    //Pgraph *G3 = create_Pgraph_with_dir("data");
    print_words(G2);
    free_Pgraph(G2);
    /*LPgraph_append(LG, G1);
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
    }*/

    //free_LPgraph(LG);
    return 0;
}
