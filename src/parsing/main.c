#include "ast.h"
#include "lexer.h"
#include "listT.h"
#include "xmalloc.h"
#include "ast_print.h"

int main()
{
    struct listT l;
    list_initT(&l);

    //init tokens
    token c1 = {
        COMMAND,
        "cat",
        5
    };

    list_push_endT(&l, &c1); //cat

    ast_node *ast = creating_ast(&l);

    int err = print_ast(&ast);

    freeL(&l);
}
