#ifndef LEXER_H
#define LEXER_H
#include "vectorS.h"
#include "list.h"
#define TRUE 1
#define FALSE 0

typedef struct listT listT;
typedef struct list_elm list_elm;


typedef enum token_type
{
    //Command etc..
    COMMAND,
    ARGUMENT,
    
    //Separator
    AND_BOOL, OR_BOOL, AND,
    DRIGHT_CHEVRON, DLEFT_CHEVRON,
    RIGHT_CHEVRON, LEFT_CHEVRON,
    //COMMA,
    COMMENT,
    SEMI_COLON,
    LEFT_PAREN, RIGHT_PAREN,
    PIPE,
    QUOTE, DOUBLE_QUOTES, BACKTICK,

    UNKNOWN


}token_type;


//Token struct containing the string and the type of token
typedef struct token
{
    token_type type;
    char *string;
    size_t len;
} token;

listT* init_lexing(char *entry);

//Main function which go throught the string and return a linked list of token
void lexing(char *entry, listT* token, char end);

int isSeparator(char c);

//Function which create the next token
size_t read_word(char *p, int isCommand, listT *token_list);

int read_separator(char *p, listT *token_list);

//Return TRUE if the char is a Separator
int isSeparator(char c);

#endif
