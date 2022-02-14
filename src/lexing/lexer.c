#include "listT.h"
#include "vectorS.h"
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
listT* init_lexing(char *entry)
{
    listT *token_list = xmalloc(sizeof(listT));
    list_initT(token_list);

    lexing(entry, token_list, '\0');

    return token_list;
}

//First function to be called, only once and always the function
//can be called in ` quote
size_t read_word(char *p, int *isCommand, listT *token_list) 
{
    if (isSeparator(*p))
    {
        int res = read_separator(p, token_list, isCommand);
        if (res)
            return token_list->tail->token->len;
        else //only space
            return 1;
        
    }
    else
    {
        //initializing the vector containing the string and its length
        vectorS *v = vector_newS();
        //initializing the token that will be returned
        token *new = xmalloc(sizeof(token));

        if(*isCommand)
            new->type = COMMAND;
        else
            new->type = ARGUMENT;

        while (!isSeparator(*p))
            vector_pushS(v, *(p++));

        vector_pushS(v, '\0');

        new->string = xmalloc(v->size*sizeof(char));

        strcpy(new->string, v->string);

        new->len = v->size-1;

        vector_freeS(v);

        list_push_endT(token_list, new);
        
        *isCommand = FALSE;
        
        return new->len;
    }
}

int read_separator(char *p, listT *token_list, int *isCommand)
{
    token *t = xmalloc(sizeof(token));
    t->len = (size_t)1;
    if (*p != ' ')
        *isCommand = FALSE;
    switch (*p)
    {
        case '&':
            *isCommand = TRUE;
            if (*(p+1) == '&')
            {
                t->len = 2;
                t->type = AND_BOOL;
            }
            else
                t->type = AND;
            break;

        case '|':
            *isCommand = TRUE;
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
            *isCommand = TRUE;
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
    list_push_endT(token_list, t);
    return TRUE;
}

void lexing(char *entry, listT* token_list, char end)
{

    char *p = entry;
   
    int isCommand = TRUE;

    //if we are not bewteen simple or double quotes we do not use space
    if (end != '\'' && end != '"') 
        while (*p != end && *p == ' ')
            p++;

    if(*p == end)
        return;

    read_word(p, &isCommand, token_list);
    p += token_list->tail->token->len;


    while (*p != end)
    {
        p += read_word(p, &isCommand, token_list);
    }
}
