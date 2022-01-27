#ifndef LEXER_H
#define LEXER_H
#include "vector.h"
#include "list.h"
#define TRUE 1
#define FALSE 0

typedef enum
{
    //Command etc..
    COMMAND,
    ARGUMENT,
    
    //Separator
    AND_BOOL, OR_BOOL,
    RIGHT_CHEVRON, LEFT_CHEVRON,
    COMMA,
    COMMENT,
    SEMI_COLON,
    LEFT_PAREN, RIGHT_PAREN,
    PIPE,
    QUOTE, DOUBLE_QUOTES,

    UNKNOWN


} token_type;

//Token struct containing the string and the type of token
typedef struct 
{
    token_type type;
    char *string;
    size_t len;
} token;

//Main function which go throught the string and return a linked list of token
*list lexing(char *entry);

//Function which create the next token
void read_word(char *p, token token);

//Return TRUE if the char is a Separator
int isSeparator(char c);

#endif
