#include <stdio.h>
#include "ast.h"
#include "ast_print.h"

int __ast_print(ast_node* ast,int i)
{
    printf("|");
    for(int j=0;j<i;j++)
    {
	printf("_");
    }
    printf("%s\n",(char*)ast->string);
    int err = 0;
    if(ast->child!=NULL)
    {
	err = __ast_print(ast->child,i+1);
    }
    if(err != 0)
	return err;
    if(ast->sibling != NULL)
    {
	err = __ast_print(ast->sibling,i);
    }
    return err;
}

int ast_print(ast_node* ast)
{
    int i = 0;
    int err = __ast_print(ast,i);
    return err;
}


