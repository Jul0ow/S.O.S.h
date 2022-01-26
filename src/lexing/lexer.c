#include "list.h"
#include "vector.h"
#include "lexer.h"
#incude "xmalloc.h"

list* lexing(char *entry)
{
    list *tokenized = xmalloc(sizeof(list));
    list_init(tokenized);

    char *p = entry;

    while(*p != '\0')
    {
        token new = xmalloc(sizeof(token));

    }
}
