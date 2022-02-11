#ifndef AST_H
#define AST_H

//enum of all commands
enum cmd
{
    CAT,
    //add here future commands once they're implemente
    DEFAULT
    //...
}

//node of the ast
typedef struct ast_node
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
        //Commands etc...
        struct node_command *node_command;
        struct node_argument *node_argument;

        //Separator
        struct node_and_bool *node_and_bool;
        struct node_or_bool *node_or_bool;
        struct node_and *node_and;
        struct node_dright_chevron *node_dright_chevron;
        struct node_dleft_chevron *node_dleft_chevron;

        //COMMA ??????????
        struct node_comment *node_command;
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
    
    size_t nb_child = 0;

    ast_node *child = NULL;

    ast_node *sibling = NULL;

    ast_node *father = NULL;
};

// === STRUCTS DEF ===

//command
struct node_command
{
    enum cmd cmd;
};

//argument
struct node_argument
{

};

//unknown
struct node_unknown
{
    //every error we can encounter (will be handled in execution part)
    enum error_type
    {
        EXEC_NOT_FOUND
        //add here future error type
    } type;
};

#endif
