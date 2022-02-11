#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "lexer.h"
#include "list.h"
#include <string.h>
#define TRUE 1
#define FALSE 0

int compareList(listT *l1, listT *l2)
{
    list_elm *p1= l1->head, *p2 = l2->head;

    while (p1 != NULL && p2 != NULL)
    {
        token *t1 = p1->token, *t2 = p2->token;
        //cr_log_info("type : %i || %i\n", t1->type, t2->type);
        if (t1->type != t2->type)
            return FALSE;
        if(t1->type == ARGUMENT || t1->type == COMMAND)
        {
            //cr_log_info("len : %li || %li\n", t1->len, t2->len);
            if (t1->len != t2->len)
                return FALSE;

            //cr_log_info("string : %s || %s\n", t1->string, t2->string);
            if (!(strcmp(t1->string, t2->string) == 0))
                return FALSE;
        }
        p1 = p1->next;
        p2 = p2->next;

    }

    return p1 == p2;
}

void freeL1(listT *l, int rToken)
{
    list_elm *elm = l->head;
    while(elm != NULL)
    {
        list_elm *next = elm->next;
        if (rToken)
        {
            if(elm->token->type == COMMAND || elm->token->type == ARGUMENT)
            free(elm->token->string);
            free(elm->token);
        }
        free(elm);
        elm = next;
    }
    //free(l);
}

Test(SOSH,simpleLexing)
{
    cr_log_info("Starting simple lexing example...");
    listT echo1;
    list_initT(&echo1);
    token c1 = {
            COMMAND,
            "echo",
            4
    };

    token c2 = {
            ARGUMENT,
            "bonjour",
            7
    };

    list_push_endT(&echo1, &c1);
    list_push_endT(&echo1, &c2);
    listT* l = init_lexing("echo bonjour");
    cr_expect(compareList(l, &echo1) == 1, 
            "trying to parse \"echo bonjour\"");
    freeL1(l, TRUE);
    freeL1(&echo1, FALSE);
    free(l);
}

Test(SOSH, separator)
{
    cr_log_info("Starting separator lexing example...");
    list echo1;
    list_init(&echo1);
    token c1 = {
            COMMAND,
            "alors",
            5
    };

    token c2 = {
            ARGUMENT,
            "oui",
            3
    };
    token c3 = {
            SEMI_COLON,
            ";",
            1
    };
    
    token c4 = {
            AND_BOOL,
            "&&",
            2
    };
    token c5 = {
            AND,
            "&",
            1
    };
    token c6 = {
            PIPE,
            "|",
            1
    };
    token c7 = {
            OR_BOOL,
            "||",
            2
    };

    token c8 = {
            QUOTE,
            "'",
            1
    };
    token c9 = {
            DOUBLE_QUOTES,
            "\"",
            1
    };
    token c10 = {
            BACKTICK,
            "`",
            1
    };

    token c12 = {
            DLEFT_CHEVRON,
            "<<",
            2
    };
    token c13 = {
            DRIGHT_CHEVRON,
            ">>",
            2
    };
    token c14 = {
            LEFT_CHEVRON,
            "<",
            1
    };
    token c15 = {
            RIGHT_CHEVRON,
            ">",
            1
    };
    token c16 = {
            LEFT_PAREN,
            "(",
            1
    };
    token c17 = {
            RIGHT_PAREN,
            ")",
            1
    };
    token c18 = {
            COMMENT,
            "#",
            1
    };
    token c19 = {
            ARGUMENT,
            ",",
            1
    };
    token c20 = {
            ARGUMENT,
            ",",
            1
    };
    token c21 = {
            ARGUMENT,
            ",",
            1
    };
    token c22 = {
            ARGUMENT,
            ",",
            1
    };
    token c23 = {
            ARGUMENT,
            ",",
            1
    };
    token c24 = {
            ARGUMENT,
            ",",
            1
    };
    token c25 = {
            ARGUMENT,
            ",",
            1
    };

    list_push_endT(&echo1, &c1); //echo
    list_push_endT(&echo1, &c2); //oui
    list_push_endT(&echo1, &c3); //;
    list_push_endT(&echo1, &c19); //,
    list_push_endT(&echo1, &c5);//&
    list_push_endT(&echo1, &c20);//,
    list_push_endT(&echo1, &c4);//&&
    list_push_endT(&echo1, &c6);//|
    list_push_endT(&echo1, &c21);//,
    list_push_endT(&echo1, &c7);//||
    list_push_endT(&echo1, &c8);//\'
    list_push_endT(&echo1, &c9);//\"
    list_push_endT(&echo1, &c10);//\`
    list_push_endT(&echo1, &c12);//<<
    list_push_endT(&echo1, &c22);//,
    list_push_endT(&echo1, &c13);//>>
    list_push_endT(&echo1, &c23);//,
    list_push_endT(&echo1, &c14);//<
    list_push_endT(&echo1, &c24);//,
    list_push_endT(&echo1, &c15);//>
    list_push_endT(&echo1, &c16);//(
    list_push_endT(&echo1, &c25);//,
    list_push_endT(&echo1, &c17);//)
    list_push_endT(&echo1, &c18);//#
    
    listT* l = init_lexing("alors oui;,&,&&|,||\'\"`<<,>>,<,>(,)#");
    cr_assert(compareList(l, &echo1) == 1, "Trying to parse separator");
    freeL(l, TRUE);
    freeL(&echo1, FALSE);
    free(l);
}
