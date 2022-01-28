#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFF_SIZE 264

void file_type(char *filename)
{
    struct stat buf;
    if (stat(filename, &buf) == -1)
        errx(1, "Problem");
    if (S_ISDIR(buf.st_mode))
        printf("cat: %s: Is a directory\n", filename);
}

void my_cat(char filename[])
{
    file_type(filename);
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        errx(1, "Could not open the file %s", filename);

    char c = fgetc(file);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(file);
    }

    fclose(file);
}

int main(int argc, char **argv)
{
    int i;
    for (i = 0; i < argc - 1; i++)
    {
        my_cat(argv[i + 1]);
    }

    return 0;
}
