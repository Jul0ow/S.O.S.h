#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

#define SIZE 256

char* get_dir()
{
    char *s = malloc(SIZE * sizeof(char));
    while (getcwd(s, SIZE) == NULL)
        if ((s = realloc(s, SIZE + SIZE)) == NULL) errx(1, "realloc");
    return s;
}

void file_type(char *filename)
{
    struct stat buf;
    if (stat(filename, &buf) == -1)
        errx(1, "Problem");
    if (!S_ISDIR(buf.st_mode))
        errx(1, "cd: %s: Not a directory\n", filename);
}

int main(int argc, char** argv)
{
    if (argc > 2)
	errx(1, "Too many arguments");
    else if (argc == 2)
    {
	file_type(argv[1]);
	if (chdir(argv[1]) == -1)
	    errx(1, "fail");
	char* s = get_dir();
	printf("%s\n", s);
	free(s);
    }
    else
	printf("nothing happens\n");
    return 0;
}
