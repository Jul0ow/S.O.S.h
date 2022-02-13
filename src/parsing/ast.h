#ifndef AST_H
#define AST_H

#include "ast.h"
#include "listT.h"
//enum of all commands
enum cmd
{
    CAT,
    //add here future commands once they're implemente
    DEFAULT
    //...
};
typedef struct ast_node ast_node;
//node of the ast
struct ast_node
{
    //type of node (~= token)
    enum node_type
    {
        //Head node
        NODE_HEAD,
        //Commands etc...
        NODE_COMMAND,
        NODE_ARGUMENT,

        //Separator
        NODE_AND_BOOL,
        NODE_OR_BOOL,
        NODE_AND,
        NODE_DRIGHT_CHEVRON,
        NODE_DLEFT_CHEVRON,
        NODE_RIGHT_CHEVRON,
        NODE_LEFT_CHEVRON,

        //COMMA ??????????
        NODE_COMMENT,
        NODE_SEMI_COLON,
        NODE_LEFT_PAREN,
        NODE_RIGHT_PAREN,
        NODE_PIPE,
        NODE_QUOTE,
        NODE_DOUBLE_QUOTES,
        NODE_BACKTICK,

        NODE_UNKNOWN

        //DO = command ?
        //WHILE = arguments ?
        //DONE = end of command ?
    } type;

    //struct of each type of node (matches the node_type)
    union
    {
        //Head
        struct node_head    *node_head;
        //Commands etc...
        struct node_command *node_command;
        struct node_argument *node_argument;

        //Separator
        struct node_and_bool *node_and_bool;
        struct node_or_bool *node_or_bool;
        struct node_and *node_and;
        struct node_dright_chevron *node_dright_chevron;
        struct node_dleft_chevron *node_dleft_chevron;
        struct node_right_chevron *node_right_chevron;
        struct node_left_chevron *node_left_chevron;

        //COMMA ??????????
        struct node_comment *node_comment;
        struct node_semi_colon *node_semi_colon;
        struct node_left_paren *node_left_paren;
        struct node_right_paren *node_right_paren;
        struct node_pipe *node_pipe;
        struct node_quote *node_quote;
        struct node_double_quotes *node_double_quotes;
        struct node_backtick *node_backtick;

        struct node_unknown *node_unknown;
    } data;

    //litteral expression
    void *string;

    //first child / right sibling implementation
    
    int nb_child;

    ast_node *child;

    ast_node *sibling;

    ast_node *father;
};

// === STRUCTS DEF ===

//command
struct node_command
{
    enum cmd cmd;
    ast_node *node;
};

//argument
struct node_argument
{
    ast_node *node;
};

struct node_and_bool
{
    ast_node *node;    
};

struct node_or_bool
{
    ast_node *node;  
};

struct node_and
{
    ast_node *node;
};

struct node_dright_chevron
{
    ast_node *node;
};

struct node_dleft_chevron
{
    ast_node *node;
};

struct node_right_chevron
{
    ast_node *node;
};

struct node_left_chevron
{
    ast_node *node;
};

struct node_semi_colon
{
    ast_node *node;
};

struct node_pipe
{
    ast_node *node;
};

struct node_backtick
{
    ast_node *node;
};

struct node_left_paren
{
    ast_node *node;
    int closed;
};

//unknown
struct node_unknown
{
    //every error we can encounter (will be handled in execution part)
    enum error_type
    {
        EXEC_NOT_FOUND,
        DEFAULT_ERROR
        //add here future error type
    } type;
};

struct node_head{
    ast_node* node;
};

// === Initialisation foncitons ===

ast_node* creating_ast(listT* list);

void free_ast(ast_node* node);

int is_separator(ast_node* current);

int is_chevron(ast_node* current);

void create_or_bool(ast_node* new);

void create_left_paren(ast_node* new);

void create_dleft_chevron(ast_node* new);

void create_dright_chevron(ast_node* new);

void create_left_chevron(ast_node* new);

void create_right_chevron(ast_node* new);

void create_and(ast_node* new);

void create_and_bool(ast_node* new);

void create_argument(ast_node *new);

void create_command(ast_node *new);

void create_and(ast_node* new);

void create_semi_colon(ast_node* new);

void create_pipe(ast_node* new);

void create_backtick(ast_node* new);

void check_command(ast_node* new);


#endif
