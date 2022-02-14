#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"
#include <string.h>
#include "vectorS.h"
#include "listT.h"

void print_token(token *t)
{
    switch (t->type)
    {
        case AND_BOOL:
            printf(" -> && (AND_BOOL)");
            break;

        case AND:
            printf(" -> & (AND)");
            break;

        case OR_BOOL:
            printf(" -> || (OR_BOOL)");
            break;

        case PIPE:
            printf(" -> | (PIPE)");
            break;

        case DLEFT_CHEVRON:
            printf(" -> << (DLEFT_CHEVRON)");
            break;

        case LEFT_CHEVRON:
            printf(" -> < (LEFT_CHEVRON)");
            break;

        case DRIGHT_CHEVRON:
            printf(" -> >> (DRIGHT_CHEVRON)");
            break;

        case RIGHT_CHEVRON:
            printf(" -> > (RIGHT_CHEVRON)");
            break;

        case COMMENT:
            printf(" -> #--- (COMMENT)");
            break;

        case SEMI_COLON:
            printf(" -> ; (SEMI_COLON)");
            break;

        case LEFT_PAREN:
            printf(" -> ( (LEFT_PAREN)");
            break;

        case RIGHT_PAREN:
            printf(" -> ) (RIGHT_PAREN)");
            break;

        case QUOTE:
            printf(" -> \' (QUOTE)");
            break;

        case DOUBLE_QUOTES:
            printf(" -> \" (DOUBLE_QUOTES)");
            break;

        case BACKTICK:
            printf(" -> ` (BACKTICK)");
            break;

        case COMMAND:
            printf(" -> %s (COMMAND)", t->string);
            break;

        case ARGUMENT:
            printf(" -> %s (ARGUMENT)", t->string);
            break;

        default:
            break;
    }
    

}

void print_list(listT *l)
{
    list_elm *elm = l->head;
    printf("Printing token ...\n");
    printf("----------------------------------------------------------\n");

    printf("head");
    while (elm != NULL)
    {
        print_token(elm->token);
        elm = elm->next;
    }

    printf(" -> tail\n");

    printf("----------------------------------------------------------\n");
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Error : nothing to lex\n");
        return 1;
    }

    vectorS *v = vector_newS();

    for (size_t i = 0; i < strlen(argv[1]); i++)
        vector_pushS(v, argv[1][i]);

    for (int i = 2; i < argc; i++)
    {
        vector_pushS(v, ' ');

        for (size_t j = 0; j < strlen(argv[i]); j++)
            vector_pushS(v, argv[i][j]);
    }

    vector_pushS(v, '\0');

    listT *token_list = init_lexing(v->string);

    print_list(token_list);

    vector_freeS(v);
    freeL(token_list);
    return 0;
}
