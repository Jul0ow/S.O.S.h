#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

#define SIZE 264

int main(int argc, char** argv)
{
    if (argc)
    struct dirent *dir;

    char *s = malloc(SIZE * sizeof(char));
    while (getcwd(s, SIZE) == NULL)
        if ((s realloc(s, SIZE + SIZE)) == NULL) errx(1, "realloc");
    
    DIR *d = opendir(s); 
    if (d)
    {
	int count = 0;
        while ((dir = readdir(d)) != NULL)
        {
	    if (dir->d_name[0] != '.')
	    {
                printf("%s   ", dir->d_name);
		count++;
	    }
	    if (count >= 10)
	    {
		count = 0;
		printf("\n");
	    }
        }
	printf("\n");
        closedir(d);
    }

    free(dir);
    free(s);

    return 0;
}
