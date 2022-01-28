#include "list.h"
#include "vector.h"
#include "lexer.h"
#include "xmalloc.h"
#include <string.h>
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
    list_init(token_list);

    lexing(entry, token_list, '\0');

    return list;
}

//First function to be called, only once and always the function
//can be called in ` quote
token* read_word(char *p, int isCommand, list *token_list) 
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

    new->string = xmalloc(v->size*sizeof(char));

    strcpy(new->string, v->string);
    
    new->len = v->size;

    vector_free(v);

    list_push_end(token_list, new);
    return new;
}

token* read_separator(char *p, list *token_list)
{
    token *new = xmalloc(sizeof(token));

    switch (*p)
    {
        case '&':
            if (*(p+1) == '&')
                token->type = AND_BOOL;
            else
                token->type = AND;
            break;

        case '|':
            if (*(p+1) == '|')
                token->type = OR_BOOL;
            else
                token->type = PIPE;
            break;

        case '<':
            if(*(p+1) == '<')
                token->type = DLEFT_CHEVRON;
            else
                token->type = LEFT_CHEVRON;
            break;

        case '>':
            if(*(p+1) == '>')
                token->type = DRIGHT_CHEVRON;
            else
                token->type = RIGHT_CHEVRON;
            break;
        
        case '#':
            token->type = COMMENT;
            break;

        case ';':
            token->type = SEMI_COLON;
            break;

        case '(':
            token->type = LEFT_PAREN;
            break;

        case ')':
            token->type = RIGHT_PAREN;
            
        case '\'':
            token->type = QUOTE;
            break;
        
        case '\"':
            token->type = DOUBLE_QUOTES;
            break;

        case '`':
            token->type = BACKTICK;
            break;

        default:
            token->type = UNKNOWN;
            break;
    }
    if (token->type == COMMENT)
        while(*p != '\0' && *p != '\n')
            p++;
    p++;
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

    read_word(p, TRUE, token_list);

    while (*p != end)
    {
        read_word(p, FALSE, token_list);
    }
}
