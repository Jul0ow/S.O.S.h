#ifndef LEXER_H
#define LEXER_H
#include "vector.h"
#include "list.h"

typedef enum
{
    //Command etc..
    COMMAND,
    ARGUMENT,
    FLAGS,

    //Script
    IF,
    TYPE,
    VAR,
    WHILE,

    //Boolean
    FALSE, TRUE,
    NOT,
    AND_BOOL, AND_INT,
    OR_BOOL, OR_INT,
    EQUAL,
    GREATER_THAN, LESS_THAN,

    
    //Operator
    DIVIDE, MULT, MODULO,
    MINUS, PLUS,
    LEFT_SHIFT, RIGHT_SHIFT,
    OP_START = AND_BOOL,
    OP_END = RIGHT_SHIFT,

    //Misc
    ARROW,
    COMMA,
    COMMENT,
    COLON,
    LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET,
    LEFT_PAREN, RIGHT_PAREN,
    SEMI_COLON,

    TOK_EOF,

    UNKNOWN


} token_type;

typedef struct 
{
    token_type type;
    char *string;
    size_t len;
} token;

*list lexing(char *entry);

#endif
