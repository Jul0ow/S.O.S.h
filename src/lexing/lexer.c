#include "list.h"
#include "vector.h"
#include "lexer.h"
#include "xmalloc.h"
#include <string.h>
#define NB_SEPARATOR 15

//Array containing all separator that you can find everywhere
//(for example whitout - because it is a separator only at the beginning of a word)
const char separator[] = {
    ' ', '|', ';','>',
    '<','\t', '\n', '\'',
    '"','`', '(',')',
    '&','\0','#'
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

    return token_list;
}

//First function to be called, only once and always the function
//can be called in ` quote
size_t read_word(char *p, int isCommand, list *token_list) 
{
    if (isSeparator(*p))
    {
        int res = read_separator(p, token_list);
        if (res)
            return token_list->tail->token->len;
        else
            return 1;
        
    }
    else
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
            vector_push(v, *(p++));

        vector_push(v, '\0');

        new->string = xmalloc(v->size*sizeof(char));

        strcpy(new->string, v->string);

        new->len = v->size-1;

        vector_free(v);

        list_push_end(token_list, new);
        
        return new->len;
    }
}

int read_separator(char *p, list *token_list)
{
    token *t = xmalloc(sizeof(token));
    t->len = (size_t)1;
    switch (*p)
    {
        case '&':
            if (*(p+1) == '&')
            {
                t->len = 2;
                t->type = AND_BOOL;
            }
            else
                t->type = AND;
            break;

        case '|':
            if (*(p+1) == '|')
            {
                t->len = 2;
                t->type = OR_BOOL;
            }
            else
                t->type = PIPE;
            break;

        case '<':
            if(*(p+1) == '<')
            {
                t->type = DLEFT_CHEVRON;
                t->len = 2;
            }
            else
                t->type = LEFT_CHEVRON;
            break;

        case '>':
            if(*(p+1) == '>')
            {
                t->type = DRIGHT_CHEVRON;
                t->len = 2;
            }
            else
                t->type = RIGHT_CHEVRON;
            break;

        case '#':
            t->type = COMMENT;
            size_t add = 0;
            while (*(p + add) != '\0' && *(p + add) != '\n')
            {
                add++;
            }
            t->len = add;

            break;

        case ';':
            t->type = SEMI_COLON;
            break;

        case '(':
            t->type = LEFT_PAREN;
            break;

        case ')':
            t->type = RIGHT_PAREN;
            break;
        case '\'':
            t->type = QUOTE;
            break;

        case '\"':
            t->type = DOUBLE_QUOTES;
            break;

        case '`':
            t->type = BACKTICK;
            break;

        default:
            t->type = UNKNOWN; //or space
            free(t);
            return FALSE;
    }
    list_push_end(token_list, t);
    return TRUE;
}

void lexing(char *entry, list* token_list, char end)
{

    char *p = entry;

    //if we are not bewteen simple or double quotes we do not use space
    if (end != '\'' && end != '"') 
        while (*p != end && *p == ' ')
            p++;

    if(*p == end)
        return;

    read_word(p, TRUE, token_list);
    p += token_list->tail->token->len;


    while (*p != end)
    {
        p += read_word(p, FALSE, token_list);
    }
}
