#include "list.h"
#include "vector.h"
#include "lexer.h"
#include "xmalloc.h"
#define NB_SEPARATOR 13

//Array containing all separator that you can find everywhere
//(for example whitout - because it is a separator only at the beginning of a word)
const char separator[] = {
    ' ', '|', ';','>',
    '<','\t', '\n', '\'',
    '"','`', '(',')',
    '&','\0'
};

int isSeparator(char c)
{
    for (int i = 0; i<NB_SEPARATOR; i++)
        if (c == separator[i])
            return TRUE;

    return FALSE;
}


//We split the initializing and the lexing to be able to apply recursively the second one
//In a way to deal with quote
list* init_lexing(char *entry)
{
    list *token_list = xmalloc(sizeof(list));
    list_init(tokenized);

    lexing(entry, token_list, '\0');

    return list;
}

//First function to be called, only once and always the function
//can be called in ` quote
token* read_word(char *p, int isCommand) 
{
    //initializing the vector containing the string and its length
    vector *v = vector_new();
    //initializing the token that will be returned
    token *new = xmalloc(sizeof(token));

    if(isCommand)
        new->type = COMMAND;
    else
        new->type = ARGUMENT;

    while (!isSeparator(*p))
        vector_push(v, p);

    vector_push(v, '\0')

        new->string = v->string;
    new->len = v->size;

    vector_free(v);
    return new;
}

void lexing(char *entry, list* token_list, char end)
{

    char *p = entry;

    //if we are not bewteen simple or double quotes we do not use space
    if (end != '\'' && end != '"') 
        while (*p != end)
            p++;

    if(*p == end)
        break;

    token *new = read_word(p, TRUE);

    


}
