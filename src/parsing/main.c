#include "ast.h"
#include "../lexing/lexer.h"
#include "../utils/list.h"
#include "xmalloc.h"
#include "print_ast.h"

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

    int err = printast(&ast);

    freeL(&l);
}
