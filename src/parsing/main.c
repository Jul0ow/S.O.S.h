#include "ast.h"
#include "lexer.h"
#include "listT.h"
#include "xmalloc.h"
#include "ast_print.h"
#include <stdio.h>

int __ast_print(ast_node* ast,int i)
{
    printf("|");
    for(int j=0;j<i;j++)
    {
	printf("_");
    }
    printf("%s\n",ast->string);
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


int main()
{
    struct listT* l = malloc(sizeof(listT));
    list_initT(l);

    //init tokens
    token* c1 = malloc(sizeof(token));
    char* str = malloc(sizeof(char)*4);
    str[0]='c';
    str[1]='a';
    str[2]='t';
    str[3]='\0';
    c1->type=COMMAND;
    c1->string = str;
    c1->len = 3;


    list_push_endT(l, c1); //cat

    ast_node* node= creating_ast(l);
    int err = ast_print(node);
    
    free_ast(node);
    freeL(l);
    return err;
}
