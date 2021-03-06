#include "ast.h"
#include "xmalloc.h"
#include <unistd.h>
#include <string.h>
#include "lexer.h"
#include "listT.h"
#include <stdio.h>

ast_node* creating_ast(listT* list)
{
    ast_node* current = calloc(sizeof(ast_node),1);
    current->type = NODE_HEAD;
    current->data.node_head = NULL;
    char* str = malloc(sizeof(char)*5);
    str[0]='t';
    str[1]='r';
    str[2]='e';
    str[3]='e';
    str[4]='\0';
    current->string = str;
    current->nb_child=0;
    current->father = NULL;
    current->child = NULL;
    current->sibling = NULL;
    list_elm* p = list->head;
    int is_backtick = 0;
    size_t nb_parenthesis = 0;
    while(p!=NULL)
    {
        token t = *(p->token);
        if (t.type == COMMENT)
        {
            p = p->next;
            continue;
        }
        else if (t.type == BACKTICK && is_backtick == 1)
        {
            is_backtick = 0;
            while (current->type != NODE_BACKTICK &&
                    current->type != NODE_HEAD)
            {
                current = current->father;
            }
            if(current->type == NODE_HEAD)
            {
                //erreur de grammaire
            }
            p = p->next;
            continue;
        }
        else if(t.type == RIGHT_PAREN)
        {
            if(nb_parenthesis == 0)
                //erreur de grammaire
            nb_parenthesis --;
            while(current->type != NODE_LEFT_PAREN &&
                    current->type != NODE_HEAD)
            {
                current = current->father;
            }
            if(current->type == NODE_HEAD)
            {
                //erreur de grammaire
            }
            current->data.node_left_paren->closed =1;
            p=p->next;
            continue;
        }
        ast_node* new = calloc(sizeof(ast_node),1);
        new->type = NODE_HEAD;
        new->data.node_head = NULL;
        new->string = t.string;
        new->child = 0;
        new->father = NULL;
        new->child = NULL;
        new->sibling = NULL;
        switch(t.type)
        {
            case COMMAND:
                //remonte jusqu'au dernier s??parateur/la t??te
                while(!is_separator(current) &&
                      current->type!=NODE_HEAD &&
                      (current->type!=NODE_BACKTICK ||
                       is_backtick == 0)&&(current->type!=NODE_LEFT_PAREN ||
                           (current->type==NODE_LEFT_PAREN &&
                           current->data.node_left_paren->closed == 1)))
                {
                    current = current->father;
                }
                //si le s??parateur a plus d'un enfant ou que c'est la t??te et
                //qu'elle a d??j?? un enfant
                if(current->nb_child >1||
                        (current->type == NODE_HEAD &&
                         current->nb_child >0))
                {
                    //erreur de grammaire
                }
                new->type = NODE_COMMAND;
                create_command(new);
                new->father = current;
                current->nb_child++;
                if(current->child ==NULL)
                    current->child = new;
                else
                    current->child->sibling=new;
                break;
            case ARGUMENT:
                new->type = NODE_ARGUMENT;
                new->father = current;
                //check si le current est un argument, ou si le p??re est un 
                //s??parateur ou une commande et que son premier fils est diff??
                //de NULL
                if((current->type==NODE_ARGUMENT ||
                    (current->type == NODE_BACKTICK &&
                     is_backtick == 0)) &&
                   current->father->type==NODE_COMMAND)
                {
                    current->sibling = new;
                    new->father = current->father;
                    current->father->nb_child++;
                }
                else if(current->type==NODE_ARGUMENT ||
                        (current->type == NODE_BACKTICK &&
                         is_backtick == 1)||(current->type == NODE_LEFT_PAREN))
                {
                    //error dans la grammaire
                }
                else if(is_separator(current)&&
                        !is_chevron(current))
                {
                    //error : father is a separator (but not a chevron)
                }
                else if(is_chevron(current))
                {
                    current = current->child;
                    current->sibling = new;
                    new->father = current->father;
                    current->father->nb_child++;
                }
                else
                {
                    current->child = new;
                    new->father = current;
                }
                create_argument(new);
                break;
            case AND_BOOL:
                if(is_separator(current))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(current->type==NODE_ARGUMENT)
                {
                    //car un and ne peut pas s??parer deux arg
                    current=current->father;
                }
                while(is_separator(current->father))
                {
                    //car le && doit avoir lieu apr??s l'exec des pr??c??dents
                    current = current->father;
                }
                new->type = NODE_AND_BOOL;
                create_and_bool(new);
                new->father = current->father;
                new->father->child = new;
                current->father = new;
                new->child = current;
                new->nb_child ++;
                break;
            case OR_BOOL:
                if(is_separator(current))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(current->type==NODE_ARGUMENT)
                {
                    //car un or ne peut pas s??parer deux arg
                    current=current->father;
                }
                while(is_separator(current->father))
                {
                    //car le || doit avoir lieu apr??s l'exec des pr??c??dents
                    current = current->father;
                }
                new->type = NODE_OR_BOOL;
                create_or_bool(new);
                new->father = current->father;
                new->father->child = new;
                current->father = new;
                new->child = current;
                new->nb_child ++;
                break;
            case AND:
                if(is_separator(current))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(current->type==NODE_ARGUMENT)
                {
                    //car un and ne peut pas s??parer deux arg
                    current=current->father;
                }
                while(is_separator(current->father))
                {
                    //car le & doit avoir lieu apr??s l'exec des pr??c??dents
                    current = current->father;
                }
                new->type = NODE_AND;
                create_and(new);
                new->father = current->father;
                new->father->child = new;
                current->father=new;
                new->child = current;
                new->nb_child++;
                break;
            case DRIGHT_CHEVRON:
                if(is_separator(current))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(current->type==NODE_ARGUMENT)
                {
                    //car un and ne peut pas s??parer deux arg
                    current=current->father;
                }
                while(is_separator(current->father))
                {
                    //car le >> doit avoir lieu apr??s l'exec des pr??c??dents
                    current = current->father;
                }
                new->type = NODE_DRIGHT_CHEVRON;
                create_dright_chevron(new);
                new->father = current->father;
                new->father->child = new;
                current->father=new;
                new->child = current;
                new->nb_child++;
                break;
            case DLEFT_CHEVRON:
                if(is_separator(current))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(current->type==NODE_ARGUMENT)
                {
                    //car un and ne peut pas s??parer deux arg
                    current=current->father;
                }
                while(is_separator(current->father))
                {
                    //car le << doit avoir lieu apr??s l'exec des pr??c??dents
                    current = current->father;
                }
                new->type = NODE_DLEFT_CHEVRON;
                create_dleft_chevron(new);
                new->father = current->father;
                new->father->child = new;
                current->father=new;
                new->child = current;
                new->nb_child++;
                break;
            case RIGHT_CHEVRON:
                if(is_separator(current))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                while(is_separator(current->father))
                {
                    //car le > doit avoir lieu apr??s l'exec des pr??c??dents
                    current = current->father;
                }
                new->type = NODE_RIGHT_CHEVRON;
                create_right_chevron(new);
                new->father = current->father;
                new->father->child = new;
                current->father=new;
                new->child = current;
                new->nb_child++;
                break;
            case LEFT_CHEVRON:
                if(is_separator(current))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(current->type==NODE_ARGUMENT)
                {
                    //car un and ne peut pas s??parer deux arg
                    current=current->father;
                }
                while(is_separator(current->father))
                {
                    //car le < doit avoir lieu apr??s l'exec des pr??c??dents
                    current = current->father;
                }
                new->type = NODE_LEFT_CHEVRON;
                create_left_chevron(new);
                new->father = current->father;
                new->father->child = new;
                current->father=new;
                new->child = current;
                new->nb_child++;
                break;
            case SEMI_COLON:
                if(is_separator(current))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(current->type==NODE_ARGUMENT)
                {
                    //car un semi_colon ne peut pas s??parer deux arg
                    current=current->father;
                }
                while(is_separator(current->father))
                {
                    //car le semi_colon doit avoir lieu apr??s l'exec des pr??c??dents
                    current = current->father;
                }
                new->type = NODE_SEMI_COLON;
                create_semi_colon(new);
                new->father = current->father;
                new->father->child = new;
                current->father=new;
                new->child = current;
                new->nb_child++;
                break;
            case LEFT_PAREN:
                while(!is_separator(current) &&
                      current->type!=NODE_HEAD &&
                      (current->type!=NODE_BACKTICK ||
                       is_backtick == 0)&&(current->type!=NODE_LEFT_PAREN ||
                           (current->type==NODE_LEFT_PAREN &&
                           current->data.node_left_paren->closed == 0)))
                {
                    current = current->father;
                }
                //si le s??parateur a plus d'un enfant ou que c'est la t??te et
                //qu'elle a d??j?? un enfant
                if(current->nb_child >1||
                        (current->type == NODE_HEAD &&
                         current->nb_child >0))
                {
                    //erreur de grammaire
                }
                new->type = NODE_LEFT_PAREN;
                create_left_paren(new);
                new->father = current;
                current->nb_child++;
                if(current->child ==NULL)
                    current->child = new;
                else
                    current->child->sibling=new;
                break;
            case PIPE:
                if(is_separator(current))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(current->type==NODE_ARGUMENT)
                {
                    //car un pipe ne peut pas s??parer deux arg
                    current=current->father;
                }
                while(is_separator(current->father))
                {
                    //car le pipe doit avoir lieu apr??s l'exec des pr??c??dents
                    current = current->father;
                }
                new->type = NODE_PIPE;
                create_pipe(new);
                new->father = current->father;
                new->father->child = new;
                current->father=new;
                new->child = current;
                new->nb_child++;
                break;
            case BACKTICK:
                is_backtick = 1;
                new->type = NODE_BACKTICK;
                if((current->type==NODE_ARGUMENT ||
                    current->type == NODE_BACKTICK) &&
                    current->father->type==NODE_COMMAND)
                {
                    current->sibling = new;
                    new->father = current->father;
                    current->father->nb_child++;
                }
                else if(current->type==NODE_ARGUMENT ||
                        current->type == NODE_BACKTICK)
                {
                    //error dans la grammaire
                }
                else if(is_separator(current)&&
                        !is_chevron(current))
                {
                    //error : father is a separator (but not a chevron)
                }
                else if(is_chevron(current))
                {
                    current = current->child;
                    current->sibling = new;
                    new->father = current->father;
                    current->father->nb_child++;
                }
                else
                {
                    current->child = new;
                    new->father = current;
                }
                create_backtick(new);
                break;
            default:
                //unidentified token
                break;
        }
        current = new;
        p = p->next;
    }
    if(nb_parenthesis != 0)
    {
        //erreur de grammaire
    }
    while(current->type != NODE_HEAD)
    {
        current = current->father;
    }
    return current;
}

void free_ast(ast_node* node)
{
    //free r??cursivement le contenu des nodes, avant de free les nodes
    //elle-m??me (il faut commencer depuis la racine)
    switch(node->type)
    {
        case NODE_HEAD:
            free(node->data.node_head);
            break;
        case NODE_AND_BOOL:
            free(node->data.node_and_bool);
            break;
        case NODE_OR_BOOL:
            free(node->data.node_or_bool);
            break;
        case NODE_AND:
            free(node->data.node_and);
            break;
        case NODE_DRIGHT_CHEVRON:
            free(node->data.node_dright_chevron);
            break;
        case NODE_DLEFT_CHEVRON:
            free(node->data.node_dleft_chevron);
            break;
        case NODE_RIGHT_CHEVRON:
            free(node->data.node_right_chevron);
            break;
        case NODE_LEFT_CHEVRON:
            free(node->data.node_left_chevron);
            break;
        case NODE_UNKNOWN:
            free(node->data.node_unknown);
            break;
        case NODE_COMMAND:
            free(node->data.node_command);
            break;
        case NODE_ARGUMENT:
            free(node->data.node_argument);
            break;
        case NODE_SEMI_COLON:
            free(node->data.node_semi_colon);
            break;
        case NODE_LEFT_PAREN:
            free(node->data.node_left_paren);
            break;
        case NODE_RIGHT_PAREN:
            free(node->data.node_right_paren);
            break;
        case NODE_PIPE:
            free(node->data.node_pipe);
            break;
        case NODE_QUOTE:
            free(node->data.node_quote);
            break;
        case NODE_DOUBLE_QUOTES:
            free(node->data.node_double_quotes);
            break;
        case NODE_BACKTICK:
            free(node->data.node_backtick);
            break;
        default:
            //an error occured
            break;
    }
    if(node->child != NULL)
        free_ast(node->child);
    if(node->sibling != NULL)    
        free_ast(node->sibling);
    free(node);
}


// Fonctions pour check des trucs sans se repeter 20x
int is_separator(ast_node* current)
{
    //v??rifie si la node est un s??parateur
    switch(current->type)
    {
        case NODE_AND_BOOL:
        case NODE_OR_BOOL:
        case NODE_AND:
        case NODE_SEMI_COLON:
        case NODE_DRIGHT_CHEVRON:
        case NODE_DLEFT_CHEVRON:
        case NODE_RIGHT_CHEVRON:
        case NODE_LEFT_CHEVRON:
        case NODE_PIPE:
            return 1;
        default:
            return 0;
    }
}

int is_chevron(ast_node* current)
{
    //verifie si la node est un chevron
    switch(current->type)
    {
        case NODE_DRIGHT_CHEVRON:
        case NODE_DLEFT_CHEVRON:
        case NODE_RIGHT_CHEVRON:
        case NODE_LEFT_CHEVRON:
            return 1;
        default:
            return 0;
    }
}

void create_or_bool(ast_node* new)
{
    struct node_or_bool* new_data = calloc(sizeof(struct node_or_bool),1);
    new_data->node = new;
    new->data.node_or_bool=new_data;
}

void create_left_paren(ast_node* new)
{
    struct node_left_paren* new_data = calloc(sizeof(struct node_left_paren),1);
    new_data->node = new;
    new_data->closed = 0;
    new->data.node_left_paren=new_data;
}

//<<
void create_dleft_chevron(ast_node* new)
{
    struct node_dleft_chevron* new_data =
        calloc(sizeof(struct node_dleft_chevron),1);
    new_data->node=new;
    new->data.node_dleft_chevron=new_data;
}

//>>
void create_dright_chevron(ast_node* new)
{
    struct node_dright_chevron* new_data=
        calloc(sizeof(struct node_dright_chevron),1); 
    new_data->node = new;
    new->data.node_dright_chevron=new_data;
}

//<
void create_left_chevron(ast_node* new)
{
    struct node_left_chevron* new_data = 
        calloc(sizeof(struct node_left_chevron),1);
    new_data->node = new;
    new->data.node_left_chevron=new_data;
}

//>
void create_right_chevron(ast_node* new)
{
    struct node_right_chevron* new_data = 
        calloc(sizeof(struct node_right_chevron),1);
    new_data->node = new;
    new->data.node_right_chevron=new_data;
}

void create_and_bool(ast_node* new)
{
    struct node_and_bool* new_data = 
        calloc(sizeof(struct node_and_bool),1);
    new_data->node = new;
    new->data.node_and_bool = new_data;
}

void create_argument(ast_node *new)
{
    struct node_argument* new_data = 
        calloc(sizeof(struct node_argument),1);
    new_data->node = new;
    new->data.node_argument = new_data;
}

void create_command(ast_node *new)
{
    struct node_command* new_data = 
        calloc(sizeof(struct node_command),1);
    new_data->node = new;
    new_data->cmd = DEFAULT;
    new->data.node_command = new_data;
    check_command(new);
}

void create_and(ast_node* new)
{
    struct node_and* new_data = 
        calloc(sizeof(struct node_and),1);
    new_data->node = new;
    new->data.node_and=new_data;
}

void create_semi_colon(ast_node* new)
{
    struct node_semi_colon* new_data =
        calloc(sizeof(struct node_semi_colon),1);
    new_data->node = new;
    new->data.node_semi_colon=new_data;
}

void create_pipe(ast_node* new)
{
    struct node_pipe* new_data = 
        calloc(sizeof(struct node_pipe),1);
    new_data->node = new;
    new->data.node_pipe=new_data;
}

void create_backtick(ast_node* new)
{
    struct node_backtick* new_data = 
        calloc(sizeof(struct node_backtick),1);
    new_data->node = new;
    new->data.node_backtick=new_data;
}

//function to check errors for command
void check_command(ast_node* new)
{
    /*
       v??rifie si la commande entr??e est valide
       */
    new->data.node_command->cmd = DEFAULT;
    if (strcmp(new->string, "cat") == 0)
    {
        new->data.node_command->cmd = CAT;
    }
    else if (strcmp(new->string, "pwd") == 0)
    {
        new->data.node_command->cmd = PWD;
    }
    else if (strcmp(new->string, "ls") == 0)
    {
        new->data.node_command->cmd = LS;
    }

    // autres commandes

    //default

    /*
    else
    {
        if(strlen(new->string)>2)
        {
            if((char*)new->string[0]=='/'||
               ((char*)new->string[1])=='/'&&
               ((char*)new->string[0]=='.'))
            {
                //v??rifie si le fichier est bien un ex??cutable
                if(access(new->string,X_OK == 0))
                    new->data.node_command->cmd = DEFAULT;
                else
                    new->type = NODE_UNKNOWN;
                free(new->data.node_command);
                struct node_unknown data = {
                    new,
                    EXEC_NOT_FOUND,
                };
                new->data.node_unknown = &data;
            }
        }
    }
    */

    /*
    switch(new->string)
    {
        case "cat":
            new->data.node_command->cmd = CAT;
            break;

            //ajouter ici les prochaines commandes

        default:
            if(strlen(new->string)>2)
            {
                if((char*)new->string[0]=='/'||
                   ((char*)new->string[1])=='/'&&
                    ((char*)new->string[0]=='.'))
                {
                    //v??rifie si le fichier est bien un ex??cutable
                    if(access(new->string,X_OK == 0))
                        new->data.node_command->cmd = DEFAULT;
                    else
                        new->type = NODE_UNKNOWN;
                    free(new->data.node_command);
                    struct node_unknown data = {
                        new,
                        EXEC_NOT_FOUND,
                    };
                    new->data.node_unknown = &data;
                }
            }
            break;
    }
    */
}

