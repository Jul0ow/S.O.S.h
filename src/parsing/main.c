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

    //cat (COMMAND)
    token* c1 = malloc(sizeof(token));
    char* str = malloc(sizeof(char)*4);
    str[0]='c';
    str[1]='a';
    str[2]='t';
    str[3]='\0';
    c1->type=COMMAND;
    c1->string = str;
    c1->len = 3;

    //-japon (ARGUMENT)
    token* c2 = malloc(sizeof(token));
    char* str2 = malloc(sizeof(char)*7);
    str2[0]='-';
    str2[1]='j';
    str2[2]='a';
    str2[3]='p';
    str2[4]='o';
    str2[5]='n';
    str2[6]='\0';
    c2->type=ARGUMENT;
    c2->string = str2;
    c2->len = 6;


    //main.c (ARGUMENT)
    token* c3 = malloc(sizeof(token));
    char* str3 = malloc(sizeof(char)*7);
    str3[0]='m';
    str3[1]='a';
    str3[2]='i';
    str3[3]='n';
    str3[4]='.';
    str3[5]='c';
    str3[6]='\0';
    c3->type=ARGUMENT;
    c3->string = str3;
    c3->len = 6;

    //&& (AND_BOOL)
    token* c4 = malloc(sizeof(token));
    char* str4 = malloc(sizeof(char)*3);
    str4[0]='&';
    str4[1]='&';
    str4[2]='\0';
    c4->type=AND_BOOL;
    c4->string = str4;
    c4->len = 2;

    //ls (COMMAND)
    token* c5 = malloc(sizeof(token));
    char* str5 = malloc(sizeof(char)*3);
    str5[0]='l';
    str5[1]='s';
    str5[2]='\0';
    c5->type=COMMAND;
    c5->string = str5;
    c5->len = 2;

    //|| (OR_BOOL)
    token* c6 = malloc(sizeof(token));
    char* str6 = malloc(sizeof(char)*3);
    str6[0]='|';
    str6[1]='|';
    str6[2]='\0';
    c6->type=OR_BOOL;
    c6->string = str6;
    c6->len = 2;

    //pwd (COMMAND)
    token* c7 = malloc(sizeof(token));
    char* str7 = malloc(sizeof(char)*4);
    str7[0]='p';
    str7[1]='w';
    str7[2]='d';
    str7[3]='\0';
    c7->type=COMMAND;
    c7->string = str7;
    c7->len = 3;

    //| (PIPE)
    token* c8 = malloc(sizeof(token));
    char* str8 = malloc(sizeof(char)*2);
    str8[0]='|';
    str8[1]='\0';
    c8->type=PIPE;
    c8->string = str8;
    c8->len = 1;

    //echo (COMMAND)
    token* c9 = malloc(sizeof(token));
    char* str9 = malloc(sizeof(char)*5);
    str9[0]='e';
    str9[1]='c';
    str9[2]='h';
    str9[3]='o';
    str9[4]='\0';
    c9->type=COMMAND;
    c9->string = str9;
    c9->len = 4;

    //& (AND)
    token* c10 = malloc(sizeof(token));
    char* str10 = malloc(sizeof(char)*2);
    str10[0]='|';
    str10[1]='\0';
    c10->type=AND;
    c10->string = str10;
    c10->len = 1;

    //( (LEFT_PAREN)
    token* c11 = malloc(sizeof(token));
    char* str11 = malloc(sizeof(char)*2);
    str11[0]='(';
    str11[1]='\0';
    c11->type=LEFT_PAREN;
    c11->string = str11;
    c11->len = 1;

    //) (RIGHT_PAREN)
    token* c12 = malloc(sizeof(token));
    char* str12 = malloc(sizeof(char)*2);
    str12[0]=')';
    str12[1]='\0';
    c12->type=RIGHT_PAREN;
    c12->string = str12;
    c12->len = 1;

    //` (BACKTICK)
    token* c13 = malloc(sizeof(token));
    char* str13 = malloc(sizeof(char)*2);
    str13[0]='`';
    str13[1]='\0';
    c13->type=BACKTICK;
    c13->string = str13;
    c13->len = 1;


    list_push_endT(l, c1); //cat
    list_push_endT(l, c2); //-japon
    list_push_endT(l, c3); //main.c
    list_push_endT(l, c4); //&&
    list_push_endT(l, c5); //ls
    list_push_endT(l, c6); //||
    list_push_endT(l, c7); //pwd
    list_push_endT(l, c8); //|
    list_push_endT(l, c9); //echo
    list_push_endT(l, c10);//&
    list_push_endT(l, c11);//(
    list_push_endT(l, c9); //echo
    list_push_endT(l, c13);//`
    list_push_endT(l, c5); //ls
    list_push_endT(l, c13);//`
    list_push_endT(l, c12);//)


    ast_node* node= creating_ast(l);
    int err = ast_print(node);
    
    free_ast(node);
    freeL(l);
    return err;
}
