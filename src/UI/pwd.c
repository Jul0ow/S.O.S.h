#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

#define SIZE 264

int main(void)
{
    char *s = malloc(SIZE * sizeof(char));
    while (getcwd(s, SIZE) == NULL)
        if (realloc(s, SIZE + SIZE) == NULL) errx(1, "realloc");
    printf("%s\n", s);
    free(s);
    return 0;
}
