#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "my_parse.h"

#define SIZE 256

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
    size_t len = strlen(buf);
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

int valid_command(char *cmd)
{
    if (strcmp(cmd, "cat") == 0
	|| strcmp(cmd, "ls") == 0
	|| strcmp(cmd, "pwd") == 0)
    {
	return 1;
    }

    return 0;
}

/*
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

    for (size_t j = 0; j < args->len - 1; j++)
	printf("%s\n", args->args[j]);
    if (args->args[args->len - 1] == NULL)
	printf("NULL\n");

    if (!valid_command(args->args[0]))
	errx(1, "Not a command");

    char* s = malloc((strlen(args->args[0]) + 9) * sizeof(char));
    strcpy(s, "command/");
    strcat(s, args->args[0]);
    execv(s, args->args);

    //free_args(args);
}

int main()
{
    char buf[SIZE];
    read(STDIN_FILENO, buf, SIZE);
    my_parse(buf);
    return 0;
}
*/
