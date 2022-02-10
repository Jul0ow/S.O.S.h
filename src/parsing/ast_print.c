#include <stdio.h>
#include "ast.h"

int printast(ast_node* ast)
{
    size_t i = 0;
    int err = __printast(ast,i);
    return err;
}

int __printast(*ast_node ast,size_t i)
{
    printf("|");
    for(size_t j=0;j<i;j++)
    {
	printf("_");
    }
    printf("%s\n",ast->expression);
    int err = 0;
    if(ast->child!=NULL)
    {
	err = __printast(ast->child,i+1);
    }
    if(err != 0)
	return err;
    if(ast->sibling != NULL)
    {
	err = __printast(ast->sibling,i);
    }
    return err;
}
