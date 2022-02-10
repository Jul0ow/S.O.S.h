#ifndef MY_PARSE_H
#define MY_PARSE_H

typedef struct Arguments
{
    char* command;
    char** args;
    size_t len;
}Args;

Args* init_args(char *buf);

void free_args(Args* args);

int valid_command(char *cmd);

char* my_str_split(char* buf, char delim, size_t* i);

#endif
