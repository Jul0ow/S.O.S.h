#include <stdio.h>
#include <unistd.h>
#include <err.h>

#include "prefix_graph.h"

#define SIZE 256

int main()
{
    char *s = malloc(SIZE * sizeof(char));
    while (getcwd(s, SIZE) == NULL)
        if (realloc(s, SIZE + SIZE) == NULL) errx(1, "realloc");
    Pgraph *G = create_Pgraph_with_dir(s);
    print_words(G);
    
    char *w = get_word(G, 'l');
    printf("\n");
    printf("%s\n", w);
    free(w);
    
    G->cur_pos = 0;
    w = get_word(G, 'p');
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

    free_Pgraph(G);
    free(s);
    return 0;
}
